# PFGameSave Debugging Guide

> Reference for SDK engineers debugging PFGameSave issues — test failures, customer bug reports, or service incidents.
> Covers four data sources (ETL, Kusto, PGS, Inproc logs), investigation methodology, test architecture, known bug patterns, and diagnostic recipes.

### Quick Navigation

| Section | Topic | Use When |
|---------|-------|----------|
| **§0** | Investigation Methodology | Starting any investigation — read this first |
| **§1** | Four Data Sources | Understanding what data is available |
| **§2** | ETL Traces | Debugging GRTS behavior, HTTP errors, state machines |
| **§3** | Kusto Telemetry | Querying SDK + GRTS telemetry (two sources), failure rates, historical patterns |
| **§4** | Correlating Sources | Cross-referencing ETL + Kusto + PGS + Inproc |
| **§5** | GRTS On-Disk State (PGS) | Examining device-local save state |
| **§6** | Inproc SDK Debug Logs | Tracing SDK calls, parameters, async lifecycle |
| **§7** | Golden Path Telemetry | Expected markers for healthy test runs |
| **§8** | Telemetry Gaps | Known missing data and workarounds |
| **§9** | Analysis Checklist | Step-by-step investigation workflow |
| **§10** | Live Data Analysis | Golden path reference data (pass154/01) |
| **§11** | Debugging Recipes | Known bug patterns with diagnostic steps |
| **§12** | Error Code Reference | HRESULT codes, HTTP status, diagnostic tree |
| **§13** | Manifest Version Lifecycle | Version fields, state transitions, diagnostics |
| **§14** | Timeline Reconstruction | Cross-device timeline methodology |
| **§15** | Quota & Storage | Blob counts, size limits, recovery |
| **§16** | Conflict Scenario Baseline | Conflict telemetry reference (xplat-14) |
| **§17** | Test Failure Debugging | Test didn't pass — triage, re-run, common non-bug failures |
| **§18** | GRTS Service Debugging | GRTS crashes, crash dumps, reinstallation |
| **§19** | Offline→Online Transition | Network disruption tests, AdminHelper, FlushGrtsAndGoOffline |
| **§20** | Inproc ↔ GRTS Provider Switching | Selecting provider per test, engine strings, CLI filtering |
| **§21** | isWinner Inversion — Commit Facts | Server-side conflict resolution changes (bc220a2, 69e0b63) |
| **§22** | Platform Type Reference | platformType telemetry values by environment |
| **§23** | SteamDeck Workflow (TBD) | SteamDeck-specific debugging — not yet documented |
| **§24** | GRTS Upload & Conflict Lifecycle | Understanding how GRTS uploads, detects conflicts, resolves them |

### Local Source Paths

These are the local repo paths for the major components involved in PFGameSave:

| Component | Path | What's Here |
|-----------|------|-------------|
| **PlayFab SDK** | `C:\git\PlayFab.C\Source` | PFGameSave SDK source, public API, sync manager, providers |
| **GRTS (ConnectedStorage)** | `C:\git\ConnectedStorage` | Gaming Runtime Transport Service — the OS-level save transport on Xbox/PC |
| **GameSave Service** | `C:\git\GS.GameSave` | Server-side GameSave service (ManifestServiceLogic, isWinner handling) |
| **libHttpClient** | `C:\git\libHttpClient` | HTTP layer used by the SDK for PlayFab API calls |
| **Test Scenarios** | `C:\git\PlayFab.C\Test\GameTestScenarios` | YAML test scenarios (`gamesave\` for InProc, `gamesave-pc\` for GRTS+InProc) |
| **Test Controller** | `C:\git\PlayFab.C\Test\GameTestController` | C# test orchestrator |
| **Test Device** | `C:\git\PlayFab.C\Test\GameTestAppWindows` | C++ test harness running on device |

---

## 0. Investigation Methodology — ALWAYS Follow This Process

> **This section is MANDATORY.** Every PFGameSave investigation — test failure or customer-reported bug — follows this two-phase process. No exceptions. The rest of this guide provides the tools; this section tells you how and when to use them.

### Why Two Phases?

Confirmation bias kills investigations. When you simultaneously collect facts AND guess root causes, you unconsciously filter evidence to fit your first guess. The fix: **separate what happened from why.**

- **Phase 1 — Fact-Finding Report:** Collect ONLY undisputable facts. No interpretation. No speculation. No root cause guessing. This produces a shareable document for the vteam.
- **Phase 2 — Root Cause Analysis:** Using the fact document, form hypotheses and test them against code (client SDK, inproc, service, GRTS). Each hypothesis is explicitly tracked as confirmed, rejected, or inconclusive.

### The Iterative Loop

This is NOT waterfall. Phase 2 will expose gaps in your fact document. When that happens:

1. Go back to Phase 1. Add the new facts.
2. Resume Phase 2 with the enriched fact base.

**This is expected and healthy — most investigations require 2–3 iterations.** The fact document is a LIVING document that gets enriched as Phase 2 exposes gaps.

### Investigation Output Files

All investigation artifacts go in a dated directory:

```text
investigations/{YYYY-MM-DD}-{slug}/
├── fact-report.md           # Phase 1 output
└── root-cause-analysis.md   # Phase 2 output
```

Example: `investigations/2026-05-12-test42-timeout/fact-report.md`

### Phase 1: Fact-Finding Report

Use these guide sections to populate the fact document:

| Fact Document Section | Guide Section to Use |
|---|---|
| Timeline | **§2** (ETL Traces) + **§14** (Timeline Reconstruction) |
| ETL Trace Highlights | **§2** (ETL Traces) |
| Kusto Telemetry | **§3** (Kusto Telemetry) |
| PGS / On-Disk State | **§5** (GRTS On-Disk State) |
| Inproc SDK Log Highlights | **§6** (Inproc SDK Debug Logs) |
| Systematic coverage | **§9** (Analysis Checklist) |
| Known patterns | **§11** (Debugging Recipes) |

#### Fact Document Template

Copy this template into `investigations/{date}-{slug}/fact-report.md` and fill in each section. **Do NOT skip sections — mark them "NOT AVAILABLE" if data is missing.**

```markdown
# Investigation: {Brief Title}
**Date:** {date}
**Source:** {test failure | customer report | internal report}
**Reporter:** {who reported it}
**Investigator(s):** {who is investigating}

## F1. Bug Report
Rewrite the original bug report into a clear, self-contained description. Do NOT link to external files
or paste raw email/chat — distill it into something anyone can read cold and understand immediately.
Include:
- What the user/test was doing (step by step)
- What was expected to happen
- What actually happened
- Repro steps (if known)
- Error codes, HRESULTs (if any)
- Affected user identifiers (gamertag, XUID, PlayFab ID, device name)
- Platforms and GRTS/Gaming Services versions involved

## F2. Timeline
Chronological events from ALL available data sources, correlated by timestamp.
Format: `[HH:MM:SS.mmm] [Source] Event description`
Sources: ETL, Kusto, Inproc logs, PGS state, test output

## F3. ETL Trace Highlights
Relevant events extracted from ETL. Include:
- The exact events (copy/paste from trace output)
- The `read-grts-etl.py` command used to extract them
- Time range, PID, key event types
- Note: flush ETL first with `logman update GamingServices -ets -fd`

## F4. Kusto Telemetry
Include for EACH query:
- The exact KQL query (copy-pasteable)
- The cluster/database used
- The results (summarized or key rows)
- Time range queried
DO NOT interpret results — just present them.

## F5. Inproc SDK Log Highlights
Relevant excerpts from device-*.txt or game engine logs. Include:
- File path where logs were found
- Key log lines (with timestamps)
- Any error messages or unexpected state transitions

## F6. PGS / On-Disk State (if available)
Current GRTS state from C:\XboxGames\GameSave\pgs\ or customer-provided data.
- Extended manifest contents (if dumped via PFGameSaveFilesSetWriteManifestsToDiskForDebug)
- Blob cache state
- Container metadata
Note: Often NOT available for customer reports — mark "NOT AVAILABLE" if so.

## F7. What Worked Correctly
Parts of the pipeline that functioned as expected. This narrows the search space.
Example: "Context init succeeded, manifest fetched, upload plan created — failure happened during chunk upload"

## F8. What's Missing
Data gaps — things you looked for but couldn't find or weren't available.
This itself is valuable information for Phase 2.
```

### Phase 2: Root Cause Analysis

Use these guide sections to test hypotheses:

| Analysis Need | Guide Section to Use |
|---|---|
| HRESULT meaning | **§12** (Error Code Reference) |
| Manifest version state | **§13** (Manifest Version Lifecycle) |
| Cross-device timeline | **§14** (Timeline Reconstruction) |
| Storage/quota issues | **§15** (Quota & Storage Debugging) |
| Conflict behavior | **§16** (Conflict Scenario Baseline) |

#### Root Cause Analysis Template

Copy this template into `investigations/{date}-{slug}/root-cause-analysis.md`. Each hypothesis is stated, tested against facts, and explicitly marked.

```markdown
# Root Cause Analysis: {Brief Title}
**Fact Report:** [fact-report.md](./fact-report.md)
**Date:** {date}
**Investigator(s):** {who is investigating}

## R1. Hypotheses

### Hypothesis 1: {Brief description}
**Based on facts:** {which facts from the fact report support investigating this}
**Test:** {how to confirm/reject — code path to check, query to run, test to reproduce}
**Result:** CONFIRMED | REJECTED | INCONCLUSIVE
**Evidence:** {what you found}

### Hypothesis 2: {Brief description}
**Based on facts:** {which facts from the fact report support investigating this}
**Test:** {how to confirm/reject — code path to check, query to run, test to reproduce}
**Result:** CONFIRMED | REJECTED | INCONCLUSIVE
**Evidence:** {what you found}

(Repeat for each hypothesis. Do not stop at one — even if the first looks right, check alternatives.)

## R2. Root Cause
{Only written when a hypothesis is CONFIRMED. Reference the hypothesis number.}

## R3. Recommended Fix
{What should change — code, config, process. Be specific: file paths, function names, behavioral change.}

## R4. Prevention
{How to prevent recurrence — test coverage, monitoring, assertions, telemetry alerts.}
```

### Hypothesis Discipline

- **State each hypothesis explicitly** before testing it. No "I think it might be..." in your head — write it down.
- **Test against facts, not intuition.** Every hypothesis check references specific facts from the Phase 1 document.
- **Mark the result.** CONFIRMED, REJECTED, or INCONCLUSIVE. Inconclusive means you need more facts — loop back to Phase 1.
- **Note what worked correctly.** Narrowing the search space is just as valuable as finding the bug. If context init, manifest fetch, and upload plan creation all succeeded, say so — it tells the next investigator where NOT to look.
- **Don't stop at one hypothesis.** Even if Hypothesis 1 looks right, briefly consider alternatives. Confirmation bias is real.

### When to Loop Back to Phase 1

Return to Phase 1 and add new facts when:

- A hypothesis test reveals you're missing a data source (e.g., you checked ETL but never pulled Kusto)
- You find a timeline gap — events jump from success to failure with nothing in between
- The code path you're investigating references state you haven't captured
- Your hypothesis is INCONCLUSIVE because the fact document doesn't cover the relevant area

Add the new facts to the existing fact document (it's a living document), then resume Phase 2.

### Reproduce with a Test Scenario

Once a hypothesis is CONFIRMED (or strongly suspected), try to reproduce the bug with an automated test scenario. A reproducing test is the strongest possible evidence — it proves the failure is real and gives you a regression gate for the fix.

#### Step 1: Find a Baseline Scenario

Start from an existing YAML scenario that's closest to the bug's conditions:

- **GRTS or Xbox involved** (cross-platform, conflict resolution, isWinner, GRTS upload/download):
  Start from `C:\git\PlayFab.C\Test\GameTestScenarios\gamesave-pc\`. These scenarios use the GRTS+InProc dual-stack and cover cross-device contention, conflicts, and platform interop.

- **Steam Deck or InProc-only** (single-platform, SDK-only saves, quota, storage, offline):
  Start from `C:\git\PlayFab.C\Test\GameTestScenarios\gamesave\`. These scenarios use the InProc SDK only.

Browse the folder and find the scenario closest to the bug's conditions. For example:
- Conflict resolution bug → start from `gamesave-pc-14-conflict-local-wins.yml` or `gamesave-pc-15-conflict-cloud-wins.yml`
- Cross-platform save stomp → start from `gamesave-pc-121-cross-platform-version-ping-pong-iswinner-regression.yml`
- Contention / active device → start from `gamesave-pc-05-contention-takeover.yml`
- Upload failure → start from `gamesave-pc-84-rapid-upload-rate-limit.yml`
- Offline/reconnect → start from `gamesave-03-offline-to-online-reconnect.yml`
- Storage/quota → start from `gamesave-12-out-of-storage.yml`

#### Step 2: Create a Reproducing Scenario

Copy the baseline YAML and modify it to match the bug's repro conditions. Key things to adjust:
- **Devices**: Add/remove devices to match the bug (single vs multi-device, GRTS vs InProc)
- **Preconditions**: Seed the right files, cloud state, conflict state
- **Trigger**: Modify the sequence to hit the exact failure path
- **Assertions**: Add `expectedHr` on the command that should fail, or use snapshot comparisons to detect data loss

Tag the new scenario as `failing` until the fix is applied:
```yaml
tags:
  - failing
  - gamesaves
```

#### Step 3: Confirm the Failure

Run the scenario WITHOUT any product fix. The test must fail with the same symptom as the original bug (same HRESULT, same state machine step, same data loss pattern). If it passes, the scenario doesn't reproduce the bug — adjust and retry.

Record the confirmed failure in the root-cause-analysis under **R2. Root Cause**.

#### Step 4: After Fixing

Once the fix is applied and the test passes, change the tag from `failing` to `passing` and add the scenario path to **R4. Prevention** as the regression test.

---

## 1. Four Data Sources

PFGameSave has **four independent data sources** that capture different layers of the system:

> **Data Availability Note (For Customer Bug Reports):**  
> When investigating customer-reported issues, **ETL is typically provided by the customer** (they can dump `GamingServices.etl` from their PC), and **Kusto is always available** (you have universal access to PlayFab telemetry). However, **PGS on-disk state is rarely available** — most customers cannot easily share their protected PGS directory. When you lack PGS data, rely on ETL + Kusto correlation to infer device state.

| Data Source | What It Captures | Where It Lives | When to Use |
|-------------|-----------------|----------------|-------------|
| **ETL (ETW Traces)** | GRTS service internals: HTTP requests, state machine transitions, upload lifecycle, process events | `C:\Windows\System32\LogFiles\WMI\GamingServices.etl` | Debugging GRTS behavior, timing, HTTP errors, state machine failures |
| **Kusto (PlayFab Telemetry)** | SDK-emitted telemetry events: activation, sync, errors, deletes; **server-authoritative version finalization**; and **GS.GameSave service logs** (suspected-data-loss + auto-rollback decisions) | `playfabinternalreader.westus2.kusto.windows.net` (PlayFabInternal), `gaming.westus.kusto.windows.net` (Gaming/UTCEvents), `pmain01sharded004.westus2.kusto.windows.net`/`ShardedDB023` (production shard — `playfab.gamesave` finalize events, §3.2 Source 3), and `pfinternallogs.kusto.windows.net`/`GameplayServices`/`GameSaveLog` (service auto-rollback log, §3.2 Source 4) | Analyzing patterns across users/titles, failure rates, historical data, cloud-side version chain / data-loss detection, and the exact auto-rolled-back population |
| **PGS (On-Disk State)** | GRTS local state: extended manifests, blob cache, sync status, version metadata | `C:\XboxGames\GameSave\pgs\` | Examining current device state, verifying sync completed, diagnosing stale/orphaned data |
| **Inproc SDK Debug Logs** | SDK-side function calls, internal state transitions, GRTS↔SDK interaction, async operation lifecycle | `C:\git\PlayFab.C\Out\gamesave-pc-tests\pass{N}\{test}\device-*.txt` | Tracing exact SDK call sequences, parameter values, timing at the API boundary, correlating SDK actions with server-side events |

**Key insight:** ETL tells you *what GRTS did* (HTTP calls, state transitions, upload chunks). Kusto tells you *what the SDK reported* (activation success/failure, sync stats, error codes). PGS tells you *what the device believes right now* (which version it has, which files are cached, sync status). Inproc logs tell you *what the SDK did step-by-step* (function entry/exit, parameter values, async lifecycle, internal provider state). You need all four to fully understand a game save flow.

---

## 2. ETL Traces — How to Read Them

### 2.1 ETL Architecture

The GamingServices auto-logger session captures events from 22+ ETW providers into ETL files. The ETL files have **no embedded manifests** — the schema is decoded by Windows TDH using system-registered providers.

| Property | Value |
|----------|-------|
| Session name | `GamingServices` |
| ETL location | `C:\Windows\System32\LogFiles\WMI\GamingServices.etl` |
| Key provider | `Microsoft.Gaming.PlayFab.GameSaveTrace` (`{bd401bba-fe3f-4e6b-85bf-4f5c8f52137b}`) |
| Trace level | 5 (Verbose) |

### 2.2 Reading ETL Files

**Preferred tool:** `grts-read-etl.py` in `C:\git\PlayFab.C\Utilities\Scripts\`

```powershell
# Recent events (most common usage):
py Utilities\Scripts\grts-read-etl.py --last-minutes 10

# Write to file:
py Utilities\Scripts\grts-read-etl.py -o trace.txt

# JSON output for programmatic analysis:
py Utilities\Scripts\grts-read-etl.py --json -o trace.json

# Filter to game save provider only:
py Utilities\Scripts\grts-read-etl.py --provider "Microsoft.Gaming.PlayFab.GameSaveTrace"

# Skip flushing (if already flushed):
py Utilities\Scripts\grts-read-etl.py --no-flush
```

**Manual fallback** (less useful — no structured field names):
```powershell
logman update "GamingServices" -ets -fd          # flush buffers
tracerpt C:\Windows\System32\LogFiles\WMI\GamingServices.etl -of XML -lr -o trace.xml
```

**Does NOT require admin** for ETL flush — `logman update -ets -fd` works as standard user (verified experimentally in pass156–159). Use `--no-flush` if already flushed.

> **⚠️ Important:** The simpler `logman flush GamingServices -ets` command is **broken** on current Windows versions (error `-2147024809`). Always use `logman update GamingServices -ets -fd` which flushes both ETW buffers AND OS file buffers.

### 2.3 Key ETL Providers

| Provider | Typical Events | Use For |
|----------|---------------|---------|
| `Microsoft.Gaming.PlayFab.GameSaveTrace` | ~35 per test | **Core game save**: upload, download, context, HTTP responses |
| `Microsoft.Gaming.GameFlt` | ~40 | **Process lifecycle**: game start/exit, PID tracking |
| `Microsoft.Xbox.XAL.GRTS-All` | ~8500 | XAL authentication (very verbose) |
| `Microsoft.Gaming.GRTS` | ~4 | GRTS service lifecycle |
| `Microsoft.Gaming.PlayFab.GameSave` | ~3 | PlayFab GameSave telemetry (SDK-side) |

**Focus on:** `GameSaveTrace` and `GameFlt` for debugging game save issues.

### 2.4 Key ETL Events

| Event Name | Level | What It Tells You |
|------------|-------|-------------------|
| `PFXGameSaveServicePrepareContextWithConfig` | VERB | Game initializing (TitleId, EntityId, flags) |
| `NewPlayFabCaller` | INFO | GRTS registered the game process (PID, AUMID) |
| `PFResponse` | INFO | HTTP response from PlayFab (RequestType, HttpStatus, ElapsedMs) |
| `PFContextCreated` | INFO | Context fully activated (download complete) |
| `PFUploadWorkerEntered` | INFO | Background upload starting |
| `PFUploadPlanComplete` | VERB | Upload plan ready (UploadPendingSize, UploadChunks) |
| `PFUploadContextUploadChunk` | INFO | Chunk uploaded (uploadUri, uploadSize, hr) |
| `PFUploadContextFinalized` | INFO | Manifest finalized on server — **upload succeeded** |
| `PFUploadContextComplete` | INFO | Upload lifecycle complete (FileCount, TotalSize, ElapsedMs) |
| `PFUploadContextUploadError` | ERROR | Upload failed (state, ElapsedMs) |
| `PFUploadContextAbandoned` | INFO | Upload abandoned — game exited before upload finished. Check `LastError` for root cause |
| `GameProcessStateChange` | INFO | Process state changed (PID, IsGameProcessListEmpty) |
| `PFActivatorTakeRemoteLoserUploadDisabled` | INFO | Conflict resolved: took remote, loser upload disabled |
| `PFContextInitFinish` | INFO | Activation sync complete (newVersion, chunkCount, downloadSize, elapsedMs, hr) |
| `PFContextOnPackageTerminated` | INFO | Game process terminated — triggers post-termination upload |
| `PFIntegrityFile` | VERB | Per-file integrity record (Checkpoint, Version, RelativePath, CRC32, FileSize) |
| `PFIntegritySnapshot` | INFO | Aggregate integrity snapshot (Checkpoint, Version, FileCount, TotalBytes) |
| `PFCopyFilesResult` | INFO | Bulk file copy result (operation, copyCount, copyFailed, totalBytesCopied) |
| `PFUploadExtractXvdData` | INFO | XVD extraction stats (FileCount, ExtractTimeMs, FilesCopied) — Xbox only |
| `PFUploadContextGenerateExtManifest` | INFO | Extended manifest generated for upload (version, path) |
| `PFContextStorageLow` | WARN | **Storage check failed** — FreeSize, TotalSize, RequiredSize. If RequiredSize is near-max uint64 (>2^63), this is the known arithmetic underflow bug (see §2.8) |
| `PFContextInitFail` | ERROR | Activation failed (state, Hresult, ResponseCount, SyncChunks, syncDuration) |
| `PFActivatorCanceled` | INFO | Activator canceled (state, blockingReason). `NeedStorage` = storage check failed |

### 2.5 ETL Event Flow — Golden Path

For a single-device golden path test (test 01), the expected ETL event sequence is:

```text
1. GameProcessStateChange          — game process detected
2. PFXGameSaveServicePrepareContext — SDK calls GRTS to init
3. NewPlayFabCaller                — GRTS registers caller
4. PFResponse (PFR_LOGIN)          — PlayFab login
5. PFResponse (PFR_LIST_MANIFESTS) — list available versions
6. PFResponse (PFR_GET_MANIFEST)   — get manifest details (if existing data)
7. PFContextCreated                — context ready (activation complete)
   [game writes save data]
8. PFUploadWorkerEntered           — upload triggered
9. PFUploadPlanComplete            — diff plan created
10. PFResponse (PFR_INIT_MANIFEST)  — allocate new version
11. PFResponse (PFR_INIT_UPLOAD)    — get upload URIs
12. PFUploadContextUploadChunk      — data uploaded
13. PFResponse (PFR_FINALIZE_MANIFEST) — commit version
14. PFUploadContextFinalized        — upload confirmed
15. PFUploadContextComplete         — lifecycle done
16. GameProcessStateChange          — game exited
```

### 2.6 Common PFResponse RequestTypes

| RequestType | PlayFab API |
|-------------|-------------|
| `PFR_LOGIN` | Entity login |
| `PFR_LIST_MANIFESTS` | List game save manifests |
| `PFR_INIT_UPLOAD` | Start upload session |
| `PFR_PUSH_UPLOAD` | Upload blob chunk |
| `PFR_FINALIZE_MANIFEST` | Commit upload |
| `PFR_DOWNLOAD_BLOBS` | Download save blobs |
| `PFR_GET_MANIFEST_DETAILS` | Get specific version metadata |
| `PFR_INIT_MANIFEST` | Create new version |
| `PFR_DELETE_MANIFEST` | Remove a version |

### 2.7 Filtering ETL for GameSave Debugging

The raw ETL output contains hundreds of thousands of events from all providers (networking, WiFi, auth, etc.). For GameSave debugging, filter to these event names which cover the full save lifecycle plus errors:

#### GameSave Core Events

| Event Name | Level | What It Tells You |
|------------|-------|-------------------|
| `PFXGameSaveInitializeConfig` | VERB | SDK initialized — titleId, apiUrl, flags |
| `PFXGameSaveFilesGetFolderWithUiAsync` | VERB | AddUser started — configHandle, async handle |
| `PFXGameSaveServicePrepareContext` | VERB | GRTS preparing context — titleId, userId, operationId |
| `PFXGameSaveServiceNoAumid` | ERROR | GRTS couldn't resolve AUMID — game init may have failed |
| `PFXGameSaveServiceAddAumid` | INFO | GRTS resolved AUMID for the game process |
| `NewPlayFabCaller` | INFO | GRTS registered the game process — CallerPid, Aumid, PackageFullName |
| `GspPlayFabGetFolderWithUiAsync_Start` | VERB | GetFolder operation started — OperationId |
| `GspPlayFabGetFolderWithUiAsync_QueueAsync` | VERB | GetFolder queued for async execution |
| `XGameSaveOp_InitializePlayFabProvider` | VERB | PlayFab provider being initialized |

#### Sync & Upload Events

| Event Name | Level | What It Tells You |
|------------|-------|-------------------|
| `PFContextSyncPlan` | VERB | Download plan — targetManifest, downloadChunks, downloadSize |
| `PFContextCheckSyncConflicts` | VERB | Conflict check — targetManifest, conflictVersion, prevVersion |
| `PFContextInitManifestRequest` | INFO | New version requested — newVersion, baseVersion, latestRemote, manifestCount |
| `PFContextInitStart` | INFO | Context init started — conflictResolution, targetManifest, oldVersion |
| `PFContextInitFinish` | INFO | Context init complete — oldVersion, newVersion, syncVersion, chunkCount |
| `PFContextMoveFromPrevious` | INFO | Blob cache moved to new version directory |
| `PFContextCreated` | INFO | Context fully activated — Version, ContextPath |
| `PFContextGetQuota` | VERB | Quota query — TotalQuota, CurrentSize, AvailableQuota |
| `PFContextSameDeviceLocalDataCheck` | INFO | Checking if local data exists for a version |
| `PFContextSameDeviceMissingLocalData` | WARN | Local data missing for a version — will re-download |
| `PFContextStaleInitFallbackToFinalized` | INFO | Fell back to last finalized version |
| `PFContextProgress` | VERB | Sync progress — State, Current, Total, Percent |
| `PFUploadWorkerEntered` | INFO | Background upload starting |
| `PFUploadPlanStart` | VERB | Upload diff plan — ModifiedFiles, AddedFiles, MissingFiles |
| `PFUploadPlanComplete` | VERB | Upload plan ready — UploadPendingSize, UploadChunks, ActiveFiles |
| `PFUploadContextUploadChunk` | INFO | Chunk uploaded — chunkId, uploadUri, httpStatus |
| `PFUploadContextChunksComplete` | VERB | All chunks uploaded — uploadSize |
| `PFUploadContextFinalized` | INFO | Manifest committed — FileCount, TotalSize, ElapsedMs |
| `PFUploadContextComplete` | INFO | Upload lifecycle done — FileCount, TotalSize, ElapsedMs |

#### Error & Warning Events

| Event Name | Level | What It Tells You |
|------------|-------|-------------------|
| `Error` | ERROR | Generic error — hr, file, lineNumber, failureCount |
| `Warning` | WARN | Non-fatal warning — hr, file, lineNumber, message |
| `CheckErrorFail` | ERROR | Assertion-style check failed — hr, info (describes what failed) |
| `InternalError` | VERB | Internal error logged — ErrorText, Hresult |
| `ResultLoggingCallback` | ERROR | Error propagated through callback chain — hr, file, lineNumber |

#### Auth Events (relevant to GameSave)

| Event Name | Level | What It Tells You |
|------------|-------|-------------------|
| `PcGdkAuthStack` | VERB | Auth stack type — "Undocked" for PC, titleId, msaAppId |
| `InitializeApiImplSingletons` | VERB | Auth API initialized — serviceGuid |
| `GamePlatformPackageService_GetProcessActivationId` | VERB | Resolving game package identity |

#### Async Operation & UI Events

| Event Name | Level | What It Tells You |
|------------|-------|-------------------|
| `XGameSaveOp_InitializePlayFabProvider` | VERB | PlayFab provider init — OperationId |
| `XGameSaveOp_WaitBegin` | VERB | Async operation waiting for completion |
| `XGameSaveOp_OnComplete` | VERB | Async operation completed — Hresult |
| `XGameSaveOp_WaitComplete` | VERB | Wait finished |
| `XGameSaveOp_PlayFabGetQuota` | VERB | Quota query operation |
| `XGameSaveOp_PlayFabSetSaveDescription` | VERB | Description set operation |
| `ContextSync` | INFO | Sync telemetry event emitted |
| `ContextActivation` | INFO | Activation telemetry event emitted |
| `UIProviderQueuedProgress` | INFO | UI progress callback queued |
| `UIProviderCompleteWithoutRequest` | INFO | UI completed without game responding — may indicate callback not registered |
| `GameFltGameProcessCreated` | INFO | Game process started — PID tracking |
| `GameFltGameProcessDestroyed` | INFO | Game process exited |
| `GameFltIsActive` | INFO | Game process active state check |
| `UserSessionAdded` | INFO | User session registered |

#### PowerShell: Extract GameSave Events from ETL

To extract just the GameSave-relevant events from a raw ETL output file:

```powershell
# Step 1: Read the full ETL to a text file
py Utilities\Scripts\read-grts-etl.py "path\to\GamingServices.etl" --no-flush -o etl-full.txt

# Step 2: Filter to GameSave events (adjust time window as needed)
$lines = Get-Content etl-full.txt
$keepPatterns = 'PFX|PFResponse|PFContext|PFUpload|PFActivator|NewPlayFabCaller|' +
    'Warning|Error|CheckErrorFail|InternalError|ResultLoggingCallback|' +
    'Gsp|PcGdkAuthStack|InitializeApiImpl|GamePlatformPackageService_GetProcessActivation|' +
    'XGameSaveOp_|ContextSync|ContextActivation|UIProvider|GameFlt|UserSessionAdded'

$output = @(); $include = $false
foreach ($line in $lines) {
    if ($line -match '^\d{2}:\d{2}:\d{2}\.\d{3}') {
        $include = ($line -match $keepPatterns)
        if ($include) { $output += $line }
    } elseif ($include -and $line -match '^\s{4,}\S') {
        $output += $line
    } else { $include = $false }
}
$output | Set-Content etl-gamesave-filtered.txt
```

#### Key Error Codes Seen in ETL

| HRESULT | Meaning | Where |
|---------|---------|-------|
| `0x89245102` | XAL user not found / auth failure | `userapiimpl.cpp` — user token lookup failed |
| `0x8083000c` | `CS_E_USER_MUST_BE_XBOX_USER` | `undockedusermanager.cpp` — GRTS requires Xbox user token |
| `0x80832300` | GRTS context preparation failed | `pfprovider.cpp` — cascades from user auth failure |
| `0x80070002` | `ERROR_FILE_NOT_FOUND` | `xaluserserver.cpp` — auth token file not found |

#### What to Look For

1. **Multiple `NewPlayFabCaller` without `PFContextCreated`** — Game process registered but never completed init. Look for errors between the two events.
2. **`CS_E_USER_MUST_BE_XBOX_USER`** — The game called PFGameSaveFiles without a valid Xbox user token. Common when using entity-auth-only path on older GRTS builds.
3. **`PFContextSameDeviceMissingLocalData`** — GRTS created a new version but the local blob data is missing. Falls back to re-downloading. Can burn version numbers.
4. **`PFXGameSaveServiceNoAumid`** — GRTS couldn't resolve the game's AUMID. Usually followed by errors. Check if the game's package is properly registered.
5. **Error cascade pattern** — A single root error (e.g., `0x89245102` in auth) cascades through `CheckErrorFail` → `ResultLoggingCallback` at multiple call sites. The **first** error in the chain is the root cause.

### 2.8 Known Bug Pattern: RequiredSize Underflow (0x80830003)

When `PFGameSaveFilesAddUserWithUiAsync` returns `0x80830003` (CS_E_OUT_OF_LOCAL_STORAGE), check ETL for this event sequence:

```
PFContextStorageLow
  FreeSize: <actual free bytes — typically plenty>
  TotalSize: <total disk capacity>
  RequiredSize: <HUGE number, near 2^64>    ← ARITHMETIC UNDERFLOW

PFActivatorCanceled
  state: StorageChecks
  blockingReason: NeedStorage

PFContextInitFail
  state: StorageError
```

**Root cause:** GRTS has a signed/unsigned arithmetic bug when computing required storage during the `PFAS_StorageChecks` activation phase. The computation underflows to near-max uint64 (~18 exabytes), causing the free space check to always fail — regardless of actual disk space.

**Trigger conditions:**
- Cloud save with multiple chunks (accumulated over several upload sessions)
- Observed threshold: ~5+ chunks
- Does NOT depend on actual save size (5 MB save triggers it)

**Confirmation:** If `RequiredSize > 2^63` (i.e., > 9,223,372,036,854,775,808), it's this bug — not an actual storage shortage.

**Workaround:** Repack the cloud save (collapse all chunks into 1). After repack, the save activates successfully.

**Related:** Bug 4405319 (Horizon 6 Steam data loss) — same GRTS code path. When this storage check fails repeatedly, GRTS may eventually perform a destructive reset of the local save folder, causing data loss on the next AddUser.

---

## 3. Kusto Telemetry — How to Query

### 3.1 Five Clusters

| Cluster | Database | Primary Table | What It Has |
|---------|----------|---------------|-------------|
| `gaming.westus.kusto.windows.net` | Gaming | UTCEvents | Client-side telemetry (Xbox/PC) — `ContextActivation`, `ContextSync`, etc. |
| `playfabinternalreader.westus2.kusto.windows.net` | PlayFabInternal | `['events.all']` | PlayFab server-side events — entity-based telemetry from SDK (`playfab.gamesave.internal`) |
| `pmain01sharded004.westus2.kusto.windows.net` | `ShardedDB023` | `['events.all']` | **PlayFab production telemetry shard** — server-authoritative GameSave version-lifecycle events (`playfab.gamesave`). See §3.2 Source 3. |
| `pfinternallogs.kusto.windows.net` | `GameplayServices` | `GameSaveLog` | **GS.GameSave service structured logs** — suspected-data-loss detection, **auto-rollback decisions**, finalize/conflict diagnostics. See §3.2 Source 4. |
| `gamingbi.westus.kusto.windows.net` | XDM | Various | Title metadata, Watson crashes, error code lookups. **⚠️ See §3.8 — `gamingbi` is reported DEAD by the fleet-analysis team; Watson + XDM now live on the `gaming` cluster as `database("Watson")` / `database("XDM")`.** |

> **⚠️ Sharded production cluster:** `pmain01sharded004 / ShardedDB023` is one shard of PlayFab's production telemetry. Players are distributed across many `pmain*sharded*` clusters/DBs, so a single shard holds only a **subset** of titles/entities. For Forza titles (e.g. Forte `16D460`) this shard carries the data; for other titles you may need a different shard. The `playfabinternalreader` cluster is the curated reader/follower over these production shards — query the shard directly for the freshest server-side version events.

> **⚠️ Sharded production cluster:** `pmain01sharded004 / ShardedDB023` is one shard of PlayFab's production telemetry. Players are distributed across many `pmain*sharded*` clusters/DBs, so a single shard holds only a **subset** of titles/entities. For Forza titles (e.g. Forte `16D460`) this shard carries the data; for other titles you may need a different shard. The `playfabinternalreader` cluster is the curated reader/follower over these production shards — query the shard directly for the freshest server-side version events.

### 3.2 Two Telemetry Sources — Why You Need Both

PFGameSave telemetry is split across **two independent sources** that capture different parts of the save pipeline. Cross-platform investigations **require both**.

#### Source 1: SDK Telemetry (PlayFabInternal Cluster)

| Property | Value |
|----------|-------|
| Cluster | `playfabinternalreader.westus2.kusto.windows.net` |
| Database | `PlayFabInternal` |
| Table | `['events.all']` (bracket notation required) |
| Namespace | `playfab.gamesave.internal` |
| Covers | **All platforms** — Steam Deck (inproc), Xbox via GRTS relay (Windows DeviceType), PC |
| Identity | `Entity_Id` (PlayFab entity), `EntityLineage_title` (title ID) |

**What SDK telemetry has that GRTS doesn't:**
- Version finalization (`gamesave_version_finalized`) — version number, total size, file count, device type
- Known-good marking (`gamesave_version_marked_known_good`) — which versions are blessed
- Local cache deletes (`context_delete`) — delete type, hresult
- Per-event `platformType` field (`Windows`, `SteamPc`, `SteamDeck`, `Xbox`, `WindowsInproc`)

**What SDK telemetry is MISSING:**
- Conflict resolution details — `conflictResolution` field on `context_activation` is **always None/empty**
- `isWinner` / `loserVersion` — **not present in any SDK event**
- GRTS sync internals (download sizes, upload chunk details)

**Schema quirk:** Event payload is nested under `EventData.Payload.*`, not `EventData.*` directly. Use `tostring(EventData.Payload.totalSizeBytes)` etc.

**Searching Source 1 by XUID (the inproc/Steam Deck source):** the SDK events carry
`EventData.Payload.userId` in the form **`XUser_<XUID>`** (verified on live Forte data — all Forte
users are `XUser_` prefixed; Xbox identity even when signed in on a Steam Deck). So to check **"did
this account use inproc/Steam Deck?"** search here by `userId`:
```kql
['events.all']
| where Timestamp between (datetime(2026-05-20) .. datetime(2026-05-28))   // explicit window — see retention
| where FullName_Namespace startswith 'playfab.gamesave'
| where tostring(EventData.Payload.userId) in ('XUser_2814626826459508','XUser_2814626752567349')
| summarize cnt=count(), plats=make_set(tostring(EventData.Payload.platformType),10),
            titles=make_set(EntityLineage_title,10), ents=make_set(EntityLineage_title_player_account,5)
        by userId=tostring(EventData.Payload.userId)
```
> **⚠️ A 0-row result here does NOT prove "not Steam Deck."** On Bug 4401522 both repro XUIDs returned
> **0 rows across Source 1 (incl. the retained 25-May repro week with 1.3M Forte events), Source 2, and
> Source 3** — i.e. the accounts emit **no findable GameSave telemetry under those XUIDs at all**
> (likely cert/flight/private-sandbox accounts off the retail stream). When that happens the device
> engine is **unverifiable from telemetry**; get the PlayFab **entity id** from Game Manager (admin XUID
> lookup) and retry by entity, but it may still be empty if the account is off the retail stream.

#### Source 2: GRTS Telemetry (Gaming Cluster — UTCEvents)

| Property | Value |
|----------|-------|
| Cluster | `gaming.westus.kusto.windows.net` |
| Database | `Gaming` |
| Table | `UTCEvents` |
| Event names | `Microsoft.Gaming.PlayFab.GameSave.ContextActivation`, `.ContextSync`, etc. |
| Covers | **Xbox/GRTS path only** — Steam Deck emits ZERO UTCEvents |
| Identity | `xbl_xid` (XUID with `x:` prefix) |

**What GRTS telemetry has that SDK doesn't:**
- Conflict resolution — `data.conflictResolution` (0=NoConflict, 1=KeepLocal, 2=TakeRemote, 3=SelectVersion)
- `data.conflictVersion`, `data.loserVersion`, `data.baseVersion`
- `data.wasContention` (bool)
- Sync download/upload details (`data.syncSizeBytes`, `data.fileCount`, `data.elapsedMs`)

**What GRTS telemetry is MISSING:**
- Version finalization events (no equivalent of `gamesave_version_finalized`)
- Known-good marking (no equivalent of `gamesave_version_marked_known_good`)
- Any Steam Deck / inproc-path data

**Schema quirk:** The `data` bag uses **camelCase** field names (`data.conflictResolution`, `data.loserVersion`). This differs from the materialized view `XboxConnectedStorageActivations` which uses **PascalCase** (`ConflictResolution`, `TotalBytes`). Using the wrong case returns empty results with no error.

#### Source 3: Server-Side Version Lifecycle (PlayFab Production Shard — `ShardedDB`)

> **Discovered 2026-06-22.** This is the **server-authoritative** record of every save version the GameSave service committed to the cloud. Emitted by the service itself (`Originator: playfab/service`, `Entity.Type: title_player_account`), NOT by the client SDK or GRTS. It is the single best source for proving/disproving cloud-side data loss (REQ#789 empty-folder wipe, save stomps, rollbacks) because it captures the **authoritative version chain** independent of any device.

| Property | Value |
|----------|-------|
| Cluster | `pmain01sharded004.westus2.kusto.windows.net` (a production shard; others exist) |
| Database | `ShardedDB023` |
| Table | `['events.all']` (bracket notation required) |
| Namespace | `playfab.gamesave` (NOTE: **not** `playfab.gamesave.internal` — that's Source 1) |
| Identity | `EntityLineage_title` (hex title ID, e.g. `16D460` = Forte), `Entity_Id` (title_player_account hex) |
| Covers | All platforms that finalize to PlayFab cloud; `DeviceType` payload field reports `PC`, `Xbox`, `Xbox Series`, etc. |
| Retention | ≥30 days verified on this shard |

**Two events only** (both extremely high volume — Forte `16D460` alone emits ~3.5M `gamesave_version_finalized` / 24h):

| Event | When | Key Payload Fields |
|-------|------|--------------------|
| `gamesave_version_finalized` | Service commits a new version to cloud | `Version`, `BaseVersion`, `TotalSizeBytes`, `TotalFileCount`, `NewFileCount`, `NewFilesSizeBytes`, `FinalizedAt`, `DeviceType`, `PlayerIdentityProvider`, `IsGeneratedByRollback`, **`RollbackReason`** (on rollback finalizes — added ~2026-06-22), `Description` (base64; decode with `base64_decode_tostring()`) |
| `gamesave_version_marked_known_good` | Service blesses a version as known-good | `Version`, `MarkedAt` |

**Why this is gold for data-loss investigations:**
- **Authoritative version chain** — `Version` is the server's monotonic version; `BaseVersion` is its parent. A healthy chain is `BaseVersion == previous Version` (e.g. 49→50). A **`BaseVersion` that jumps back to 0 (or far below the latest) over an existing high version is the REQ#789 cloud wipe**, recorded server-side.
- **Size/file regression = visible data loss** — `TotalSizeBytes` / `TotalFileCount` dropping between consecutive versions is direct evidence of a stomp/rollback.
- **`IsGeneratedByRollback`** — flags versions the service produced via rollback (correlate with rollback bugs / §13).
- **`Description` decodes to human-readable save metadata** — it is base64 of the game's save description blob. For Forza it decodes to live progress, e.g.:
  ```text
  Cars In Garage: 81
  Discover Japan Progress: 3,255
  Horizon Festival Progress: 25,020
  XP: 1,308,246
  Credits: 31,822,337
  ```
  A regression in these numbers across versions is plain-text proof of a save rollback the player would notice.
- **Device attribution** — `DeviceType` tells you which platform finalized each version, so a PC-finalized version stomping an Xbox-finalized one is visible without correlating other sources.

**Relationship to Source 1 (PlayFabInternal):** Source 1 documents `gamesave_version_finalized` / `gamesave_version_marked_known_good` under the `playfab.gamesave.internal` namespace via the `playfabinternalreader` follower. This shard carries the same lifecycle events at their production origin under the `playfab.gamesave` namespace, server-originated and typically fresher. When the reader is lagging or curated, query the shard directly.

**How to query (no Kusto CLI required):** there is no Kusto CLI in the repo toolchain. Query over the REST API with an AAD token from the Azure CLI (`az login` as your `@microsoft.com` account first):

```powershell
$tok = az account get-access-token `
  --resource "https://pmain01sharded004.westus2.kusto.windows.net" `
  --query accessToken -o tsv
$headers = @{ Authorization = "Bearer $tok"; "Content-Type" = "application/json" }
$kql = @"
['events.all']
| where Timestamp > ago(24h)
| where FullName_Namespace == 'playfab.gamesave' and FullName_Name == 'gamesave_version_finalized'
| where EntityLineage_title == '16D460'          // Forte
| where Entity_Id == 'YOUR_ENTITY_ID'
| extend P = EventData.Payload
| project Timestamp,
    Version=toint(P.Version), BaseVersion=toint(P.BaseVersion),
    TotalSizeBytes=tolong(P.TotalSizeBytes), Files=toint(P.TotalFileCount),
    NewFiles=toint(P.NewFileCount), DeviceType=tostring(P.DeviceType),
    Rollback=tostring(P.IsGeneratedByRollback)
| order by Version asc
"@
$body = @{ db = "ShardedDB023"; csl = $kql } | ConvertTo-Json -Depth 5
$r = Invoke-RestMethod -Method Post `
  -Uri "https://pmain01sharded004.westus2.kusto.windows.net/v2/rest/query" `
  -Headers $headers -Body $body -TimeoutSec 100
($r | Where-Object TableKind -eq 'PrimaryResult').Rows
```

**Data-loss detector queries:**

```kql
// 1) BaseVersion regression / cloud wipe: base jumps backward over an existing high version
['events.all']
| where Timestamp > ago(7d)
| where FullName_Namespace == 'playfab.gamesave' and FullName_Name == 'gamesave_version_finalized'
| where EntityLineage_title == '16D460'
| extend P = EventData.Payload
| project Entity_Id, Timestamp, Version=toint(P.Version), BaseVersion=toint(P.BaseVersion),
    Bytes=tolong(P.TotalSizeBytes), Files=toint(P.TotalFileCount), DeviceType=tostring(P.DeviceType)
| order by Entity_Id asc, Version asc
| extend prevVersion = prev(Version), prevEntity = prev(Entity_Id)
| where Entity_Id == prevEntity and BaseVersion < prevVersion - 1   // gap or backward base = suspect
```

```kql
// 2) Size/file-count regression between consecutive finalized versions (potential stomp)
['events.all']
| where Timestamp > ago(7d)
| where FullName_Namespace == 'playfab.gamesave' and FullName_Name == 'gamesave_version_finalized'
| where EntityLineage_title == '16D460'
| extend P = EventData.Payload
| project Entity_Id, Version=toint(P.Version), Bytes=tolong(P.TotalSizeBytes), Files=toint(P.TotalFileCount)
| order by Entity_Id asc, Version asc
| extend prevBytes = prev(Bytes), prevFiles = prev(Files), prevEntity = prev(Entity_Id)
| where Entity_Id == prevEntity and (Bytes < prevBytes or Files < prevFiles)
```

```kql
// 3) Title-wide discovery: which titles/entities are on THIS shard
['events.all']
| where Timestamp > ago(24h)
| where FullName_Namespace == 'playfab.gamesave' and FullName_Name == 'gamesave_version_finalized'
| summarize Finalizations=count(), Entities=dcount(Entity_Id) by EntityLineage_title
| order by Finalizations desc
```

**Schema quirks (same family as Source 1):**
- Table requires bracket notation: `['events.all']`.
- Payload nested under `EventData.Payload.*` — use `toint(EventData.Payload.Version)`, `tolong(EventData.Payload.TotalSizeBytes)`, etc.
- Namespace is `playfab.gamesave` (no `.internal` suffix) and `FullName_Name` is the event name.
- Shard scoping: this shard holds a subset of players; a missing entity may live on a different `pmain*sharded*` cluster.

#### Source 4: Service Auto-Rollback & Data-Loss Log (`pfinternallogs` → `GameSaveLog`)

> **Discovered 2026-06-22.** These are the **GS.GameSave service's own structured logs** (Geneva) — the server's internal view of save finalization, its **suspected-data-loss detection heuristics**, and the **auto-rollback decisions** it makes. This is where you find the *exact population of users the service auto-rolled back* and *why*. Distinct from Source 3 (which is the clean per-version event stream); this is the service's operational diagnostic log with one row per log point hit.

| Property | Value |
|----------|-------|
| Cluster | `pfinternallogs.kusto.windows.net` |
| Database | `GameplayServices` |
| Table | `GameSaveLog` |
| Log-point selector | `env_name` (the service code's log scope, e.g. `LogSuspectedDataLossOnFinalize`) |
| Identity | `entityKey` (e.g. `title_player_account!<HEX>`), `playerId`, `titleId` (hex, e.g. `16D460` = Forte) |
| Time | `TIMESTAMP` and `env_time` (use `TIMESTAMP` for range filters) |
| Errors | `severityText`, `env_ex_type`, `env_ex_msg`, `env_ex_stack` |

**The query that returns the auto-rolled-back users** (the ~1,300 users the service rolled back; 784 distinct entities / 1,335 rows in 3d — 100% Forte `16D460`):

```kql
GameSaveLog
| where TIMESTAMP > ago(3d)
| where env_name == 'LogSuspectedDataLossOnFinalize'
| where enableExtendedManifestAutoRollback == true
| project entityKey, RollbackTime = env_time, CorruptVersion = version, RollbackVersion = previousVersion
```

**Data-loss / auto-rollback log-point family** (3d volumes, dominated by Forte `16D460`):

| `env_name` | 3d Volume | Meaning | Useful Fields |
|-----------|-----------|---------|---------------|
| `LogReportedSizeRegression` | ~3.0M | Finalize's reported size is **smaller** than a prior version. **Very noisy** — fires on any shrink; most are benign (0 `wentToZero`, 0 ≥90% drops in 3d). Detection candidate, NOT a conclusion. | `playerId`, `version`, `delta`, `wentToZero`, `latestManifestVersion`, `largestManifestVersion`, `largestSizeBytes` |
| `LogMissingManifestDescriptionOnFinalize` | ~450K | Finalize arrived without a manifest description. | `entityKey`, `version`, `isMissingManifestDescription` |
| `InConflictWithBaseDuringFinalizeClientBug` | ~9K | Finalize's base version conflicts with server state — **attributed to a client bug** (GRTS/SDK sent a stale/zero base). Root-cause-adjacent for the wipe. | `entityKey`, `version`, `baseVersion`, `conflictVersion` |
| `LogSuspectedDataLossOnFinalize` | 1,335 (784 users) | Service flagged a finalize as **suspected data loss**. This is the gate for the rollback query above. | `version`, `previousVersion`, `enableExtendedManifestAutoRollback`, `enableExtendedManifestCheckWhatIf` |
| `LogDataLossProtectionAutoRollbackChoice` | 731 | The **rollback decision** — service chose which save to keep. | `autoRollbackVersion`, **`selectedSave` (`Cloud`/`Local`)**, `version` |
| `LogOriginFileMapNotFoundInRollback` | 984 | **Rollback FAILURE mode** — the rollback couldn't find the origin file map. Watch these: a failed rollback may not have restored data. | `entityKey`, `version` |

**Pipeline order:** `LogReportedSizeRegression` (size shrank) → `LogSuspectedDataLossOnFinalize` (flagged) → `LogDataLossProtectionAutoRollbackChoice` (decided, `selectedSave` Cloud/Local) → version reverted (`CorruptVersion → RollbackVersion`, typically N→N-1). `InConflictWithBaseDuringFinalizeClientBug` and `LogMissingManifestDescriptionOnFinalize` are corroborating root-cause signals; `LogOriginFileMapNotFoundInRollback` flags rollbacks that may have failed.

**Feature flags** seen on all current Forte rows: `enableExtendedManifestAutoRollback == true` AND `enableExtendedManifestCheckWhatIf == true`. Use these to separate **shadow/what-if detection** (logged but not acted on) from **enforced auto-rollback**.

**Schema notes / gotchas:**
- `GameSaveLog` is a union of every service log point, so it has **200+ columns**; most are null for any given `env_name`. Always filter `env_name` first, then project only that event's populated fields.
- Identity field varies by log point: `LogReportedSizeRegression` populates `playerId` (not `entityKey`); the rollback/suspected-loss events populate `entityKey`. Use `coalesce(entityKey, strcat('title_player_account!', playerId))` when joining across log points.
- The rich size columns (`sizeDropPercent`, `previousSizeBytes`, `newSizeBytes`) **exist in the schema but were observed sparsely/empty** on sampled events in this window — don't assume they're populated; prefer `delta` + the `latest` vs `largest` manifest-version/size comparison on `LogReportedSizeRegression`.
- Rows can duplicate (same entity/version logged twice) — `summarize dcount(entityKey)` for user counts, not raw `count()`.

**Example — full rollback decision detail for a title:**
```kql
GameSaveLog
| where TIMESTAMP > ago(3d)
| where titleId == "16D460"
| where env_name == "LogDataLossProtectionAutoRollbackChoice"
| project env_time, entityKey, CorruptVersion = version, RollbackTo = autoRollbackVersion,
    KeptSave = selectedSave
| order by env_time desc
```

**Example — cross-reference an auto-rolled-back user with the authoritative version chain (Source 3):**
1. Get `entityKey` + `CorruptVersion` from `LogSuspectedDataLossOnFinalize` here.
2. Strip the `title_player_account!` prefix to get the bare hex `Entity_Id`.
3. Query Source 3 (`['events.all']` / `playfab.gamesave`) on that `Entity_Id` for the `gamesave_version_finalized` history and decode the `Description` to see the actual XP/Credits at the corrupt vs rolled-back version (§3.2 Source 3).

#### XUID ↔ EntityId Conversion (Xbox ↔ PlayFab identity)

Most authoritative GameSave sources key on the PlayFab **EntityId** (`title_player_account` 16-hex),
but bug reports usually give you an Xbox **XUID** (bare decimal). There is **no arithmetic transform**
— you "convert" by reading a row that carries **both** columns. Two sources do:

| Source | XUID column | EntityId column | Scope |
|--------|-------------|-----------------|-------|
| Shard `['events.all']` (`pmain01sharded004.westus2` / `ShardedDB023`) | `EventData.Payload.Standard.UserId` (bare decimal) | `EntityLineage_title_player_account` (16-hex; UPPER) | FH6 (`EntityLineage_title == '16D460'`) |
| Gaming `UTCEvents` PlayFab tasks (`gaming.westus` / `Gaming`) | `xbl_xid` (may carry `x:` prefix) | `tostring(data.entityId)` | any title |

**Gotchas:**
- **Upper-case the EntityId** — conventionally uppercased for cross-cluster joins.
- **Strip the `x:` prefix** on Gaming `xbl_xid` (`replace_string(xbl_xid,'x:','')`) to match the shard's bare-decimal XUID.
- **One EntityId per XUID per title** — scope by title for a 1:1 answer.
- **`data.entityId` is sparse in `UTCEvents`** — only set on rows where the PlayFab task logged it (GameSave task events frequently have **empty `xbl_xid`**, so XUID→entity via UTCEvents is best-effort).
- **⚠️ Retention is the catch (learned on Bug 4401522):** the shard `['events.all']` retains only **~30 days**, so an account that last played >30d ago **cannot be resolved on the shard** even though `GameSaveLog` (Source 4) still has its May rows. `UTCEvents` retains longer, but **QA/dev-profile runs** (`ResetProfileOnLogin`, `Skip=IE`, RAP captures) on a retail title may be **absent from the public Gaming cluster** (client telemetry routed elsewhere / not sampled), so their XUIDs return **0 rows** in both sources. When telemetry can't bridge XUID→EntityId, fall back to a **PlayFab Game Manager player lookup by XUID** (admin side) to read the `title_player_account`.

**Q — XUID → EntityId (single, FH6 shard):**
```kql
let _lookback = 30d;                          // shard retains ~30d — won't resolve older accounts
let _xuid = '2814622128365522';               // bare decimal XUID
cluster('pmain01sharded004.westus2.kusto.windows.net').database('ShardedDB023').['events.all']
| where Timestamp > ago(_lookback)
      and EntityLineage_title == '16D460'
      and tostring(EventData.Payload.Standard.UserId) == _xuid
| summarize Events = count(), FirstSeen = min(Timestamp), LastSeen = max(Timestamp)
            by XUID = _xuid, EntityId = toupper(tostring(EntityLineage_title_player_account))
| where isnotempty(EntityId)
| order by LastSeen desc
```

**Q — EntityId → XUID (single, FH6 shard):** same table, swap the filter to
`toupper(tostring(EntityLineage_title_player_account)) == toupper(_entityId)` and
`isnotempty(tostring(EventData.Payload.Standard.UserId))`, then read `EventData.Payload.Standard.UserId`.

**Q — Batch XUID → EntityId (FH6 shard):**
```kql
let _xuids = dynamic(['2814622128365522','2535410624887126']);
cluster('pmain01sharded004.westus2.kusto.windows.net').database('ShardedDB023').['events.all']
| where Timestamp > ago(30d) and EntityLineage_title == '16D460'
      and tostring(EventData.Payload.Standard.UserId) in (_xuids)
| extend EntityId = toupper(tostring(EntityLineage_title_player_account)),
         XUID = tostring(EventData.Payload.Standard.UserId)
| where isnotempty(EntityId)
| summarize LastSeen = max(Timestamp), Events = count(), EntityIds = make_set(EntityId) by XUID
| extend EntityId = tostring(EntityIds[0])     // EntityIds size >1 ⇒ XUID has data in multiple titles
| project XUID, EntityId, EntityIds, Events, LastSeen
```

**Q — Title-agnostic map via Gaming `UTCEvents`** (use for non-FH6 titles or no shard access):
```kql
let _xuid = '2814622128365522';
cluster('gaming.westus.kusto.windows.net').database('Gaming').UTCEvents
| where timestamp > ago(7d)
      and name startswith 'Microsoft.Gaming.PlayFab.GameSave.'
      and isnotempty(tostring(data.entityId))
| extend XUID = replace_string(tostring(xbl_xid), 'x:', '')      // xbl_xid may be `x:2814...`
| where XUID == _xuid
| summarize Events = count(), LastSeen = max(timestamp)
            by XUID, EntityId = toupper(tostring(data.entityId)), PfTitleId = tostring(data.pfTitleId)
| order by LastSeen desc
```

> Validated live 2026-06-29 (Q1↔Q2 round-trip consistent). Replace `16D460` / cluster / database as needed.

#### Dedicated Rollup Tables (Gaming Cluster) — PREFERRED for investigations

The Gaming cluster has **dedicated rollup tables** that are the richest data source for cross-device save investigations. **Use these instead of raw UTCEvents** for structured queries — they have typed columns, device IDs, and upload/download detail in one place.

| Table | Purpose | Key Advantage |
|-------|---------|---------------|
| `PlayFabGameSaveContextActivation` | Every context activation across all devices | Has `ConflictResolution`, `WasContention`, `BaseVersion`, `TotalActiveBytes`, `XblDeviceId`, `DeviceModel` |
| `PlayFabGameSaveContextSync` | Every upload AND download operation | Has `IsDownload`, `SyncSizeBytes`, `FileCount`, `BlockCount`, `XblDeviceId`, `InGame` |

**Filter by:** `PfTitleId` (hex title ID like "16D460") + `EntityId` (PlayFab entity hex) or `XuidList`

**PlayFabGameSaveContextActivation schema** (key columns):
```
EventTime, XblDeviceId, DeviceModel, PlatformType, PfTitleId, EntityId,
ContextVersion, BaseVersion, TotalSizeBytes, TotalActiveBytes, CompressedSizeBytes,
ConflictResolution (0=NoConflict, 1=KeepLocal, 2=TakeRemote, 3=SelectVersion),
WasContention (bool), SyncState (5=Complete), ConflictVersion, LoserVersion,
LauncherName, SessionId, XuidList (dynamic array)
```

**PlayFabGameSaveContextSync schema** (key columns):
```
EventTime, XblDeviceId, PfTitleId, EntityId, Version,
IsDownload (bool: true=cloud→device, false=device→cloud),
SyncSizeBytes (bytes transferred — 0 on download = already current),
TotalSizeBytes (container total after sync), FileCount, BlockCount,
ElapsedTimeMs, Hresult, RetryCount, InGame (bool), SessionId
```

**Example — cross-device sync timeline:**
```kql
PlayFabGameSaveContextSync
| where EventTime > ago(24h)
| where PfTitleId == "16D460"
| where EntityId == "BD61BB4D2A21AFFD"
| project EventTime, XblDeviceId, Version, IsDownload,
    SyncSizeBytes, TotalSizeBytes, FileCount, BlockCount,
    ElapsedTimeMs, Hresult, RetryCount, InGame
| order by EventTime asc
```

**Example — activation history with conflict detection:**
```kql
PlayFabGameSaveContextActivation
| where EventTime > ago(7d)
| where PfTitleId == "16D460"
| where EntityId == "BD61BB4D2A21AFFD"
| project EventTime, XblDeviceId, DeviceModel, ContextVersion, BaseVersion,
    TotalSizeBytes, TotalActiveBytes, ConflictResolution, WasContention,
    SyncState, ConflictVersion, LoserVersion
| order by EventTime asc
```

**Key interpretation patterns:**
- Upload: `IsDownload=False`, `SyncSizeBytes > 0` = data pushed to cloud
- Download (delta): `IsDownload=True`, `SyncSizeBytes > 0` = delta fetched from cloud
- Download (no-op): `IsDownload=True`, `SyncSizeBytes = 0` = already current
- Full re-download: `IsDownload=True`, `SyncSizeBytes ≈ CompressedSizeBytes` = local was wiped
- Device switch: Download on Device B with same version that Device A just uploaded
- Version gap: `BaseVersion` != previous `ContextVersion` = missing uploads
- Size regression: `TotalSizeBytes` drops between versions = potential data loss

#### Legacy Materialized Views (Gaming Cluster)

| View | Key Fields | Notes |
|------|-----------|-------|
| `XboxConnectedStorageActivations` | `EventTime`, `Xuid` (numeric, no prefix), `ConflictResolution`, `SyncState`, `TotalBytes`, `XblDeviceType` | Deduplicated — far fewer rows than UTCEvents |

These use `EventTime` (not `timestamp`) and `Xuid` (numeric, no `x:` prefix) — different from the raw UTCEvents table.

#### Complete Telemetry Gap Assessment

| Data Point | SDK (PlayFabInternal) | GRTS (UTCEvents) | Server Shard (`playfab.gamesave`) |
|-----------|----------------------|-------------------|-----------------------------------|
| Version finalization | ✅ Full history | ❌ Not emitted | ✅ **Authoritative** (`gamesave_version_finalized`) |
| Base-version chain | ⚠️ Limited | ✅ BaseVersion | ✅ **Authoritative** `BaseVersion` per finalize |
| Conflict resolution | ❌ Always empty | ✅ ConflictRes 0/1/2/3 | ❌ Not emitted |
| isWinner / loserVersion | ❌ Not in any event | ✅ In ContextActivation | ❌ Not emitted |
| Version sizes | ✅ TotalSizeBytes | ✅ TotalBytes/SyncSize | ✅ TotalSizeBytes + TotalFileCount |
| Device types | ✅ DeviceType field | ✅ XblDeviceType | ✅ DeviceType field |
| Sync upload/download | ❌ Limited | ✅ Full details | ❌ Not emitted |
| Known-good marking | ✅ Full history | ❌ Not emitted | ✅ `gamesave_version_marked_known_good` |
| Save description (decoded) | ❌ | ❌ | ✅ `Description` (base64 → readable) |
| Rollback flag | ❌ | ❌ | ✅ `IsGeneratedByRollback` |
| Local cache deletes | ✅ context_delete | ❌ Not emitted | ❌ Not emitted |
| Steam Deck coverage | ✅ Full | ❌ Zero events | ⚠️ Only if finalized to PlayFab cloud |

> **⚠️ Critical:** Any investigation involving conflict resolution or isWinner behavior **MUST** query the Gaming cluster UTCEvents. The SDK telemetry alone cannot prove or disprove conflict bugs — conflict fields are never populated on the SDK side.

> **⚠️ Server-authoritative version chain:** For data-loss / save-stomp / cloud-wipe (REQ#789) investigations, the production shard's `playfab.gamesave` finalize events (§3.2 Source 3) are the authoritative record of what actually committed to the cloud — independent of any device. Use them to detect `BaseVersion` regressions and size/file-count drops, then correlate with the conflict data in UTCEvents.

> **⚠️ Steam Deck blind spot:** Steam Deck uses the inproc/SDK path exclusively and emits zero UTCEvents. You can only see Steam Deck data in PlayFabInternal. For cross-platform conflicts, the conflict data appears only in the Xbox/GRTS side of UTCEvents.

#### ⚠️ "Gaming Handheld" ≠ Steam Deck — engine discrimination (learned on Bug 4401522)

A device whose `DeviceType` / `DeviceModel` reads **`Windows.Desktop : Gaming Handheld`** is a
**Windows GRTS handheld** (ROG Ally, Legion Go, MSI Claw, etc.), **NOT** a SteamOS Steam Deck. This
trips up cross-device investigations because bug reports (and even Forza QA) colloquially call any
handheld a "Steam Deck."

**How to tell what engine a device actually ran (authoritative discriminator):**

| Signal | GRTS path (Xbox console, **Windows handheld**, GRTS PC) | inproc path (**SteamOS Steam Deck**, forced-inproc PC) |
|--------|--------------------------------------------------------|--------------------------------------------------------|
| Emits **UTCEvents** (`gaming.westus`)? | **YES** — `Microsoft.Gaming.PlayFab.GameSave.ContextActivation/Sync`, plus rollup rows in `PlayFabGameSaveContextActivation/Sync` | **NO — zero UTCEvents** |
| `platformType` (Source 1, `PlayFabInternal`) | `Windows` / `Xbox` | `Windows` (older), **`SteamDeck`** (after **2510 QFE3**), or `WindowsInproc` (forced) |
| Game assert log banner | `"Running on GRTS platform with out of proc PlayFab saves upload support"` | inproc/SDK path — does **NOT** print the GRTS banner |
| ETL / PGS on disk | Produced (GRTS) | Not produced — inproc SDK logs only (§6) |

> **Decision rule:** if the device has **any** UTCEvents / rollup rows, it ran **GRTS** — even if it's
> a handheld. Only a device that is **absent from UTCEvents** is a candidate inproc/Steam Deck. **The
> "zero UTCEvents" test is the dependable discriminator.** `platformType` (Source 1) is a *weaker* tag
> than it appears: in observed Forte data it reads **`Windows` for everything** (inproc **and** the
> GRTS relay both report `Windows`), and the SDK only emits the positive **`SteamDeck`** value after
> **2510 QFE3** — so an older inproc Steam Deck is indistinguishable from PC/GRTS by `platformType`
> alone. Use UTCEvents presence/absence; treat `platformType=SteamDeck` as positive-confirm-only.

**Why this matters:** a "Steam Deck → console" rollback that is actually a **GRTS-handheld → GRTS-console**
rollback is a **GRTS-path** bug, not an inproc bug — so it won't be caught by inproc-only test passes,
and the fix belongs on the GRTS/client conflict-resolution side. (On Bug 4401522, both captured assert
logs were `forza_gaming.xbox.scarlett...` GRTS **consoles**; the "Steam Deck" halves were never
captured, and the in-retention live repro device was a **GRTS Windows handheld**, not SteamOS.)

#### The `base=0` cross-device rollback pattern (Bug 4401522 mechanism)

The cross-device save-stomp reproduces **entirely on the GRTS path** as follows:

1. A device starts a context with **`BaseVersion = 0`** in `PlayFabGameSaveContextActivation`. `base=0`
   means **"my local save has no cloud lineage"** — the local container holds data but no record of
   which cloud version it derived from. Common cause: **`ResetProfileOnLogin=true`** (a Forza test/cert
   flag) writes a **fresh starter profile locally** *before* sync, so the device has new local bytes
   that never came from the cloud.
2. The cloud head is a **real save** (e.g. v32, tens of MB). So activation is **not** a clean
   first-sync into an empty local — it's **local-with-no-lineage (base 0) vs a populated cloud** =
   a genuine **version conflict** (`ConflictResolution != 0`, `WasContention=false`).
3. The conflict resolves to **`ConflictResolution=1` (KeepLocal)** — the **wrong** direction. The
   tiny fresh-starter is kept and declared the winner: the next `gamesave_version_finalized` carries a
   `Conflict` sub-object **`{"IsWinner":true,"ConflictingVersion":<cloudHead>}`**, and the matching
   `ContextSync` shows `IsDownload=true, SyncSizeBytes=0` (it **skipped** downloading the real cloud
   save) followed by `IsDownload=false` (it **uploaded** the starter over the cloud).
4. Result: the cloud head is replaced by the small stale-base version → **rollback / data wipe**. The
   service data-loss net (deployed ~2026-06-18; `LogSuspectedDataLossOnFinalize` →
   `LogDataLossProtectionAutoRollbackChoice`) now catches the size collapse and emits an auto-rollback
   finalize `DeviceType "<...> (Cloud AutoRollback)"`, `IsGeneratedByRollback=true` to restore the last
   known-good. Before that date it committed unprotected (permanent rollback).

**The defect:** a `base=0` (no-lineage) local save must **never** win a conflict against a populated
cloud — it should resolve **TakeRemote** (download the cloud), not **KeepLocal**. Telemetry shows the
**outcome** (`ConflictResolution=KeepLocal`) but not *who* chose it (client auto-policy vs. a conflict
UI selection) — that distinction is the open client-side question.

**The `Conflict` sub-object on `gamesave_version_finalized` (Source 3):** payloads carry **either** a
`Description` (base64 save description) **or** a `Conflict` object on conflicting finalizes:
`tostring(EventData.Payload.Conflict)` → `{"IsWinner":bool,"ConflictingVersion":N}`. `IsWinner:true` =
this finalize beat version N in a conflict. This is the **only** Source-3 surfacing of conflict
outcome (UTCEvents/rollups carry the rest).



The SDK emits exactly 5 telemetry events to the `playfab.gamesave.internal` namespace:

| # | Event Wire Name | When Emitted | Key Fields |
|---|----------------|--------------|------------|
| 1 | `context_activation` | Activation completes (success, offline, or failure) | `totalSizeBytes`, `manifestState`, `syncState`, `conflictResolution`, `contextVersion` |
| 2 | `context_activation_failure` | Activation fails (ADDITIONAL to #1, not instead) | `hresult`, `syncState`, `callingLocation`, `httpStatus`, `canceled`, `aborted` |
| 3 | `context_sync` | Sync operation completes (upload or download) | `blockCount`, `fileCount`, `syncSizeBytes`, `originalSizeBytes`, `syncDownload`, `elapsedMs` |
| 4 | `context_sync_error` | Sync encounters an error | `hresult`, `errorSource`, `blockName`, `retryCount`, `correlationVector` |
| 5 | `context_delete` | Save data deleted | `deleteType`, `totalSizeBytes`, `hresult`, `contextVersion` |

**Critical semantics:**
- `context_activation` fires on ALL attempts (success + failure)
- `context_activation_failure` fires ADDITIONALLY on failure (not instead of `context_activation`)
- `context_sync` has `hresult` that is always S_OK (success-only for downloads, mixed for uploads)
- `context_sync_error` is the real failure indicator — use this for failure rates
- Each event has a once-only guard to prevent double-emission within a session

### 3.4 Common Fields on Every Event

| Field | Source | Notes |
|-------|--------|-------|
| `userId` | `localUser.LocalId()` | Platform-specific local user ID |
| `platformType` | `PFPlatformGetPlatformType()` | `Windows`, `SteamPc`, `SteamDeck`, `Xbox`, `WindowsInproc` |
| `sessionId` | `CreateGUID()` per activation | Correlates all events in one gameplay session |
| `entityId` | Entity handle (server-resolved) | Not in event payload — resolved by PlayFab backend |

### 3.5 Querying PlayFabInternal for PFGameSave Events

```kql
// All PFGameSave events for a specific entity
['events.all']
| where Timestamp > ago(7d)
| where EntityLineage_title == "YOUR_TITLE_ID"
| where FullName_Namespace == "playfab.gamesave.internal"
| where Entity_Id == "YOUR_ENTITY_ID"
| project Timestamp, FullName_Name, EventData
| order by Timestamp asc
```

**PlayFabInternal quirks:**
- Table requires bracket notation: `['events.all']`
- Event payload nested under `EventData.Payload.*` — use `tostring(EventData.Payload.totalSizeBytes)` not `EventData.totalSizeBytes`
- String conflict resolution values: `CR_TakeRemote`, `CR_KeepLocal`, `CR_NoConflictsExpected` — but **always empty/None** in practice
- Single-quoted KQL strings with `\d` cause SYN0002 errors — use double-quoted with `\\d`
- Entity filter: `Entity_Id`, Title filter: `EntityLineage_title`

### 3.6 Querying UTCEvents (Gaming Cluster)

```kql
// All game save events for a user by XUID
UTCEvents
| where EventInfo_Time > ago(7d)
| where xbl_xid == "x:YOUR_XUID"
| where EventInfo_Name has "GameSave" or EventInfo_Name has "Context"
| project EventInfo_Time, EventInfo_Name, EventInfo_BaseData
| order by EventInfo_Time asc
```

**UTCEvents quirks:**
- XUID has `x:` prefix in `xbl_xid` column
- SCID field has a TYPO in old telemetry: `SerivceConfigurationId` (missing 'c')
- Steam-launched titles bypass GRTS and emit ZERO UTCEvents — must use PlayFabInternal
- The `data` bag uses **camelCase** (`data.conflictResolution`), not PascalCase — wrong case silently returns empty

```kql
// Conflict resolution events — the ONLY source for isWinner/loserVersion data
UTCEvents
| where EventInfo_Time > ago(30d)
| where xbl_xid == "x:YOUR_XUID"
| where EventInfo_Name == "Microsoft.Gaming.PlayFab.GameSave.ContextActivation"
| extend conflictRes = tostring(data.conflictResolution),
         conflictVer = tostring(data.conflictVersion),
         loserVer = tostring(data.loserVersion),
         baseVer = tostring(data.baseVersion),
         wasContention = tostring(data.wasContention),
         contextVer = tostring(data.contextVersion)
| where conflictRes != "0"  // filter out NoConflict
| project EventInfo_Time, contextVer, conflictRes, conflictVer, loserVer, baseVer, wasContention
| order by EventInfo_Time asc
```

### 3.7 Key Kusto Queries for Golden Path Verification

See `.squad/skills/kusto-telemetry/queries/` for the full query library. Most relevant:

| Query File | Purpose |
|-----------|---------|
| `context-activations.kql` | Track activation events by user/title |
| `upload-download-activity.kql` | Sync operations (upload/download) |
| `upload-download-summary-per-title.kql` | Aggregated sync stats |
| `error-investigation.kql` | Error patterns |
| `playfab-gamesave-telemetry.kql` | PlayFabInternal game save events |

---

### 3.8 Cross-Team Intel — Gaming-Cluster Rules, Vectors & Crash Correlation

> **Source:** kelcon's fleet-analysis system, `C:\git\xbdiag_analysis\kusto\docs\DATA-ANALYSIS-GUIDE.md`
> (+ `INTERNALERROR-ANALYSIS.md`, `TELEMETRY-SOURCE-CATALOG.md`). That system tracks the
> live Hub#692 GameSave data-loss investigation (~800–900 affected users/day). This section
> distills the parts most useful for SDK-side debugging. Treat the source as the living
> authority for fleet numbers; cross-check before quoting figures.

#### 3.8.1 CRITICAL Gaming-cluster KQL rules (violation = silently wrong results)

| Rule | Why | Correct form |
|------|-----|--------------|
| **Filter on `ExtIngestTime`, not `EventTime`** | `EventTime` is client-reported and contains garbage dates (e.g. year 2117); it also defeats extent pruning | `where ExtIngestTime between(ago(7d) .. now())` |
| **Always `where XblSandbox == "RETAIL"`** | Test sandboxes (`MSFT.1`, `XDKS.1`, `CERT`) pollute fleet results | filter every Gaming-table query |
| **HRESULTs are UNSIGNED** | Gaming cluster stores HRESULT as `UInt64`; a signed/negative literal matches **nothing** | use `2156068867`, not `-2138898429` |
| **`gamingbi` cluster is DEAD** | Returns empty tables | use `gaming` cluster; Watson = `database("Watson")`, XDM = `database("XDM")` |
| **Watson view: use the `_In` suffix** | `Wnrt_Xbox_UM_CabHits` (no suffix) is an empty materialized view | `database("Watson").Wnrt_Xbox_UM_CabHits_In` |
| **PlayFabInternal regex: `[0-9]` not `\d`** | Backslash breaks KQL syntax in that cluster (matches our §3.5 note) | `extract('"Version":([0-9]+)', 1, Payload)` |

#### 3.8.2 Additional Gaming materialized views (fast — prefer over UTCEvents)

Beyond `PlayFabGameSaveContextActivation` / `…ContextSync` (§3.2), the fleet team relies on:

| Table | ~Vol/day | Key use |
|-------|----------|---------|
| `PlayFabGameSaveContextActivationFailure` | ~3–41K | Activation failures — **`CallingLocation` (CL) + `Hresult` + `Canceled`/`Aborted`** classify the loss vector |
| `PlayFabGameSaveContextSyncError` | ~96 | Sync chunk failures with `Hresult` + `ErrorSource` — low volume, each actionable |
| `PlayFabGameSaveContextDelete` | ~64 | Context deletes — **`Type=2` (DT_All) looks like data loss to studios** |
| `XboxConnectedStorageOrphans` | ~750K | Orphan blobs / quota (`UnrefBytes`, `QuotaBytes`) — feeds the eviction→loss chain |
| `XboxConnectedStorageInternalError` | ~3.7M | Device-level HRESULT diagnostics; `ErrorText` carries the WIL failure site (file:line) |

#### 3.8.3 Unsigned HRESULT reference (Gaming cluster)

| Unsigned | Hex | Meaning | Loss vector |
|----------|-----|---------|-------------|
| 2156068867 | `0x80830003` | CS_E_USER_CANCELED | Slice 1a (CL=11) |
| 2147500036 | `0x80004004` | E_ABORT | multiple |
| 2149122448 | `0x80190190` | HTTP 400 (PF service) | Slice F |
| 2147954402 | `0x80072EE2` | Network timeout | environmental |
| 2147942432 | `0x80070020` | ERROR_SHARING_VIOLATION | Slice D (XVD lock) |
| 2147942402 | `0x80070002` | FILE_NOT_FOUND | usually benign noise |
| 2147944126 | `0x800706BE` | RPC_S_CALL_FAILED | service instability / XVD unmount |
| 2156077073 | `0x80832011` | CS_E_REMOTE_ATOM_MISMATCH | **persistent corruption** (devices stuck in a retry loop) |

> **InternalError top signatures** (from `INTERNALERROR-ANALYSIS.md`): `0x8000FFFF` E_UNEXPECTED "enqueue after shutdown" dominates (~77%, shutdown race); `0x80832011` atom-mismatch is low-device/high-per-device **corruption**; `0x800710D2` "empty container, non-empty progress map" is widespread but self-correcting; the SHARING_VIOLATION + BUSY + SYNC_IN_PROGRESS cluster = I/O contention.

#### 3.8.4 Data-loss vector taxonomy (Hub#692 "slices")

The fleet team classifies each detected loss by joining the rolled-back user to their
`…ActivationFailure` / `…SyncError` records and matching HRESULT + `CallingLocation`:

| Slice | Signature | Root cause |
|-------|-----------|------------|
| **1a** | `CallingLocation == 11` AND `Hresult == 2156068867` (0x80830003) | Activation **canceled during sync** |
| **D** | SHARING_VIOLATION / RPC failures | **XVD unmount hang** / partition lock contention |
| **F** | `Hresult == 2149122448` (HTTP 400) | PlayFab service rejection |
| **G** | multi-device, ext-manifest mismatch | **stale ext manifest** (~652 multi-device "losers") |

Last measured vector mix (June 2026): **No Signal 43.8%**, E_ABORT 28.5%, HTTP 400 21.3%,
Network Timeout 6.3%. "No Signal" = rolled-back user with **no** matching failure record — the
unknown-vector bucket to chase. (Slice G "stale ext manifest" lines up directly with the GRTS
`PendingConflictDesc`/base-version findings in §21 and §24.)

#### 3.8.5 Client-side AutoRollback detection (complements service-log Source 4)

Source 4 (§3.2) finds rollbacks from the **service** log (`LogSuspectedDataLossOnFinalize`).
The fleet team independently detects the same loss from **client** sync telemetry — a save that
**shrank >50%** between consecutive uploads:

```kql
PlayFabGameSaveContextSync
| where ExtIngestTime between(ago(7d) .. now())
| where XblSandbox == "RETAIL" and IsDownload == 0 and TotalSizeBytes > 0
| summarize by EntityId, PfTitleId, Version=tolong(Version), TotalSizeBytes
| sort by EntityId, PfTitleId, Version asc
| extend PrevSize = prev(TotalSizeBytes), PrevEntity = prev(EntityId)
| where EntityId == PrevEntity and PrevSize > 0 and TotalSizeBytes < PrevSize * 0.5
| extend ShrinkPct = round(100.0 * (PrevSize - TotalSizeBytes) / PrevSize, 1)
```
Use **both**: the service log is authoritative on *what was rolled back*; this client view adds
`Hresult`, `RetryCount`, `XblDeviceId`, `OsVersionShort` (build) on the failing sync itself.
Also watch for `IsDownload=1` with `FileCount=0` / `TotalSizeBytes=0` — an **empty container
download = the rollback landing on the device** (the user is now playing on rolled-back state).

#### 3.8.6 Watson crash correlation (Gaming cluster → Watson DB)

Join via `DeviceGlobalId` (`g:NNNNN`), which is in the PF tables and matches Watson `DeviceId`
(NOT the same ID space as `XblDeviceId` hex — you cannot convert between them):

```kql
let watsonDeviceId = toscalar(
    PlayFabGameSaveContextSync | where XblDeviceId == "<HEX_DEVICE>" | take 1 | project DeviceGlobalId);
database("Watson").Wnrt_Xbox_UM_CabHits_In
| where DeviceId == watsonDeviceId and CrashTime > ago(30d)
| where ModuleName has_any ("connectedstorage", "gamingservices", "xgameruntime")
| project CrashTime, ModuleName, FunctionName, BucketId, OsBuildLab
| order by CrashTime desc
```

#### 3.8.7 Identity & cross-source join keys

| ID | Format | Used to join |
|----|--------|--------------|
| `EntityId` = `Entity_Id` = title_player_account | 16-char hex | Gaming ↔ PlayFabInternal ↔ shard (§3.2 Source 3/4) |
| `XUID` / `UserId` | decimal | Xbox Live identity |
| `XblDeviceId` | 16-char hex | Gaming device id (NOT Watson-compatible) |
| `DeviceGlobalId` | `g:` + decimal | **Gaming ↔ Watson** crash join |
| `PfTitleId` | hex (e.g. `16D460`) | title scope across all sources |

> **Note on the canonical worked timeline** (DATA-ANALYSIS-GUIDE §6, Step 9): the kill chain it
> documents — `Upload vN OK → CL=11 ActivationFailure (0x80830003) → vN+1 activated with
> TotalSizeBytes=0 (ROLLBACK) → Download vN+1 = 0 bytes/0 files → Upload vN+1 on rolled-back
> state` — is the **client-telemetry mirror** of the truncated-finalize/auto-rollback pattern the
> `dataloss` skill (`specs/playfab-gamesave/dataloss/`) finds from the service side. Cross-referencing
> the two confirms whether a given user's loss is CL=11 (Slice 1a) vs stale-ext-manifest (Slice G).

---

### 3.9 Game-Telemetry Progress-Loss Detection (Forza) — player-visible ground truth

> **Source:** Andy's `C:\git\PlayFab.C.2\Tools\DataLossDetection\progress-loss-detection-methodology.md`
> (validated live against ADX 2026-06-22). This detects **player-visible** progress loss
> directly from the **game's own gameplay telemetry** — the ground truth that the "base64 XP"
> idea only approximates. It lives on the **same shard** as Source 3 (`pmain01sharded004 /
> ShardedDB023 / ['events.all']`, title `16D460`) but reads Forza events, not GameSave events.

**Two game events carry the signal (title `16D460`):**

| Event | Cadence | Payload signal |
|-------|---------|----------------|
| `ForzaProfileLoadSucceeded` | once per **boot** | `EventData.Payload.CarsInGarage` (0 on a wiped/default load), `SecondsSpentDriving`. Reports the **loaded profile's** content — the wipe is observed *at load time* and can't be masked by re-accumulation. **Primary signal.** |
| `ForzaRewardData` | in-session stream | `DistanceDriven`, `Level`, `Standard.PlayTime` (cumulative). The "odometer" — catches **load-failure** wipes that emit no boot event. |

> Player id = `EntityLineage_title_player_account` (bare hex = `Entity_Id`). `Standard.PlayTime`/`TotDist` are **session** counters, not loaded-profile content.

**Loss signature = full-profile wipe:** every cumulative counter collapses to defaults **at once**
(`CarsInGarage`, `Level`, `TotalCars`, `DistanceDriven`, `PlayerHousesOwned`, `RoadsDiscovered`,
`PlayTime`) on a fresh `SesId`. Markers by robustness: (1) multi-field collapse (rejects
single-field corruption noise), (2) `PlayTime` regression — a monotonic-invariant violation,
co-occurs ~88%, (3) new `SesId` at the collapse.

**Detection rules that matter:**
- **Trailing baseline, not a fixed daily window.** Establish the player over a wide lookback
  (`PeakCars > 10` over prior 7+ d), require only the **wipe** (`CarsInGarage == 0`) to land in
  the target window. ~2.5× more recall than same-day (518 vs 208, 1-day wipe window).
- **Regression, not peak-vs-last.** Peak-vs-last masks rollback-recovered transients (a player
  who hit 0 then auto-recovered to 41 shows `peak==last==41`). Use `EstT = minif(Timestamp,
  established)`, `WipeT = maxif(Timestamp, wiped)`, flag `EstT < WipeT`.
- **Scale technique (important KQL lesson):** at ~350M events/3d, `partition by Player … prev()`
  hits `LimitsExceeded` and a global `order by … prev()` hits `DeadlineExceeded`. Use
  **`summarize hint.shufflekey=Player`** with conditional aggregates (`minif`/`maxif`) — memory-bounded.
- The **odometer is a short-window tool** (parses the gameplay firehose; falls over past ~2 d).
  Run the boot-load detector for the wide baseline; run the odometer per-day (`target=1d`) and union.

**Recovery / mitigation attribution:** `gamesave_version_finalized` (namespace `playfab.gamesave`,
same store — our Source 3) with `IsGeneratedByRollback == true` = a rollback. Two payload fields
classify *why*:

- **`RollbackReason`** (string) — **added to the payload by the service ~2026-06-22; now live.**
  Present on **100%** of rollback-generated finalizes (verified 138/138 in 12h). Observed values
  (24h, title `16D460`):
  | `RollbackReason` | `DeviceType` "(Cloud AutoRollback)" suffix | ~24h count | Meaning |
  |---|---|---|---|
  | `Suspected data loss detected during finalize.` | **yes** (230/230) | 230 | the **auto data-loss mitigation** fired — the effectiveness signal |
  | `Manual Rollback from Steward` | mostly **no** (87 no / 1 yes) | 88 | operator/support manual revert (Steward tool) |
  | `Player game saves rollback` | no | 1 | player/client-initiated rollback |
- **`DeviceType has "(Cloud AutoRollback)"`** — the older proxy discriminator, still valid and now
  corroborated by `RollbackReason`. Set service-side via
  `ManifestConversion.ToVersionFinalizedEventOnRollback`, gated per title by
  `AppendCloudAutoRollbackSuffix` (default `true`, enabled for `16D460`).

> **Prefer `RollbackReason`** now that it's on the payload — it cleanly separates the **auto
> data-loss mitigation** (`Suspected data loss detected during finalize.`) from **manual Steward**
> reverts, which the suffix-only method lumped into a generic "other rollback" bucket. *(Historical
> note: this field was previously NOT on the payload, so older docs/queries used the `DeviceType`
> suffix as the only discriminator — that workaround is no longer required.)*

**Description field:** decode with Kusto's built-in `base64_decode_tostring()` **server-side** —
e.g. `extend Progress = base64_decode_tostring(tostring(EventData.Payload.Description))`. Verified
live; handles multi-language payloads (English `Cars In Garage: 36`, French `Voitures dans le
garage : 331`, Portuguese `Carros na Garagem: 216`). The service also confirmed `BaseVersion` and
`RollbackReason` are now in the same payload.

#### 3.9.1 Deterministic positional parse of `Description` (locale-invariant)

> **Source:** Andy McCalib, 2026-06-23 — validated locally. The `Description` is a **fixed 5-line
> `Label: Value` block whose field *order* is locale-invariant** (23 locales observed). **Extract by
> line position, not by label.** For Forte `16D460` the 5 lines are, in order:
> `0` Cars In Garage · `1` Discover Japan Progress · `2` Horizon Festival Progress · `3` XP · `4` Credits.

Parse rules (all confirmed live):
- **Split on `\n`, index by position** — labels differ per locale, positions never do.
- **Values are integers with locale grouping separators** (`6,604,752` / `1.817.559` / `4 238 068` /
  NBSP). Since they're always whole numbers, **strip all non-digits** — unambiguous, no decimal-comma
  confusion: `tolong(replace_regex(extract('[:：](.*)', 1, tostring(lines[N])), @'[^0-9]', ''))`.
- **Colon class `[:：]`** covers Chinese fullwidth `：`.
- **Parse success:** Andy 100% client-side (3,000/3,000), 99.9998% in-cluster. Reproduced here:
  **99.995%** in-cluster (349,180/349,196 in 2h; 349,195 were exactly 5 lines).

```kql
['events.all']
| where Timestamp > ago(1d) and EntityLineage_title == '16D460'
        and FullName_Namespace == 'playfab.gamesave' and FullName_Name == 'gamesave_version_finalized'
        and isnotempty(tostring(EventData.Payload.Description))
| extend lines = split(base64_decode_tostring(tostring(EventData.Payload.Description)), '\n')
| extend Cars = tolong(replace_regex(extract('[:：](.*)', 1, tostring(lines[0])), @'[^0-9]', '')),
         XP   = tolong(replace_regex(extract('[:：](.*)', 1, tostring(lines[3])), @'[^0-9]', '')),
         Credits = tolong(replace_regex(extract('[:：](.*)', 1, tostring(lines[4])), @'[^0-9]', ''))
```

#### 3.9.2 Progress-drop detection (catches losses real-time detection misses)

A **drop** in a positional value (e.g. XP) from a player's established high-water mark to a much lower
later finalize is a **direct, decoded, player-visible** data-loss signal — and it catches players the
size-based / `LogSuspectedDataLoss` real-time detection misses. Tune by threshold; **Andy's measured
per-day yields (full population):** ≥90% drop ≈ **2,000/day**, ≥99% ≈ **200**, ≥99.9% ≈ **100**.

Robust shape (drop vs **running established high-water**, survives version gaps + rollback re-accumulation):
```kql
['events.all']
| where Timestamp > ago(1d) and EntityLineage_title == '16D460'
        and FullName_Namespace == 'playfab.gamesave' and FullName_Name == 'gamesave_version_finalized'
        and isnotempty(tostring(EventData.Payload.Description))
| extend Player = tostring(EntityLineage_title_player_account), Version = tolong(EventData.Payload.Version),
         IsRollback = tostring(EventData.Payload.IsGeneratedByRollback) == 'true',
         XP = tolong(replace_regex(extract('[:：](.*)', 1,
              tostring(split(base64_decode_tostring(tostring(EventData.Payload.Description)), '\n')[3])), @'[^0-9]', ''))
| where isnotnull(XP)
| partition hint.strategy=native by Player (
    order by Version asc
    | scan declare(RunMax:long=0) with ( step s: true => RunMax = iff(s.RunMax > XP, s.RunMax, XP); )
    | extend PriorMax = prev(RunMax) )      // established high-water BEFORE this version
| where not(IsRollback) and isnotnull(PriorMax) and PriorMax > 1000 and XP < PriorMax
| extend DropPct = round(100.0 * (PriorMax - XP) / PriorMax, 2)
| summarize MaxDrop = max(DropPct) by Player
| where MaxDrop >= 90      // tune: 90 / 99 / 99.9
```

> **Caveats (from local validation):** (1) absolute counts depend on threshold **and** the establishment
> baseline window — use a **trailing baseline** (per §3.9, establish over 7–30 d, detect the drop in the
> target window) for the production-scale yields above; a same-window detector under-establishes players
> who peaked earlier. (2) The XP-drop method only sees wipes whose finalize **still emitted a (default)
> Description** — i.e. the **Pattern B profile-only collapse** (the small progression file reset to a
> default but present). Whole-container truncations that omit the description (Pattern A) are invisible
> to this method and need the size/ext-manifest signal instead. Use drop-detection **alongside**, not
> instead of, the service + size signals.

#### 3.9.3 Forza player-facing error codes (`E:47-b`, etc.) + game save log

> **Source:** local exploration 2026-06-23. The `E:NN-x` codes a player sees on a failed save load are
> in the **`ForzaProfileLoadFailed`** event (`custom` namespace, same shard `['events.all']`, title
> `16D460`). This is the **load-FAILURE** counterpart to `ForzaProfileLoadSucceeded` (§3.9): the game
> failed to load the profile and came up on a default — i.e. a wipe the player directly experienced.

| Event (`custom` ns) | ~Vol (16D460) | Purpose |
|---|---|---|
| `ForzaProfileLoadFailed` | ~300/2h | Profile load failed — carries the **`DisplayErrorCode`** the player sees |
| `ForzaProfileLoadBackupFailed` | ~16/2h | The backup-profile load also failed |
| `GenericSaveLog` | ~8M/3d | Game-side save-subsystem trace (init, GRTS version check, folder states) |

**`ForzaProfileLoadFailed` payload (`EventData.Payload.*`):**

| Field | Meaning |
|-------|---------|
| **`DisplayErrorCode`** | The player-facing code, e.g. `E:47-b`, `E:43-1b`, `E:8f-f`. **This is the "E:47-b".** |
| `ResultReason` | Human-readable cause, e.g. `Profile file not found.`, `Incorrect XUID found in binary save.`, `Profile options failed to read to end.` (a **garbled/binary** reason = the profile bytes were corrupt) |
| `DiagnosticErrorCode` / `DiagnosticFlags` | Numeric error + flag bits the code is built from (e.g. `E:47-b` → DiagErr 11, Flags 71) |
| `BackupDiagnosticErrorCode` / `BackupDiagnosticFlags` | Same for the backup-profile attempt |
| `LoadResult` | Load outcome enum (7 = failed in observed rows) |

**Observed code → reason map (title `16D460`, 24h):**

> **Code format & dual representation (Miguel, 2026-06-23):** the code is `E:<DiagnosticFlags>-<DiagnosticErrorCode>`.
> The same failure shows in **two equivalent forms — hex (the common `DisplayErrorCode`) and decimal** —
> e.g. `E:47-b` (hex) ≡ `E:71-11` (decimal), because `0x47=71` and `0xb=11`. Both forms carry identical
> `DiagnosticFlags`/`DiagnosticErrorCode`. Match on **either** form.

| `DisplayErrorCode` (hex / decimal) | Flags / Err | What succeeded vs failed | ~% of all boots |
|---|---|---|---|
| `E:47-b` / `E:71-11` | 71 / 11 | AddUser OK, profile **container present, profile blob MISSING** | ~0.04% |
| `E:43-1b` / `E:67-27` | 67 / 27 | AddUser OK, **profile container NOT FOUND** (other containers present) | ~0.02% |
| `E:4f-b` / `E:79-11` | 79 / 11 | container + blob present, **VersionFlags blob GONE** | ~0.03% |
| `E:8f-f` / `E:143-15` | 143 / 15 | container + blob present, **blob can't be DECRYPTED — data corruption** | ~0.02% |

`ResultReason` reads **"Profile file not found."** for the first three (the container/blob/VersionFlags is
missing); the decryption case shows garbled/binary text or **"Profile load failed: DecryptionFailed"**.
Other codes seen: `E:32f-0`/`E:815-0` = "Incorrect XUID found in binary save." · `E:2f-0` = "Profile
options failed to read to end." · `-L` suffix = local-copy variant.

> **Cross-reference to Watson (per Andy/Miguel):** these events carry the player's `Standard.UserId`
> (XUID, decimal). Match those XUIDs to Watson crash reports (Gaming-cluster `database("Watson")` via
> `DeviceGlobalId` — see §3.8.6) to correlate the load failure with a client crash. Miguel matched prior
> error XUIDs to Watson this way; the same applies to these four.

> **Why this matters for data loss:** the first three codes are **three distinct on-disk loss modes**
> all surfacing as "Profile file not found." — (a) container present but **profile blob gone** (`E:47-b`),
> (b) **whole profile container missing** while other containers survive (`E:43-1b`), (c) container+blob
> present but **VersionFlags blob gone** (`E:4f-b`). All three are the player-facing manifestation of the
> root cause this guide traces — the progression data missing/default at load. `E:8f-f` is the **corruption**
> variant (blob present but undecryptable). These are the **load-failure class** (no
> `ForzaProfileLoadSucceeded` is emitted), so they're invisible to the boot-load `CarsInGarage` detector —
> `ForzaProfileLoadFailed` is the only catcher. Combined the four are ~0.11% of all boots.
> The container-vs-blob-vs-VersionFlags split is a strong **GRTS-side lead**: selective loss of specific
> blobs within a surviving container points at the on-console XVD/container write path, not a wholesale wipe.

```kql
// Player-facing save-load error codes for a title (or a single player: add the entity filter)
['events.all']
| where Timestamp > ago(24h) and EntityLineage_title == '16D460'
        and FullName_Name == 'ForzaProfileLoadFailed'
| extend P = EventData.Payload
| project Timestamp, Player = tostring(EntityLineage_title_player_account),
    DisplayErrorCode = tostring(P.DisplayErrorCode), ResultReason = tostring(P.ResultReason),
    DiagnosticErrorCode = toint(P.DiagnosticErrorCode), DiagnosticFlags = tostring(P.DiagnosticFlags),
    LoadResult = toint(P.LoadResult), Platform = tostring(P.Standard.Platform), Xcloud = tostring(P.Standard.Xcloud)
| order by Timestamp desc
```

> **`GenericSaveLog`** is the game's own save-subsystem trace — `LogName`, `LogMessage`,
> `RootFolderState`, `BufferFolderState`, `GRTSVersion`, `ConsoleName`. Useful for the **client-side**
> init/load sequence (e.g. `MinGRTSVersionMet`, `TaskInitDoWork_*`, `InitStorageAsync`) when correlating
> a load failure to the GRTS state at boot. Filter `FullName_Name == 'GenericSaveLog'` and project
> `tostring(EventData.Payload.LogName)` / `LogMessage`.

> **Total loss population = boot-load wipes ∪ cloud auto-rollback** (different blind spots;
> neither is a superset). Measured 2026-06-22 (`baseline=7d`, `target=4d`): **2,220 players**
> (2,033 boot-load, 785 cloud auto-rollback, 598 both). **The rollback can fire *before* the
> next load**, so a cleanly-mitigated player never shows a gameplay wipe — only the service
> signal catches them. (Verified: entity `E99AB001975FBCA0` got a `Xbox Series (Cloud
> AutoRollback)` finalize at 05:30:45, then booted 48 s later reading 133 cars — intact. The
> `dataloss` skill's service-side detection catches this user; a pure-gameplay sweep would not.)

**⚠️ CRITICAL correction to size-based detection (§3.2 Source 3 / §3.8.5):** the Forza
progression file lives **inside the save-blob bytes the service never reads**, and is *tiny*
relative to 100+ MB of liveries/replays/photos. So **aggregate `TotalSizeBytes` / `TotalFileCount`
do NOT reliably correlate with these wipes** — a wipe often leaves total bytes **flat or higher**.
Aggregate-size shrink catches the subset where the whole container truncated (the 795 KB/3-file
cases), but **misses wipes masked by media**. The robust server-side marker is the **extended-manifest
profile-collapse**: the client-authored `extended-{Version}-manifest.json` enumerates the real file
tree, and the Forza progression file (`C_ProfileData`) collapses to a **~23–28 KB default-profile
fingerprint** on a wipe (healthy ~235–750 KB). Diff `C_ProfileData` against the prior finalized
version per-file — far better than aggregate size/count.

**Root-cause lead (population characteristic):** loss is **heavily concentrated in local,
non-streamed saves** — Xbox Series (Forza platforms 6/7) lose several× more than others, and on
platform 6 **cloud-streamed play (`Xcloud=true`) loses ~8× less** than local play. This points at
**client-side local-save load/persist** (consistent with the Xbox-Series mount/unmount flush-race
investigation), not a uniform server fault.

---

## 4. Correlating ETL + Kusto + PGS + Inproc Logs

### 4.1 Correlation Strategy

ETL, Kusto, PGS, and inproc log events capture the same operations from different vantage points. To correlate:

| ETL Field | Kusto Field | Inproc Log Pattern | How to Match |
|-----------|-------------|-------------------|--------------|
| Timestamp | `Timestamp` / `EventInfo_Time` | `[HH:MM:SS]` prefix | Within ~1-2 second window |
| `PFResponse.HttpStatus` | `context_activation_failure.httpStatus` | (not directly visible) | Exact match |
| `PFResponse.RequestType` | (inferred from event type) | `[PlayFab] PFXPALAddUserBegin` / `UploadWithUiAsync` | Match API call to phase |
| `PFUploadContextComplete.FileCount` | `context_sync.fileCount` | `[ExtManifest:after Upload]` JSON Files[] | Exact match |
| `PFUploadContextComplete.TotalSize` | `context_sync.syncSizeBytes` | Extended manifest `CompressSize` | Exact match |
| `PFUploadContextComplete.ElapsedMs` | `context_sync.elapsedMs` | `[Command] << ... >> elapsed=NNNms` | Approximate match |
| Process PID (from `NewPlayFabCaller`) | (not in Kusto) | (not in inproc) | Use timestamp correlation |
| (not in ETL) | (not in Kusto) | `[PlayFab] GameSaveAPIProviderGRTS::*` | SDK internal state only |
| (not in ETL) | (not in Kusto) | `RunContextState[id=N]` lifecycle | Async op tracking only |

### 4.2 Correlation Workflow

1. **Get the time window**: From test logs (`controller.log`), note start/end timestamps
2. **Pull ETL events**: `read-grts-etl.py --last-minutes N` filtered to that window
3. **Pull Kusto events**: Query PlayFabInternal or UTCEvents for the same user/title/timeframe
4. **Examine PGS state**: Read the context state, version manifest, and extended manifest from `C:\XboxGames\GameSave\pgs\u_{XUID}_{TitleID}\`
5. **Review inproc SDK logs**: Read `device-DeviceA-log.txt` for `[PlayFab]` prefixed lines showing SDK internal function calls, parameters, and return codes
6. **Align by timestamp**: ETL events show GRTS-internal timing; Kusto events show SDK-reported timing; inproc logs show SDK-boundary timing with `elapsed=NNNms` per command
7. **Cross-reference**: Match upload sizes, file counts, error codes, and version numbers across all four sources
8. **Verify PGS ↔ Kusto consistency**: Confirm PGS `UploadVersion` matches Kusto's latest `version_finalized.Version`
9. **Verify inproc ↔ PGS manifests**: Confirm `[ExtManifest:after Upload]` JSON matches PGS extended manifest on disk

### 4.3 What Each Source Tells You That the Others Can't

| Question | Answer Source |
|----------|--------------|
| What HTTP calls did GRTS make? | ETL only (`PFResponse` events) |
| What state was the PFActivator in? | ETL only (state transition events) |
| What was the compression ratio? | Kusto `context_sync` (`syncSizeBytes` / `originalSizeBytes`) |
| Was there a conflict resolution? | GRTS UTCEvents (`data.conflictResolution`) — **not** SDK PlayFabInternal (always empty). Also ETL (`PFActivatorTakeRemote*`) |
| What was the loser version in a conflict? | GRTS UTCEvents only (`data.loserVersion`) |
| How many retries occurred? | Kusto `context_sync_error.retryCount` |
| Did the upload actually complete on the server? | ETL (`PFUploadContextFinalized`) — definitive |
| What's the failure rate across all users? | Kusto only (aggregate queries) |
| Did the game process exit cleanly? | ETL only (`GameProcessStateChange`) |
| What version does the device think it has? | PGS only (`{Version}.json`, `SyncStatus` field) |
| What files are cached locally? | PGS only (extended manifest `Files[]` + blob subdirectory) |
| Is the device in a broken sync state? | PGS only (`SyncStatus=0` in context state file) |
| Does the manifest match what Kusto says was finalized? | Cross-reference PGS version vs Kusto `version_finalized.Version` |
| What parameters did the SDK pass to GRTS? | Inproc logs only (`[PlayFab] PFXPALCallGetFolderWithUiAsync: configRequest.*`) |
| What was the exact async operation lifecycle? | Inproc logs only (`XAsyncOp::Begin` → `XAsyncOp::Cleanup`, `RunContextState` lifecycle) |
| Did the SDK correctly configure callbacks? | Inproc logs only (`SetUiCallbacks: callbacks: progress=set, ...`) |
| What was the UI progress callback sequence? | Inproc logs only (`MyPFXPALGameSaveProgressUiCallback: syncState=N`) |
| What folder did GRTS assign? | Inproc logs only (`PFXPALGetFolderComplete: folder obtained=...`) |
| What SDK version was running? | Inproc logs only (`[GAME SAVE] PFGameSaveFilesInitialize - SDK Version: ...`) |

> **HTTP Data in Inproc Logs:**  
> HTTP call details (request/response bodies, headers, status codes) are visible in inproc logs under the `[HTTPCLIENT]` prefix. These complement ETL's `PFResponse` events and provide SDK-side perspective on HTTP behavior (e.g., client-side timeout handling, connection reuse, header injection).

---

## 5. GRTS On-Disk State (PGS)

GRTS stores all local state under `C:\XboxGames\GameSave\pgs\`. This is the on-disk state data source — it shows what the device currently believes about the user's save state, independent of ETL traces, Kusto telemetry, or inproc logs.

### 5.1 Directory Structure

```text
pgs/
├── t_{XUID}_{TitleID}/                    # Title-level (sync temp data, cleaned after sync)
│   └── {Version}-sync/                    # Download plan for a version
├── u_{XUID}_{TitleID}/                    # User-level (save data + manifests)
│   ├── {XUID}_{TitleID}.json              # Context state: sync status, process info, quotas
│   ├── {Version}.json                     # Version manifest: device, status, upload progress
│   ├── extended-{Version}-manifest.json   # File/folder tree for this version (the key file)
│   └── {Version}/                         # Blob cache: compressed save data files
```

**Real example** from golden path test 01 (user `2814640093565666`, title `E18D7`):

```text
pgs/
└── u_2814640093565666_E18D7/
    ├── 2814640093565666_E18D7.json         (417 bytes)  — context state
    ├── 4434.json                           (295 bytes)  — version manifest
    ├── extended-4434-manifest.json         (314 bytes)  — extended manifest
    └── 4434/                               (empty dir)  — blob cache (cleared after teardown)
```

### 5.2 Context State File (`{XUID}_{TitleID}.json`)

Contains the GRTS session context for this user+title. Updated on every sync operation.

```json
{
  "Context": {
    "Aumid": "41336MicrosoftATG.XboxLiveE2E_dspnxghe87tn0!Game",
    "InitFlags": "8",
    "LastHR": "0",
    "PackageFullName": "41336MicrosoftATG.XboxLiveE2E_1.7.0.0_x64__dspnxghe87tn0",
    "ProcessId": "49708",
    "SaveLocation": "C:\\gamesaves-test\\DeviceA\\",
    "SessionId": "{4030563E-EB99-4209-B4F3-B4799D225957}",
    "SyncErrorCode": "0",
    "SyncStatus": "2",
    "TotalQuota": "1073741824",
    "TotalSize": "0",
    "UploadSize": "0",
    "UploadVersion": "4434"
  }
}
```

| Field | Values | Meaning |
|-------|--------|---------|
| `SyncStatus` | `0`=BROKEN, `1`=SYNCHRONIZED, `2`=NOT_SYNCHRONIZED | Current sync state. `2` after teardown is normal (context was deactivated). |
| `UploadVersion` | Integer string | The version that was last uploaded or synced to. |
| `LastHR` | HRESULT string | Last error code. `"0"` = success. |
| `SyncErrorCode` | Error code string | `"0"` = no sync errors. Non-zero indicates last sync failure. |
| `TotalQuota` | Bytes string | Title quota (1 GB default = `1073741824`). |
| `TotalSize` / `UploadSize` | Bytes string | Disk usage. `"0"` after teardown (blobs were cleaned up). |
| `SessionId` | GUID | GRTS session GUID — correlates to ETL session traces. |
| `SaveLocation` | Path | Where game files are extracted to. |
| `LastConflictWinner` / `LastConflictLoser` | Version strings | Conflict resolution history (absent when no conflicts). |

### 5.3 Version Manifest (`{Version}.json`)

Tracks the state of a specific version — whether it's being uploaded, finalized, or in progress.

```json
{
  "Manifest": {
    "UserId": "2814640093565666",
    "GameId": "E18D7",
    "DeviceId": "{8D41D6C0-8EED-4182-B329-BC8351A89B5E}",
    "Version": "4434",
    "BaseVersion": "4433",
    "Status": 2,
    "Created": "2026-05-09T02:51:30.9800Z",
    "LastWrite": "2026-05-09T02:51:30.9800Z",
    "UploadProgress": { "Size": "0", "Expected": "0" },
    "Chunks": []
  }
}
```

| Field | Values | Meaning |
|-------|--------|---------|
| `Status` | `0`=Init, `1`=Uploading, `2`=Finalized | Version lifecycle state. `2` = upload complete, confirmed by server. |
| `BaseVersion` | Version string | The version this one was based on. Useful for tracking version chains. |
| `UploadProgress.Size/Expected` | Bytes strings | Tracks multi-chunk upload progress. Both `"0"` when finalized. |
| `Chunks` | Array | Pending upload chunks. Empty `[]` when finalized. |
| `DeviceId` | GUID | The device that created this version. |

### 5.4 Extended Manifest (`extended-{Version}-manifest.json`)

**The most important PGS file.** Contains the complete file/folder tree for a version. This is what GRTS uses to know which files exist in the cloud and how to reconstruct them locally.

```json
{
  "v1": {
    "DeviceId": "{8D41D6C0-8EED-4182-B329-BC8351A89B5E}",
    "Version": "4434",
    "Created": "2026-05-09T02:51:29.4400Z",
    "LastModified": "2026-05-09T02:51:29.9520Z",
    "Folders": [
      { "Id": "{0E188D2F-49E8-75DA-0759-6FC7370A4F8B}", "Name": "progress" },
      { "Id": "{F13A23D2-598D-42C5-B5AC-C7E971BAFBEA}", "Name": "roundtrip" }
    ],
    "Files": []
  }
}
```

#### Folders Array

Each folder maps a GUID `Id` to a display `Name`. Files reference folders via `FolderId`.

#### Files Array (when populated)

After an upload, the extended manifest contains file entries with compression details:

```json
{
  "FileId": "{FEDE2621-3CEB-4B9E-A947-084E64FB8083}",
  "Name": "FEDE26213CEB4B9EA947084E64FB8083",
  "CompressSize": 265,
  "Size": 10240,
  "Compression": "zip",
  "LastModified": "2026-05-09T02:51:22.4490Z",
  "Extract": [{
    "FileId": "{A1FD9A0D-0AF3-4E1E-AA2A-5C120EFBDEAB}",
    "Name": "payload.bin",
    "Size": 10240,
    "Created": "2026-05-09T02:51:21.8280Z",
    "LastModified": "2026-05-09T02:51:21.8280Z",
    "SkipFile": false,
    "FolderId": "{0E188D2F-49E8-75DA-0759-6FC7370A4F8B}"
  }]
}
```

| Field | Meaning |
|-------|---------|
| `FileId` | GUID identifying the compressed blob in the cloud. |
| `Name` | Blob filename on disk (GUID without dashes). |
| `CompressSize` | Compressed size in bytes (265 bytes for 10240-byte payload = 97.4% compression). |
| `Size` | Original uncompressed size in bytes. |
| `Compression` | Compression algorithm (`zip`). |
| `Extract[]` | Files extracted from this blob. One blob can contain multiple game files. |
| `Extract[].FolderId` | Links the file to a folder GUID from the `Folders` array. |
| `Extract[].SkipFile` | If `true`, file is marked for deletion in next sync. |

### 5.5 Blob Cache (`{Version}/` subdirectory)

The blob cache stores compressed save data files. After a download-sync, GRTS places compressed blobs here before extracting them to the game's save folder. After teardown (`RemoveUser`), the blob cache may be empty — blobs are cleaned up when the context is released.

### 5.6 Version Consumption Pattern

A typical golden path test consumes ~2-3 version numbers per cycle:
- Upload: `InitManifest` (allocate v4433) → `FinalizeManifest` (commit v4433)
- Re-sync download: `InitManifest` (allocate v4434) → `FinalizeManifest` (commit v4434, re-uploads manifest to confirm download state)

Confirmed from golden path test 01: Version 4432 (pre-existing) → 4433 (upload) → 4434 (download re-sync).

### 5.7 PGS Investigation Workflow

When examining PGS for debugging:

1. **Check `SyncStatus`** — Is it `0` (BROKEN)? That explains sync failures.
2. **Check `LastHR`** — Non-zero means last operation failed.
3. **Read the extended manifest** — Does `Files[]` contain what you expect? Empty = nothing synced.
4. **Compare version to Kusto** — Does the PGS version match the latest `version_finalized.Version` in Kusto?
5. **Check blob cache** — If files are in the extended manifest but not in `{Version}/`, blobs were cleaned up (normal after teardown).
6. **Look for multiple version files** — Leftover `{OldVersion}.json` files may indicate incomplete cleanup or interrupted syncs.

---

## 6. Inproc SDK Debug Logs — How to Read Them

The inproc SDK debug logs are produced by the test harness (`GameTestAppWindows.exe`) and capture every SDK function call, internal state transition, and async operation lifecycle as seen from the game process. These are the **fourth data source** — they show exactly what the SDK did at the API boundary, with full parameter dumps.

> **Game Engine Integration:**  
> Production games (Unreal, Unity, etc.) can emit similar per-frame or per-save debug logging by calling the SDK's verbose logging APIs or by custom integration of the `[PlayFab]` trace sinks. Check game-specific documentation for engine-side game save logging options — they complement SDK inproc logs with game-level context.

### 6.1 Test Output Directory Structure

```text
C:\git\PlayFab.C\Out\gamesave-pc-tests\
├── pass1\                              # First run of a pass batch
│   ├── 01\                             # Test number (gamesave-pc-01-...)
│   │   ├── controller.log              # Full orchestrator log (commands, responses, timing)
│   │   ├── controller-stdout.txt       # Raw stdout from controller process
│   │   ├── controller-stderr.txt       # Stderr from controller (usually empty)
│   │   ├── device-DeviceA-log.txt      # Full device SDK debug log (the key file)
│   │   ├── device-DeviceA-summary.txt  # Filtered device log (no [PlayFab]/[HTTPCLIENT] noise)
│   │   ├── device-DeviceA-fetched-log.txt      # Log fetched from device via GatherLogs
│   │   ├── device-DeviceA-fetched-summary.txt  # Filtered fetched log
│   │   └── test-results.json           # Pass/fail summary with timing
│   ├── 10\                             # Another test
│   │   └── ...
│   └── summary.csv                     # Pass-level summary
├── pass154\                            # Another pass batch
│   └── ...
├── combined-summary.csv                # All-passes aggregate
└── test-history.csv                    # Historical trend data
```

**Naming convention:** `pass{N}` directories contain numbered subdirectories for each test. `N` increments each batch. Test subdirectories use the test scenario number (01, 10, etc.).

### 6.2 Log File Types

| File | Size (typical) | Content | When to Use |
|------|---------------|---------|-------------|
| `controller.log` | 40-60 KB | All 46 commands as JSON, orchestrator decisions, manifest snapshots, step timing | Understanding test flow, checking which step failed, seeing manifest state at each checkpoint |
| `device-DeviceA-log.txt` | 50-60 KB | **Full device output** including `[PlayFab]`, `[HTTPCLIENT]`, `[WEBSOCKET]` internal traces | Deep SDK debugging — the primary inproc log |
| `device-DeviceA-summary.txt` | 20-25 KB | Filtered log: `[Command]` lines, API results, manifest snapshots — no internal SDK noise | Quick scan: did the test pass? What steps ran? |
| `device-DeviceA-fetched-log.txt` | ~same as log | Copy gathered from device via `GatherLogs` command at end of test | Same content, gathered remotely (for Xbox/remote devices) |
| `test-results.json` | <1 KB | JSON summary: runId, start/end times, pass/fail/skip counts | Automation: was this test run clean? |

### 6.3 Log Line Format

Every line in the device log follows this format:

```text
[HH:MM:SS] <message>
[HH:MM:SS]      [PREFIX] <internal message>
```

The indentation and prefix tag indicate the source layer:

| Pattern | Source | What It Contains |
|---------|--------|-----------------|
| `[HH:MM:SS] <msg>` (no indent) | Test harness (GameTestApp) | Command receive/complete, API calls, results |
| `     [PlayFab] <msg>` (6-space indent) | PlayFab SDK internals | Function entry/exit, parameters, state machines, version info |
| `     [HTTPCLIENT] <msg>` | libHttpClient (HC) | HTTP request/response details, WinHTTP callbacks |
| `     [WEBSOCKET] <msg>` | WebSocket layer | WS frame receive confirmations |

### 6.4 Key Log Patterns

#### Command Lifecycle
```text
[19:51:15] [Command] Received command: PFInitialize (id=5473e5da...)
     [PlayFab] RunContextState[id=1]::RunContextState        ← SDK creating context
     [PlayFab] PlayFabCore::PFCoreGlobalState::PFCoreGlobalState
[19:51:15] PFInitialize (hr=0x00000000)                      ← API returned success
[19:51:15] [Command] << PFInitialize >> status=succeeded, hresult=0x00000000, elapsed=18ms
```

**Key fields in `[Command] <<...>>` lines:**
- `status=succeeded|failed` — did the command pass the test assertion?
- `hresult=0x00000000` — the HRESULT from the SDK call (0 = success, 0x8... = error)
- `elapsed=NNNms` — wall-clock time for this command

#### SDK Initialization
```text
     [PlayFab] [GAME SAVE] PFGameSaveFilesInitialize - SDK Version: 2510.2.0.260204
     [PlayFab] PlatformGetPlatformType: Out-of-proc GRTS available -> platformInfo=GRTSAvailable
```

This tells you: which SDK build is running, and whether GRTS (out-of-proc) or inproc path was selected.

#### AddUser With UI (Download/Sync)
```text
     [PlayFab] GameSaveAPIProviderGRTS::AddUserWithUiAsync: options=0
     [PlayFab] PFXPALAddUserBegin: starting user initialization
     [PlayFab] PFXPALAddUserBegin: titleId='E18D7', apiEndpoint='https://E18D7.playfabapi.com'
     [PlayFab] PFXPALCallGetFolderWithUiAsync: configRequest.titleId=E18D7
     [PlayFab] PFXPALCallGetFolderWithUiAsync: configRequest.flags=0x00000008
     [PlayFab] PFXPALCallGetFolderWithUiAsync: configRequest.fileLocation=C:\gamesaves-test\DeviceA\
     [PlayFab] MyPFXPALGameSaveProgressUiCallback: syncState=0    ← NotStarted
     [PlayFab] MyPFXPALGameSaveProgressUiCallback: syncState=1    ← PreparingForDownload
     [PlayFab] MyPFXPALGameSaveProgressUiCallback: syncState=2    ← Downloading
     [PlayFab] MyPFXPALGameSaveProgressUiCallback: syncState=5    ← SyncComplete
     [PlayFab] PFXPALGetFolderComplete: folder obtained='C:\gamesaves-test\DeviceA\', isConnectedToCloud=true
```

**syncState values:** 0=NotStarted, 1=PreparingForDownload, 2=Downloading, 5=SyncComplete. Gaps in the sequence (e.g., missing state 3 or 4) indicate aborted operations.

#### Upload
```text
     [PlayFab] GameSaveAPIProviderGRTS::UploadWithUiAsync: option=1
     [PlayFab] GameSaveAPIProviderGRTS::UploadWithUiAsync: using UploadReleaseActive
     [PlayFab] GameSaveAPIProviderGRTS::UploadWithUiResult: hr=0x00000000
```

Upload logs are sparser than download — the SDK delegates to GRTS and waits. The `option=1` means `ReleaseDeviceAsActive`.

#### Uninitialize / Teardown
```text
     [PlayFab] GameSaveAPIProviderGRTS::UninitializeAsync: cleaned up 1 users
     [PlayFab] PFGameSave::GameSaveGlobalState::CancelAllPendingUIWaits
     [PlayFab] RunContextState[id=0] Termination BLOCKED: 1 pending TaskQueue callbacks (depth=0)
     [PlayFab] RunContextState[id=0] Termination complete, notifying listener
```

The "Termination BLOCKED" message is **normal** — it means the RunContext is waiting for pending callbacks to drain. "Termination complete" confirms clean shutdown.

#### Manifest Snapshots
The test harness captures extended manifest state at key checkpoints:

```text
[19:51:21] [ExtManifest:after AddUserWithUi] (GRTS) C:\XboxGames\GameSave\pgs\u_2814640093565666_E18D7
[19:51:21] [ExtManifest:after AddUserWithUi] {"v1":{"DeviceId":"...","Version":"4433",...,"Files":[]}}
```

These `[ExtManifest:...]` lines dump the PGS extended manifest as inline JSON. The label tells you when: `before AddUserWithUi`, `after AddUserWithUi`, `after Upload`.

#### Warning: Unclosed Handles
```text
     [PlayFab] Warning: Unclosed handles remain during cleanup (PlayFab::HandleTable<...>)
```

This warning fires when `PFServiceConfigCloseHandle` is called AFTER `PFUninitializeAsync`. It's benign — the test intentionally closes handles out of order. **Do NOT treat this as an error.**

### 6.5 Error Detection Patterns

For **failed** tests, look for these patterns:

| Pattern | Meaning |
|---------|---------|
| `status=failed, hresult=0x80004005` | E_FAIL — generic failure |
| `status=failed, hresult=0x80070490` | Element not found |
| `status=failed, hresult=0x800...` | Any non-zero HRESULT = SDK error |
| `errorMessage="HRESULT did not match expectedHr"` | Test expected different HRESULT |
| `Scenario '...' failed:` (in controller.log) | Which command caused the failure |
| `RESULT: N test(s) FAILED` (in controller.log) | Summary line at end |

**Example of a failed upload** (from pass1/10):
```text
[Command] << PFGameSaveFilesUploadWithUiAsync >> status=failed, hresult=0x80004005, elapsed=9632ms
```

Compare to a **successful upload** (from pass154/01):
```text
[Command] << PFGameSaveFilesUploadWithUiAsync >> status=succeeded, hresult=0x00000000, elapsed=2094ms
```

### 6.6 Inproc Log Investigation Workflow

1. **Quick scan**: Open `device-DeviceA-summary.txt` — check all `[Command] <<...>>` lines for `status=failed`
2. **Find the failing step**: Grep for `status=failed` — note the command name, commandId, elapsed time
3. **Get context**: In the full `device-DeviceA-log.txt`, search for the `commandId` to find the `[Command] Received` line, then read all `[PlayFab]` lines between receive and result
4. **Check SDK state**: Look for `[PlayFab]` lines showing function parameters, configRequest fields, syncState progression
5. **Check manifest state**: Look for `[ExtManifest:before ...]` and `[ExtManifest:after ...]` lines — did the manifest change as expected?
6. **Correlate with controller.log**: The controller shows the same command with the full JSON response including `errorMessage`
7. **Cross-reference with other sources**: Match timestamps to ETL, Kusto, and PGS

### 6.7 Debug API: `PFGameSaveFilesSetWriteManifestsToDiskForDebug`

This debug-only API tells the SDK to persist extended manifest snapshots to disk after every sync operation. Without it, manifests exist only in memory during the inproc (Win32) path. GRTS already writes manifests to `C:\XboxGames\GameSave\pgs\`, so the API is a **no-op** on the GRTS provider.

#### API Signature

```cpp
// Source: PlayFabGameSave/Source/Api/PFGameSaveFilesForDebug.h
PF_API PFGameSaveFilesSetWriteManifestsToDiskForDebug(_In_ bool writeManifests);
```

**Parameters:**
- `writeManifests` — `true` to enable manifest-to-disk writing, `false` to disable (default).

**Returns:** `S_OK` on success, `E_FAIL` on exception.

#### Behavior by Provider

| Provider | Effect | Manifest Location |
|----------|--------|-------------------|
| **Win32 (inproc)** | Writes `extended-{version}-manifest.json` after each sync operation | `<saveFolder>\cloudsync\extended-*-manifest.json` |
| **GRTS (out-of-proc)** | **No-op** — logs the call, ignores the parameter | GRTS already writes to `C:\XboxGames\GameSave\pgs\u_{XUID}_{TitleID}\` |

**Inproc `saveFolder`** is the directory passed to `PFGameSaveFilesInitialize` — in tests this is typically `C:\gamesaves-test\DeviceA\`.

#### When to Call

Call **after** `PFGameSaveFilesInitialize` but **before** `AddUserWithUi`. The test harness calls it once per session:

```yaml
# From gamesave-pc-01-single-device-golden-path.yml
- command: PFGameSaveFilesSetWriteManifestsToDiskForDebug
  params:
    writeManifests: true
```

In C++ unit tests:
```cpp
// From PlayFabGameSaveUnitTests/actions.cpp
RIF(PFGameSaveFilesSetWriteManifestsToDiskForDebug(true));
```

#### How the Test Harness Uses It

The test harness has a `LogExtendedManifest(saveFolder, label)` helper (in `PFGameSaveFilesHandlers.cpp`) that reads manifest snapshots from **both** locations:

1. **GRTS path**: Scans `C:\XboxGames\GameSave\pgs\u_*\` for `extended-*-manifest.json`
2. **Inproc path**: Reads `<saveFolder>\cloudsync\extended-*-manifest.json`

This produces the `[ExtManifest:...]` log lines seen in `device-DeviceA-log.txt`:

```text
[19:51:21] [ExtManifest:after AddUserWithUi] (GRTS) C:\XboxGames\GameSave\pgs\u_2814640093565666_E18D7
[19:51:21] [ExtManifest:after AddUserWithUi] {"v1":{"DeviceId":"...","Version":"4433",...}}
```

The label tells you which checkpoint: `before AddUserWithUi`, `after AddUserWithUi`, `after Upload`, etc.

#### Inproc vs GRTS Manifest Comparison

| Aspect | Inproc (`cloudsync/`) | GRTS (`pgs/`) |
|--------|----------------------|---------------|
| **Written automatically** | No — requires `SetWriteManifestsToDiskForDebug(true)` | Yes — always |
| **Location** | `<saveFolder>\cloudsync\extended-{ver}-manifest.json` | `C:\XboxGames\GameSave\pgs\u_{XUID}_{TitleID}\extended-{ver}-manifest.json` |
| **Content format** | Same JSON schema (`v1` envelope with DeviceId, Version, Status, Files) | Same JSON schema |
| **Additional files** | `cloudsync\localstate.json`, `cloudsync\manifests.json` | Context state, version manifest, blob cache |
| **Persists after uninit** | Only while `saveFolder` exists | Survives until `DeleteSaveRoot` or manual cleanup |

#### Debugging Workflow

1. **Enable in test YAML**: Add `PFGameSaveFilesSetWriteManifestsToDiskForDebug` with `writeManifests: true` after `PFGameSaveFilesInitialize`
2. **Check log output**: Search for `[ExtManifest:` in `device-DeviceA-log.txt` to see manifest state at each checkpoint
3. **Compare versions**: The `Version` field in the manifest JSON should match the Kusto `marked_known_good` event's version number
4. **Cross-reference with PGS**: If running GRTS, compare `pgs/` manifests with the `[ExtManifest:after ...]` log output — they should match
5. **Diagnose stale state**: If `Version` doesn't increment after upload, the sync didn't finalize — check Kusto for `version_finalized` event

> **Debug Manifest API in Test YAML:**  
> For test automation and reproducible debugging, add the `PFGameSaveFilesSetWriteManifestsToDiskForDebug` API call to your test YAML recipe (after `PFGameSaveFilesInitialize`). This enables automatic manifest dumps to `device-DeviceA-log.txt` at each checkpoint, eliminating the need for manual disk inspection during debugging.

---

## 7. Golden Path Expected Telemetry Markers

> **See also:** §2.5 for the detailed ETL event flow and §10 for a complete annotated golden path walkthrough with real data from pass154/01.

For xplat test 01 (single-device: write → upload → teardown → re-add → verify download):

### Expected ETL Events
1. `GameProcessStateChange` — game starts
2. `PFXGameSaveServicePrepareContextWithConfig` — GRTS init
3. `PFResponse(PFR_LOGIN)` — HTTP 200
4. `PFResponse(PFR_LIST_MANIFESTS)` — HTTP 200
5. `PFContextCreated` — activation complete
6. `PFUploadWorkerEntered` — upload starts
7. `PFUploadPlanComplete` — plan ready, UploadPendingSize > 0
8. `PFUploadContextUploadChunk` — chunk uploaded, hr=0x0
9. `PFUploadContextFinalized` — manifest committed
10. `PFUploadContextComplete` — upload done, ElapsedMs < 5000
11. Second activation cycle (re-sync from cloud)
12. `PFContextCreated` — download complete
13. `GameProcessStateChange` — game exits

### Expected Kusto Events (PlayFabInternal)
1. `context_activation` — `syncState != NotStarted`, `conflictResolution == CR_NoConflictsExpected`
2. `context_sync` (download) — `syncDownload=true`, `hresult=0x0`
3. `context_sync` (upload) — `syncDownload=false`, `hresult=0x0`, `fileCount > 0`
4. Second `context_activation` — re-sync from cloud
5. Second `context_sync` (download) — `syncDownload=true`, `fileCount > 0`

### Healthy Markers (no issues)
- No `context_activation_failure` events
- No `context_sync_error` events
- All `PFResponse` events have `HttpStatus=200`
- Upload `ElapsedMs` < 5000 for 10KB test data
- `conflictResolution` is always `CR_NoConflictsExpected`
- Version numbers increment by ~3 per cycle

### Unhealthy Markers (problems)
- `context_activation_failure` with non-zero `hresult`
- `PFResponse` with `HttpStatus != 200`
- `PFUploadContextUploadError` events
- `context_sync_error` with non-zero `retryCount`
- Upload `ElapsedMs` > 30000 (30s timeout threshold)
- Missing `PFUploadContextFinalized` (upload didn't complete)

---

## 8. Telemetry Gaps & Known Issues

From the telemetry codebase review:

### Missing Serialization (Bug)
- `ContextActivationEvent::ToJson()` does NOT serialize `baseVersion` or `hresult` — these fields are tracked internally but silently dropped in JSON. PowerBI cannot use them.

### Missing Emit Sites (Gaps)
1. **GAP-1**: Conflict-upload failure during AddUser emits no activation telemetry
2. **GAP-2**: Upload returning `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` emits no sync telemetry
3. **GAP-3**: CompareStep failure during upload has no sync telemetry

### Offline Invisible
- Users who only play offline produce zero telemetry events (no entity pipeline without login)
- `useMocks == true` suppresses all telemetry (test harness mode)

### Platform Type Nuances
| Scenario | `platformType` | In-Proc? |
|----------|---------------|----------|
| PC with GRTS | `Windows` | No |
| PC with `ForceUseInprocGameSaves=1` | `WindowsInproc` | Yes |
| Steam Deck | `SteamDeck` | Yes (always) |
| Xbox Console | `Xbox` | No |

---

## 9. Analysis Checklist

When analyzing a test run's telemetry:

### Step 1 (PRIMARY for test debugging): Check Inproc SDK Logs
- [ ] Open `device-DeviceA-summary.txt` — any `status=failed` lines?
- [ ] Open `device-DeviceA-log.txt` — grep for `[PlayFab]` lines around failures
- [ ] SDK version correct? (`[GAME SAVE] PFGameSaveFilesInitialize - SDK Version: ...`)
- [ ] Platform detection correct? (`PlatformGetPlatformType: ... -> platformInfo=...`)
- [ ] AddUser sync progression complete? (syncState 0→1→2→5)
- [ ] Upload result hr=0x0? (`UploadWithUiResult: hr=...`)
- [ ] RunContextState termination clean? (no stuck "BLOCKED" without "complete")
- [ ] `[ExtManifest:after ...]` JSON consistent with PGS on-disk state?

### Step 2 (Optional — deeper investigation): Get Context
- [ ] What test? (test number, YAML recipe)
- [ ] What user? (XUID, EntityId)
- [ ] What title? (TitleID, SCID)
- [ ] What time window? (from controller.log timestamps)

### Step 3 (Optional — deeper investigation): Pull ETL
- [ ] `read-grts-etl.py --last-minutes N -o trace.txt`
- [ ] Filter to `GameSaveTrace` + `GameFlt` providers
- [ ] Note: PFResponse RequestTypes + HttpStatus codes
- [ ] Note: Any ERROR-level events
- [ ] Note: Upload lifecycle (WorkerEntered → PlanComplete → Chunk → Finalized → Complete)

### Step 4 (Optional — deeper investigation): Pull Kusto
- [ ] Query PlayFabInternal for `playfab.gamesave.internal` events by EntityId + title + time window
- [ ] Query UTCEvents for GameSave events by XUID + time window (if PC/Xbox, not Steam)
- [ ] Note: activation events (success vs failure)
- [ ] Note: sync events (upload vs download, sizes, timing)
- [ ] Note: any `context_sync_error` events

### Step 5 (Optional — deeper investigation): Correlate
- [ ] Align ETL and Kusto timestamps
- [ ] Match upload sizes (ETL `TotalSize` ↔ Kusto `syncSizeBytes`)
- [ ] Match file counts (ETL `FileCount` ↔ Kusto `fileCount`)
- [ ] Match error codes (ETL `hr` ↔ Kusto `hresult`)
- [ ] Verify: ETL `PFUploadContextFinalized` present → Kusto `context_sync` should show success

### Step 6 (Optional — deeper investigation): Check On-Disk State
- [ ] `C:\XboxGames\GameSave\pgs\u_{XUID}_{TitleID}\` — context file, manifests, data
- [ ] Version numbers advanced correctly?
- [ ] `SyncStatus` value makes sense?
- [ ] Extended manifest reflects expected files?

---

## 10. Live Test Data Analysis — Golden Path (pass154/01)

### 10.1 Test Identifiers

| Field | Value |
|---|---|
| Test | xplat-01 — Single-Device Golden Path Sync |
| Run ID | `794b1c650f9e` |
| User ID (XUID) | `2814640093565666` |
| Title ID | `E18D7` |
| Entity ID | `4E9A6B8DEE21F150` |
| Master Player Account | `A67B6574D72C6783` |
| Namespace ID | `624BD2B0B8811A52` |
| Device ID | `{8D41D6C0-8EED-4182-B329-BC8351A89B5E}` |
| Time Window | `2026-05-09T02:51:07Z` → `02:51:29Z` (22 seconds) |
| Outcome | **PASSED** |
| SDK Version | `2510.2.0.260204` |
| Save Folder | `C:\gamesaves-test\DeviceA\` |
| GRTS Manifest Path | `C:\XboxGames\GameSave\pgs\u_2814640093565666_E18D7` |

### 10.2 Test Structure — Two Sessions

The golden path test runs **two sessions** on one device to verify upload and download:

**Session 1 — Upload (steps 1-28)**
1. Initialize SDK stack (XGameRuntime, PF, PFServices, ServiceConfig, TaskQueue)
2. Configure UI auto-responses and callbacks
3. Clean local save folder
4. `PFGameSaveFilesInitialize` → `AddUserWithUiAsync` (initial sync — downloads nothing, creates GRTS context)
5. Write 10KB test file (`progress/payload.bin`, pattern `0xAA 0xBB 0xCC 0xDD`)
6. `PFGameSaveFilesUploadWithUiAsync` (mode=ReleaseDeviceAsActive) — uploads to cloud
7. Clean local save folder (preserveManifest=false)
8. Tear down SDK stack

**Session 2 — Download (steps 29-46)**
1. Re-initialize SDK stack from scratch
2. Reconfigure UI callbacks
3. Clean local save folder
4. `PFGameSaveFilesInitialize` → `AddUserWithUiAsync` (syncs — downloads the file uploaded in session 1)
5. Verify downloaded file matches what was uploaded
6. Clean and tear down

### 10.3 Test Log Timeline (from controller.log + device-DeviceA-log.txt)

```text
SESSION 1 — UPLOAD
───────────────────────────────────────────────────────────────────
02:51:15.611  [1/46] XGameRuntimeInitialize          hr=0x00000000    7ms
02:51:15.664  [2/46] PFInitialize                    hr=0x00000000   18ms
02:51:15.720  [3/46] PFServicesInitialize             hr=0x00000000   19ms
02:51:15.773  [4/46] PFServiceConfigCreateHandle      hr=0x00000000    2ms  (E18D7)
02:51:15.817  [5/46] XTaskQueueCreate                 hr=0x00000000    2ms
02:51:15.862  [6/46] SetUiActiveDeviceContentionAuto  hr=0x00000000    0ms  (SyncLastSavedData)
02:51:15.865  [7/46] AutoNavigateGameSaveUi           hr=0x00000000    3ms
02:51:15.907  [8/46] DeleteLocalFolder                hr=0x00000000    6ms
02:51:15.961  [9/46] DeleteLocalFolder                hr=0x00000000   11ms
02:51:16.036 [10/46] PFGameSaveFilesInitialize        hr=0x00000000   15ms  SDK 2510.2.0.260204
02:51:16.080 [11/46] SetWriteManifestsToDiskForDebug  hr=0x00000000    2ms
02:51:16.129 [12/46] PFGameSaveFilesSetUiCallbacks    hr=0x00000000    3ms
02:51:16.171 [13/46] SetActiveDeviceChangedCallback   hr=0x00000000    3ms
02:51:17.737 [14/46] XUserAddAsync                    hr=0x00000000 1526ms  ← user login
02:51:17.783 [15/46] PFLocalUserCreateHandle          hr=0x00000000    8ms
02:51:21.571 [16/46] PFGameSaveFilesAddUserWithUiAsync hr=0x00000000 3723ms ← initial sync
  · 02:51:20  UiProgress: NotStarted → PreparingForDownload → Downloading → SyncComplete
  · After: 0 dirs, 0 files (nothing to download — clean slate)
  · Manifest: Version=4433, 2 folders (progress, roundtrip), 0 files
02:51:21.625 [17/46] PFGameSaveFilesGetFolder         hr=0x00000000   13ms  → C:\gamesaves-test\DeviceA\
02:51:21.700 [18/46] DeleteSaveRoot (preserveManifest) hr=0x00000000  33ms
02:51:21.855 [19/46] WriteGameSaveData                hr=0x00000000   51ms  ← writes progress/payload.bin (10240 bytes)
02:51:24.028 [20/46] PFGameSaveFilesUploadWithUiAsync hr=0x00000000 2094ms  ← UPLOAD
  · Mode: UploadReleaseActive
  · After: progress/ dir exists, payload.bin extracted (265 bytes compressed, 10240 raw)
  · Manifest: Version=4433, 1 file (FEDE2621...→payload.bin)
02:51:24.136 [21/46] DeleteSaveRoot (no preserve)     hr=0x00000000   61ms  (1 file, 1 dir removed)
02:51:24.226  ...    PFLocalUserCloseHandle            hr=0x00000000
02:51:24.268  ...    XUserCloseHandle                  hr=0x00000000
02:51:24.345  ...    PFGameSaveFilesUninitializeAsync  hr=0x00000000   36ms
02:51:24.564  ...    PFUninitializeAsync               hr=0x00000000   52ms

SESSION 2 — DOWNLOAD
───────────────────────────────────────────────────────────────────
02:51:24.694 [29/46] XGameRuntimeInitialize           hr=0x00000000    1ms
02:51:24.762 [30/46] PFInitialize                     hr=0x00000000   23ms
02:51:24.820 [31/46] PFServicesInitialize              hr=0x00000000   12ms
02:51:24.873 [32/46] PFServiceConfigCreateHandle       hr=0x00000000    2ms
02:51:24.930 [33/46] XTaskQueueCreate                  hr=0x00000000    3ms
02:51:25.041  ...    DeleteLocalFolder ×2              hr=0x00000000
02:51:25.171 [38/46] PFGameSaveFilesInitialize         hr=0x00000000   18ms
02:51:25.857 [42/46] XUserAddAsync                     hr=0x00000000  476ms  ← faster (cached)
02:51:25.908 [43/46] PFLocalUserCreateHandle           hr=0x00000000    5ms
02:51:27.871 [44/46] PFGameSaveFilesAddUserWithUiAsync hr=0x00000000 1868ms ← DOWNLOAD sync
  · Before: 0 dirs, 0 files
  · After: progress/ dir, payload.bin downloaded
  · Manifest: Version=4434, 1 file (same payload.bin from upload)
02:51:27.932 [45/46] PFGameSaveFilesGetFolder          hr=0x00000000   15ms
02:51:28.070 [46/46] DeleteSaveRoot (no preserve)      hr=0x00000000   79ms  (1 file, 1 dir removed)
02:51:28.594  ...    Cleanup complete
02:51:28     PASSED
```

### 10.4 Kusto Events — PlayFabInternal

**Query used:**
```kql
['events.all']
| where Timestamp between (datetime(2026-05-09T02:50:00Z) .. datetime(2026-05-09T02:53:00Z))
| where EntityLineage_title == 'E18D7'
| where Entity_Id == '4E9A6B8DEE21F150'
| project Timestamp, FullName_Name, FullName_Namespace, EventData
| order by Timestamp asc
```

**Results — 6 events total:**

| # | Timestamp (UTC) | Event | Namespace | Key Fields |
|---|---|---|---|---|
| 1 | 02:51:19.053 | `entity_logged_in` | `com.playfab` | Session 1 login |
| 2 | 02:51:23.712 | `gamesave_version_marked_known_good` | `playfab.gamesave` | Version=**4432** |
| 3 | 02:51:23.895 | `gamesave_version_finalized` | `playfab.gamesave` | Version=**4433**, 2 files, 1015 bytes, PC, XBoxLive |
| 4 | 02:51:26.362 | `entity_logged_in` | `com.playfab` | Session 2 login |
| 5 | 02:51:30.787 | `gamesave_version_marked_known_good` | `playfab.gamesave` | Version=**4433** |
| 6 | 02:51:30.787 | `gamesave_version_finalized` | `playfab.gamesave` | Version=**4434**, 1 file, 314 bytes, PC, XBoxLive |

**Notable observations:**

1. **No SDK `context_activation` or `context_sync` events** — These are emitted by the SDK client via UTCEvents, but the GRTS out-of-proc path on PC routes through the Gaming Services service, which does NOT relay SDK telemetry to PlayFabInternal. The SDK telemetry events require the UTCEvents pipeline, which this cluster doesn't have (verified: `UTCEvents` table does not exist in the `Gaming` database on `gaming.westus`).

2. **Server-side events only** — All `playfab.gamesave.*` events are emitted by the PlayFab **backend** (Originator.Id = "playfab", Type = "service"), not by the client SDK. This is a critical learning: for GRTS-based PC tests, you see server events but not client SDK telemetry.

3. **Version progression**: 4432 → marked known good → 4433 finalized (upload) → 4433 marked known good → 4434 finalized (download sync). Each sync cycle consumes one version: the `marked_known_good` event fires for the *previous* version, then `version_finalized` fires for the *new* version created.

4. **File count discrepancy**: Session 1 finalized shows "2 files, 1015 bytes" — but the test only wrote 1 file (payload.bin, 10240 bytes). The server's file count represents *manifest-level files* (including compressed containers and metadata), not the game's logical file count. The 1015 bytes is the compressed+metadata size, not the raw 10240 bytes.

### 10.5 Kusto Event Deep Dive

#### Event 2: `gamesave_version_marked_known_good` (Session 1 upload)
```json
{
  "Payload": {
    "Version": 4432,
    "MarkedAt": "2026-05-09T02:51:23.6948444Z"
  }
}
```
> The server marks version 4432 (the *pre-upload* version) as "known good" — meaning the client confirmed it synced successfully before starting a new upload. This is the server's way of tracking that the client has a consistent view.

#### Event 3: `gamesave_version_finalized` (Session 1 upload)
```json
{
  "Payload": {
    "Version": 4433,
    "DeviceType": "PC",
    "TotalSizeBytes": 1015,
    "TotalFileCount": 2,
    "NewFileCount": 2,
    "NewFilesSizeBytes": 1015,
    "FinalizedAt": "2026-05-09T02:51:23.6948444Z",
    "PlayerIdentityProvider": "XBoxLive",
    "IsGeneratedByRollback": false
  }
}
```
> The upload completed. Version 4433 is now the latest. `DeviceType=PC`, `PlayerIdentityProvider=XBoxLive` (GDK user auth). `IsGeneratedByRollback=false` — this is a normal upload, not a rollback recovery.

#### Event 6: `gamesave_version_finalized` (Session 2 download)
```json
{
  "Payload": {
    "Version": 4434,
    "DeviceType": "PC",
    "TotalSizeBytes": 314,
    "TotalFileCount": 1,
    "NewFileCount": 1,
    "NewFilesSizeBytes": 314,
    "FinalizedAt": "2026-05-09T02:51:30.7686172Z",
    "PlayerIdentityProvider": "XBoxLive",
    "IsGeneratedByRollback": false
  }
}
```
> Session 2's AddUserWithUi sync created a new version (4434). The server sees the download-sync as a new finalized version because GRTS re-uploads a manifest after syncing to confirm the device's state. TotalFileCount dropped from 2→1, TotalSizeBytes from 1015→314 — reflecting a re-sync with fewer manifest artifacts.

### 10.6 Correlation: Test Logs ↔ Kusto Timeline

```text
Time (UTC)       Test Log Event                       Kusto Event
─────────────    ──────────────────────────────────    ──────────────────────────────
02:51:15.611     XGameRuntimeInitialize                 —
02:51:17.737     XUserAddAsync complete (1526ms)         —
02:51:19.053      —                                    entity_logged_in (session 1)
02:51:21.571     AddUserWithUiAsync complete (3723ms)    —
  ├ 02:51:20     UiProgress: PreparingForDownload        —
  ├ 02:51:20     UiProgress: Downloading                 —
  └ 02:51:21     UiProgress: SyncComplete                —
02:51:21.855     WriteGameSaveData (payload.bin)         —
02:51:24.028     UploadWithUiAsync complete (2094ms)     —
02:51:23.712      —                                    gamesave_version_marked_known_good (v4432)
02:51:23.895      —                                    gamesave_version_finalized (v4433)
02:51:24.694     Session 2 starts                        —
02:51:25.857     XUserAddAsync complete (476ms)          —
02:51:26.362      —                                    entity_logged_in (session 2)
02:51:27.871     AddUserWithUiAsync complete (1868ms)    —
  └              Downloads payload.bin to local           —
02:51:30.787      —                                    gamesave_version_marked_known_good (v4433)
02:51:30.787      —                                    gamesave_version_finalized (v4434)
02:51:28.070     DeleteSaveRoot + cleanup                —
02:51:28         PASSED                                  —
```

**Key correlation insights:**

1. **Login precedes sync**: `entity_logged_in` fires ~1.5s after `XUserAddAsync` completes — the login goes through PlayFab authentication before GRTS context creation.

2. **Upload timing**: `UploadWithUiAsync` finishes at 02:51:24.028 (local time), but server sees the version_finalized at 02:51:23.895 — the server events fire *during* the upload, not after. The client's completion callback fires ~130ms after the server recorded it.

3. **Download creates a version**: The session 2 `AddUserWithUiAsync` (download) triggers `version_finalized` on the server at 02:51:30.787, even though the *client* completed at 02:51:27.871. The ~3 second gap suggests the server processes version finalization asynchronously after the client's sync completes.

4. **No client SDK telemetry**: The 5 SDK telemetry events (`context_activation`, `context_sync`, etc.) are absent from PlayFabInternal. They would only appear in UTCEvents, which requires the Windows Telemetry pipeline. For GRTS PC testing, you only see **server-side gamesave events** and **login events**.

### 10.7 GRTS On-Disk Manifest Progression

The device-log shows ExtManifest snapshots at key moments:

**After Session 1 AddUserWithUi (initial sync — no files to download):**
```json
{
  "v1": {
    "DeviceId": "{8D41D6C0-8EED-4182-B329-BC8351A89B5E}",
    "Version": "4433",
    "Folders": [
      { "Id": "{0E188D2F-49E8-75DA-0759-6FC7370A4F8B}", "Name": "progress" },
      { "Id": "{F13A23D2-598D-42C5-B5AC-C7E971BAFBEA}", "Name": "roundtrip" }
    ],
    "Files": []
  }
}
```

**After Session 1 Upload:**
```json
{
  "v1": {
    "Version": "4433",
    "Files": [{
      "FileId": "{FEDE2621-3CEB-4B9E-A947-084E64FB8083}",
      "CompressSize": 265,
      "Size": 10240,
      "Compression": "zip",
      "Extract": [{
        "Name": "payload.bin",
        "Size": 10240,
        "FolderId": "{0E188D2F-49E8-75DA-0759-6FC7370A4F8B}"
      }]
    }]
  }
}
```

**After Session 2 Download (AddUserWithUi):**
```json
{
  "v1": {
    "Version": "4434",
    "Files": [{
      "FileId": "{FEDE2621-3CEB-4B9E-A947-084E64FB8083}",
      "CompressSize": 265,
      "Size": 10240,
      "Compression": "zip",
      "Extract": [{ "Name": "payload.bin", "Size": 10240 }]
    }]
  }
}
```

**Manifest learning**: The manifest version increments even when the *same file* is re-synced (4433→4434). The file content is identical (same FileId, same sizes). The version bump is because each device-registration/sync creates a new version to track which device has the latest state.

### 10.8 ETL Capture — SOLVED ✅

**Status:** ETL flush and capture is fully working. Validated across 7 experiment phases (pass156–159) with zero data loss.

#### The Solution

```powershell
# 1. Flush ETW + OS file buffers (works WITHOUT admin)
logman update GamingServices -ets -fd

# 2. Copy ETL files to test output (insurance against circular rotation)
copy C:\Windows\System32\LogFiles\WMI\GamingServices*.etl <test-output-dir>

# 3. Decode from copies (self-contained snapshot)
py grts-read-etl.py --no-flush --etl-dir <test-output-dir> --last-minutes 10 -o grts-etl-trace.txt
```

#### Key Facts

| Finding | Detail |
|---------|--------|
| **Admin required?** | **NO** — `logman update -ets -fd` succeeds as standard user |
| **Events per test run** | ~46–50 PF events, covering full upload pipeline |
| **`logman flush` (simpler command)** | **BROKEN** — fails with error `-2147024809`. Always use `logman update -ets -fd` |
| **GRTS restart needed?** | **NO** — flush-only is sufficient |
| **`read-grts-etl.py` built-in flush** | Already flushes by default (unless `--no-flush`) |

#### Why Copy-Then-Decode?

By flushing first, copying ETL files, then decoding from copies with `--etl-dir`:
- The snapshot is **self-contained** — immune to concurrent GRTS activity
- Subsequent test runs won't overwrite via circular rotation
- Each test's ETL data is preserved alongside its other logs

> **Full experiment report:** See `investigations/etl-flush-experiment-results.md` for raw data, GRTS restart behavior, and the complete results across 7 phases.

### 10.8a ETL Capture — Xbox Console

PC ETL (§10.8) lives at `C:\Windows\System32\LogFiles\WMI\GamingServices.etl` and can be flushed and read locally. Xbox consoles require **remote capture** via `xbdiagcap`.

#### The Solution

```powershell
# 1. Trigger diagnostic capture on the Xbox (runs on the console, captures ETL + more)
xbrun /x /system /o xbdiagcap

# 2. Create a local staging directory
mkdir c:\temp\diag
cd /d c:\temp\diag

# 3. Copy captured diagnostics from the Xbox temp folder
xbcp xT:\Windows\Temp\xbdiag_capture\*

# 4. The GRTS ETL file is:
#    xbdiag_auto_AppPlat.etl
#
# Decode it with grts-read-etl.py (same tool as PC, point at the local copy):
py Utilities\Scripts\grts-read-etl.py --no-flush --etl-path c:\temp\diag\xbdiag_auto_AppPlat.etl -o xbox-etl-trace.txt
py Utilities\Scripts\grts-read-etl.py --no-flush --etl-path c:\temp\diag\xbdiag_auto_AppPlat.etl --json -o xbox-etl-trace.json
```

#### Key Facts

| Finding | Detail |
|---------|--------|
| **Tool** | `xbdiagcap` (built into Xbox dev kit tools) |
| **Requires xbrun?** | Yes — runs on the console via `xbrun /x /system /o` |
| **Output location on Xbox** | `T:\Windows\Temp\xbdiag_capture\` |
| **GRTS ETL file** | `xbdiag_auto_AppPlat.etl` — contains GamingServices / AppPlatform events |
| **Copy to PC** | `xbcp xT:\Windows\Temp\xbdiag_capture\*` copies all captured files |
| **Decoding** | Same `grts-read-etl.py` tool with `--etl-path` pointing to the local `.etl` copy |

#### When to Use

- Debugging GRTS errors that only reproduce on Xbox (e.g., `CS_E_OUT_OF_LOCAL_STORAGE` during activation)
- Investigating connected storage partition issues (`\\?\GLOBALROOT\Device\HarddiskX\PartitionY`)
- Tracing activation sync state machine flow on Xbox hardware
- Any time you need the same ETL event detail on Xbox that §10.8 provides on PC

#### Xbox ETL Decoding Notes

The Xbox ETL decodes on PC using the same `grts-read-etl.py` tool. However, there are important differences from PC ETL:

| Aspect | PC ETL | Xbox ETL (via xbdiagcap) |
|--------|--------|--------------------------|
| **ETL file** | `GamingServices.etl` | `xbdiag_auto_AppPlat.etl` |
| **Also reads** | Local GamingServices session only | May pull in local PC ETL files too if in same decode pass — use positional arg to decode only the Xbox file |
| **Event names** | Decoded via `name` field (e.g., `PFContextCreated`) | Same — `name` field works |
| **Event data** | In `props` dict with named fields | Same — `props` dict with named fields |
| **Provider manifest** | Registered locally on PC | Events decode correctly even though Xbox provider manifests aren't registered on PC — the ETL embeds enough metadata |

**Tip:** When decoding, pass only the Xbox ETL file as a positional argument to avoid mixing in local PC events:
```powershell
py grts-read-etl.py --no-flush --json -o xbox-trace.json c:\temp\diag\xbdiag_auto_AppPlat.etl
```

### 10.8b Xbox Connected Storage Partition Architecture

On Xbox, `PFGameSaveFilesGetFolder` returns a path like `\\?\GLOBALROOT\Device\Harddisk4\Partition1` — this is the game-visible mount of the connected storage XVD. GRTS internally manages data on a **different** partition (e.g., `\\?\GLOBALROOT\Device\Harddisk17\Partition1\pgs\u_{userId}_{gameId}\{version}\`).

| Path | What It Is | Writable by Game? |
|------|-----------|-------------------|
| `\\?\GLOBALROOT\Device\Harddisk4\Partition1` | Game-visible connected storage mount (returned by `GetFolder`) | **No** — direct filesystem writes (e.g., `CreateFile`, `fs::create_directories`) return `0x80070005` (Access Denied) |
| `\\?\GLOBALROOT\Device\Harddisk17\Partition1\pgs\...` | GRTS internal PGS data directory | No — managed exclusively by GRTS service |

**Key insight:** On Xbox, the game's save data must go through the GameSave API — you cannot write files directly to the connected storage partition the way you can on PC-GRTS (where the save folder is a regular directory like `C:\gamesaves-test\`). Any test code that writes directly to the `GetFolder` path (e.g., `DoChaosMode` filesystem operations) must run on PC-GRTS, not Xbox.

**Common mistake:** Pinning the chaos-writer device to `engine: xbox` causes `DoChaosMode` (which writes files directly to the save folder) to fail with `Access is denied (0x80070005)`. The chaos-writer role must always run on PC-GRTS.

### 10.9 PGS On-Disk State Analysis

The PGS directory provides the **ground-truth** of what the device believes after the test completed. Examining `C:\XboxGames\GameSave\pgs\u_2814640093565666_E18D7\` reveals the final state:

#### Files Present on Disk

| File | Size | Last Modified | Description |
|------|------|---------------|-------------|
| `2814640093565666_E18D7.json` | 417 bytes | 2026-05-08 19:51:30 | Context state |
| `4434.json` | 295 bytes | 2026-05-08 19:51:30 | Version manifest (v4434) |
| `extended-4434-manifest.json` | 314 bytes | 2026-05-08 19:51:29 | Extended manifest (v4434) |
| `4434/` | (empty) | 2026-05-08 19:51:27 | Blob cache (cleaned after teardown) |

**Key observations:**
- Only v4434 files remain — no leftover v4432 or v4433 manifests (clean state).
- Blob cache (`4434/`) is empty because the test ran `RemoveUser` during teardown, which cleans up extracted blobs.
- No `t_` (title-level temp) directory exists — sync temp data was cleaned up after successful sync.

#### Context State Analysis

```json
{
  "SyncStatus": "2",        // NOT_SYNCHRONIZED — normal after RemoveUser teardown
  "LastHR": "0",            // No errors
  "SyncErrorCode": "0",     // No sync errors
  "UploadVersion": "4434",  // Matches Kusto's latest version_finalized
  "TotalSize": "0",         // Blobs cleaned up after teardown
  "UploadSize": "0"         // Nothing pending upload
}
```

`SyncStatus=2` (NOT_SYNCHRONIZED) after teardown is **expected** — the test called `RemoveUser` which deactivates the context. On next `AddUserWithUi`, GRTS will re-sync from cloud.

#### Extended Manifest: Post-Teardown vs Mid-Test

The current on-disk extended manifest (v4434) has **empty Files[]** — this is the teardown state. Compare with the manifest captured by the test harness *during* the test:

| Moment | Version | Files[] | Source |
|--------|---------|---------|--------|
| After Session 1 `AddUserWithUi` | v4433 | `[]` (fresh init, nothing in cloud) | device-DeviceA-log.txt line 184 |
| After Session 1 `Upload` | v4433 | `[payload.bin: 10240→265 zip]` | device-DeviceA-log.txt line 239 |
| Before Session 2 `AddUserWithUi` | v4433 | `[payload.bin: 10240→265 zip]` | device-DeviceA-log.txt line 458 |
| After Session 2 `AddUserWithUi` (download) | v4434 | `[payload.bin: 10240→265 zip]` | device-DeviceA-log.txt line 492 |
| **On-disk now** (post teardown) | v4434 | `[]` (cleaned up) | `extended-4434-manifest.json` |

The extended manifest **lost its Files[] entries** during teardown — this is normal. GRTS clears the file list when the context is deactivated via `RemoveUser`. On next activation, it re-syncs from the cloud and rebuilds the file list.

#### Cross-Reference: PGS ↔ Kusto Consistency Check

| Check | PGS Value | Kusto Value | Match? |
|-------|-----------|-------------|--------|
| Latest version | `UploadVersion=4434` | `version_finalized.Version=4434` | ✅ |
| Device ID | `{8D41D6C0-8EED-4182-B329-BC8351A89B5E}` | `version_finalized.DeviceType=Win32` | ✅ (same device, different representation) |
| Base version chain | `BaseVersion=4433` | `marked_known_good.Version=4433` | ✅ (known_good marks the base) |
| Error state | `LastHR=0`, `SyncErrorCode=0` | No error events | ✅ |

**PGS learning**: The PGS context state and Kusto server events agree on version 4434 as the latest finalized version. The `BaseVersion` field in PGS's version manifest (4433) corresponds to the version that Kusto's `marked_known_good` event confirmed — creating a verifiable chain: v4433 was good → v4434 was built on top of it.

### 10.10 Summary of What This Golden Path Teaches

| What to look for | Where to find it | What "healthy" looks like |
|---|---|---|
| User login success | Kusto: `entity_logged_in` | 1 event per session, entity type = `title_player_account` |
| Upload success | Kusto: `gamesave_version_finalized` | `IsGeneratedByRollback=false`, increasing version |
| Download success | Test logs: `AddUserWithUiAsync` hr=0x00000000 | After sync: files present in save folder |
| Version consistency | Kusto: `marked_known_good` + `finalized` pair | known_good version = finalized version - 1 |
| File integrity | GRTS manifest: CompressSize, Size fields | Sizes match what was written |
| Timing sanity | Correlation table (§10.6) | Server events within ~3s of client completion |
| No rollbacks | Kusto: `IsGeneratedByRollback` | Always `false` for normal operations |
| No errors | Test logs: all hresult=0x00000000 | No `0x8...` error codes anywhere |
| PGS version matches server | PGS `UploadVersion` = Kusto `version_finalized.Version` | Versions agree across all three sources |
| PGS sync state healthy | PGS `SyncStatus` ≠ 0 | `0` (BROKEN) means sync is stuck |
| No orphaned version files | Only latest version's files in PGS | Leftover old versions = incomplete cleanup |
| SDK init clean | Inproc: `PFGameSaveFilesInitialize` hr=0x0 | SDK version logged, platform detected correctly |
| Sync callbacks complete | Inproc: syncState progression 0→1→2→5 | No gaps in callback sequence |
| Upload SDK handoff clean | Inproc: `UploadWithUiResult: hr=0x00000000` | No E_FAIL or timeout |
| Teardown clean | Inproc: `RunContextState[id=N] Termination complete` | All RunContexts terminated, no stuck BLOCKED |
| Manifest snapshots consistent | Inproc: `[ExtManifest:after ...]` JSON | Version, Files[], folders match PGS on-disk |

### 10.11 Inproc SDK Debug Log Analysis

The inproc SDK logs from `device-DeviceA-log.txt` provide the most detailed view of what the SDK did at each step. Here is the complete golden path flow as seen through `[PlayFab]` prefixed lines.

#### Session 1 — Upload Path

**SDK Initialization** (19:51:15-19:51:16):
- `PFInitialize`: Creates `PFCoreGlobalState`, `RunContextState[id=0..2]` — 18ms
- `PFServicesInitialize`: Creates `RunContextState[id=3..4]` — 19ms
- `PFGameSaveFilesInitialize`: Logs **SDK Version: 2510.2.0.260204**, creates `RunContextState[id=5..6]` — 15ms
- Platform detection: `IsOutOfProcGRTSAvailable: true` → `platformInfo=GRTSAvailable` (using out-of-proc GRTS, not inproc)

**GameSave Configuration** (19:51:16):
- `SetWriteManifestsToDiskForDebug(writeManifests=1)` — enables manifest snapshots in logs
- `SetUiCallbacks`: progress=set, syncFailed=set, activeDeviceContention=set, conflict=set, outOfStorage=set
- `SetActiveDeviceChangedCallback`: callback=provided

**AddUser + Sync** (19:51:17-19:51:21, 3723ms total):
- `PFXPALAddUserBegin`: titleId=E18D7, apiEndpoint=https://E18D7.playfabapi.com
- `PFXPALCallGetFolderWithUiAsync`: configRequest.flags=0x00000008, fileLocation=C:\gamesaves-test\DeviceA\
- Sync progress callbacks: syncState 0→1→2→5 (NotStarted → PreparingForDownload → Downloading → SyncComplete)
- `PFXPALGetFolderComplete`: folder obtained, isConnectedToCloud=**true**
- Result: hr=0x00000000, version 4433, no files (empty save at start)

**Upload** (19:51:21-19:51:24, 2094ms total):
- `UploadWithUiAsync: option=1` → using `UploadReleaseActive` mode
- `UploadWithUiResult: hr=0x00000000`
- Extended manifest after upload: Version=4433, Files=[{payload.bin, CompressSize=265, Size=10240}]

**Teardown** (19:51:24):
- `UninitializeAsync: cleaned up 1 users`
- `RunContextState[id=0] Termination BLOCKED: 1 pending TaskQueue callbacks` → normal
- `RunContextState[id=0] Termination complete` → clean exit
- `Warning: Unclosed handles remain during cleanup` → benign (test-ordering artifact)

#### Session 2 — Download/Re-sync Path

**Re-initialization** (19:51:24-19:51:25):
- New `PFCoreGlobalState` with `RunContextState[id=7..13]`
- Same SDK version, same GRTS detection

**AddUser + Download** (19:51:25-19:51:27, 1868ms total):
- `[ExtManifest:before AddUserWithUi]`: Version=4433, Files=[payload.bin] — **previous upload's manifest still on disk**
- Same XPAL flow: titleId=E18D7, flags=0x00000008
- No progress callbacks visible (sync was fast — server already had this device's data)
- `PFXPALGetFolderComplete`: folder obtained, isConnectedToCloud=**true**
- `[ExtManifest:after AddUserWithUi]`: Version=**4434**, Files=[payload.bin] — **version bumped by download re-sync**
- Save folder after: 1 dir (progress), 0 files at root — the file was downloaded into `progress/payload.bin`

**Teardown** (19:51:28):
- Same clean pattern: `UninitializeAsync` → `Termination BLOCKED` → `Termination complete`
- `Warning: Unclosed handles` — benign

#### Key Observations from Inproc Logs

1. **RunContextState IDs increment monotonically** — Session 1 used ids 0-6, Session 2 used 7-13. Useful for distinguishing sessions in mixed logs.
2. **syncState callback sequence** — Session 1 showed all 4 states (0,1,2,5). Session 2 showed none (or they were batched) — download was instant because data existed.
3. **Version progression visible in manifest snapshots** — 4433 (after initial sync) → 4433 (after upload, same version with files added) → 4434 (after re-sync download).
4. **`isConnectedToCloud=true`** — Both sessions confirmed cloud connectivity. If this were `false`, all subsequent operations would be offline-only.
5. **`configRequest.flags=0x00000008`** — This flag combination means the SDK is using the GRTS-managed path (not inproc).
6. **Upload elapsed=2094ms vs AddUser elapsed=3723ms/1868ms** — Upload is comparable to initial sync time. Second sync (with existing data) is 2× faster.

---

## 11. Debugging Recipes — Known Bug Patterns

This section provides recipes for diagnosing known failure modes discovered during live testing and user investigation. Each recipe includes symptoms, first-look checklist, deep-dive telemetry analysis, and known telemetry gaps.

<!-- NOTE: Recipes 11.1–11.2 were removed after review (content was inaccurate). Numbering starts at 11.1 with the remaining validated recipes. -->

---

### 11.1 Cross-Platform Conflict Debugging

**Overview:**  
When the same player syncs game saves across different platforms (e.g., PC with SDK + Xbox with GRTS), conflicts are common if both devices modified files locally. Debugging requires correlating ETL (platform-specific) and Kusto (unified telemetry) to identify which platform's changes are winning.

**Symptoms:**
- Multi-platform player reports: "I saved on PC, then Xbox didn't get my changes" or vice versa.
- Version numbers advance on both platforms, but file contents don't match.
- One platform shows older data than the other.
- Device contention UI appears on second platform, but user didn't manually trigger conflict resolution.

**First Look:**

Check for these indicators:
```text
Kusto (PlayFabInternal):
  1. Two context_activation events with different DeviceType
  2. Timestamps within 5-30 minutes of each other
  3. Second event: conflictResolution == CR_Conflict

ETL:
  1. (PC side): PFContextCreated, then PFUploadWorkerEntered, then upload cycle
  2. (Xbox side): PFActivatorTakeRemoteLoserUploadDisabled event
  3. (PC side): No PFUploadContextFinalized if Xbox took remote

On-Disk:
  1. (PC): C:\XboxGames\GameSave\pgs\u_{XUID}_{TITLE_ID}\manifest (version N)
  2. (Xbox): Similar path, manifest (version N+1 or N+2)
  3. Version mismatch between devices
```

**Deep Dive Telemetry Analysis:**

1. **Identify platform and timing:**
   ```kql
   ['events.all']
   | where Timestamp > ago(24h)
   | where EntityLineage_title == 'YOUR_TITLE'
   | where Entity_Id == 'PLAYER_ID'
   | where FullName_Namespace == 'playfab.gamesave.internal'
   | where FullName_Name == 'context_activation'
   | project Timestamp, EventData, DeviceType=EventData.Payload.DeviceType, Conflict=EventData.Payload.conflictResolution
   | order by Timestamp asc
   ```

2. **Align uploads with activations:**
   - Extract version finalized events:
     ```kql
     ['events.all']
     | where FullName_Name == 'gamesave_version_finalized'
     | where Timestamp between (datetime('FIRST_PLATFORM_TIME') .. datetime('SECOND_PLATFORM_TIME'))
     | project Timestamp, Version=EventData.Payload.Version, DeviceType=EventData.Payload.DeviceType
     | order by Timestamp asc
     ```
   - **Expected pattern:** Platform A uploads Version N, Platform B activates, detects conflict, Platform B uploads Version N+1 (or takes remote).

3. **Determine conflict winner:**
   - Check manifest `PlatformId` or metadata on server for Version N and N+1.
   - Cross-reference with player's file timestamps (if available via game telemetry).
   - **If Platform B's Version N+1 exists but wasn't initiated by Platform B: Platform B took remote (Platform A lost).**
   - **If Platform A initiated N, Platform B initiated N+1: conflict occurred, B won.**

4. **Trace file content divergence:**
   - Query uploaded sizes and file hashes (if logged in game telemetry):
     ```kql
     ['your_game_telemetry']
     | where EventName == 'FileSaved' or 'FileUploaded'
     | where PlayerID == 'PLAYER_ID'
     | where Timestamp within the conflict window
     | project Timestamp, Filename, FileHash, PlatformType
     ```
   - Compare hashes across platforms and versions.
   - **If Platform A's file hash differs from Platform B's: files diverged, explaining conflict.**

**Known Telemetry Gaps:**
- Kusto does not log which platform's changes "won" in conflict resolution—must infer from version metadata.
- ETL is platform-specific; PC ETL won't show Xbox activity and vice versa.
- File content hashes not logged in PlayFab telemetry; must rely on game-specific telemetry.
- No correlation ID linking PC ETL to Xbox ETL for same activation (platform-siloed logging).

**Mitigation Checklist:**
- [ ] Identify both platforms' activation timestamps.
- [ ] Confirm second platform detected conflict (context_activation.conflictResolution != CR_NoConflictsExpected).
- [ ] Confirm versions advanced appropriately on both platforms.
- [ ] Check which platform's version is latest on server.
- [ ] Verify files match between player's expectations and server state.
- [ ] Determine if conflict resolution was automatic (TakeRemote) or required UI (wait for player decision).

---

### 11.2 Active Device Contention

**Overview:**  
When multiple devices try to sync the same player's saves *simultaneously* or in rapid succession, the platform may detect "contention" and force conflict resolution. This is intended behavior but can lead to unexpected UI or data loss if devices have very different versions or if one device has stale metadata.

**Symptoms:**
- Player has two devices physically connected to network simultaneously.
- Activates SDK on both within seconds of each other (or one is always running, other joins).
- Sees device contention UI on one device (or both).
- One device's changes don't appear on the other; opposite device's version is selected.
- OR: One device hangs during contention resolution (UI stuck, user force-closes app).

**First Look:**

Check for these indicators:
```text
ETL (either device):
  1. Two PFContextCreated events with DIFFERENT PID (if both processes running) or same Device ID
  2. Between them: PFActivatorTakeRemote or PFActivatorTakeLoser marker
  3. Timestamps within 5-15 seconds

Kusto (PlayFabInternal):
  1. Two context_activation events on same player, different devices, within 10 seconds
  2. Both show conflictResolution != CR_NoConflictsExpected

Kusto (UTCEvents, Xbox/PC):
  1. Look for Device Contention event (if game logs it)
  2. Check timing relative to context_activation
```

**Deep Dive Telemetry Analysis:**

1. **Identify simultaneous activations:**
   ```kql
   ['events.all']
   | where Timestamp > ago(24h)
   | where EntityLineage_title == 'YOUR_TITLE'
   | where Entity_Id == 'PLAYER_ID'
   | where FullName_Name == 'context_activation'
   | project Timestamp, DeviceType=EventData.Payload.DeviceType, SyncState=EventData.Payload.syncState
   | order by Timestamp asc
   ```
   
   Look for: Two activations within 15 seconds, different DeviceType values.

2. **Check conflict detection mode:**
   - Extract `conflictResolution` from both context_activation events.
   - **If both show CR_Conflict: both devices detected conflict (high contention).**
   - **If one shows CR_Conflict, other shows CR_NoConflictsExpected: second device avoided conflict (TakeRemote worked).**

3. **Trace version selection:**
   - Query versions finalized during this window:
     ```kql
     ['events.all']
     | where FullName_Name == 'gamesave_version_finalized'
     | where Timestamp between (datetime('FIRST_ACTIVATION') .. datetime(FIRST_ACTIVATION) + 2m)
     | project Timestamp, Version=EventData.Payload.Version, DeviceType=EventData.Payload.DeviceType
     ```
   - **Expected pattern:** One device uploads Version N, second device's upload creates Version N+1 (or takes remote).

4. **Analyze contention resolution UI:**
   - If player saw UI: check for game-level telemetry on which option they chose (TakeLocal vs TakeRemote vs Merge).
   - Correlate choice with resulting version metadata and file contents.
   - **If player chose TakeLocal but other device's changes appear: UI decision was reversed (likely bug or connectivity glitch).**

5. **Check for UI hang/timeout:**
   - Look for: Context_activation_failure events on one device within contention window.
   - Check error code: `0x8...` errors suggest timeout or connectivity loss during conflict resolution.
   - **If failure event present: contention resolution failed on that device, causing inconsistency.**

**Known Telemetry Gaps:**
- Kusto does not log user's UI choice during contention resolution (actual options: wait or proceed, not TakeLocal vs TakeRemote).
- No telemetry on how long UI was displayed (UX latency invisible).
- ETL does not correlate which device initiated the conflict detection.
- No field logging whether automatic contention resolution was applied vs manual UI choice.

**Mitigation Checklist:**
- [ ] Identify exact timestamps of both context_activation events.
- [ ] Confirm both devices detected versions (conflictResolution != CR_NoConflictsExpected).
- [ ] Check for any upload failures (PFUploadContextUploadError in ETL).
- [ ] Verify final winning version matches player's expectations.
- [ ] If UI hung: check for context_activation_failure with non-zero hresult.
- [ ] If data loss: compare file contents between device versions and server latest.

---

### 11.3 Upload/Download Failure Diagnosis

**Overview:**  
Upload and download failures manifest differently across ETL and Kusto. ETL shows HTTP-level errors; Kusto shows player-level outcome. Debugging requires examining both to distinguish network errors (transient) from service bugs (persistent) from client SDK bugs (SDK-specific).

**Symptoms:**
- Player's game fails to save (upload timeout or error).
- OR: Player's game fails to sync on new device (download timeout or error).
- Test logs show non-zero hresult for PFGameSaveFilesUploadWithUiAsync or AddUserWithUiAsync.
- Kusto shows `context_sync_error` event with non-zero hresult.
- ETL shows multiple PFResponse events with HttpStatus != 200.

**First Look:**

Check for these indicators:
```text
ETL:
  1. PFUploadContextUploadError or PFDownloadContextDownloadError
  2. PFResponse(PFR_PUSH_UPLOAD) with HttpStatus: 500, 503, or timeout
  3. Multiple retries (PFResponse RequestType repeated 3-5 times)
  4. PFUploadContextUploadChunk hr != 0x0 (chunk failed)

Kusto:
  1. context_sync_error or context_activation_failure
  2. hresult value (0x8... error code)
  3. sync type: syncDownload=true (download) vs false (upload)
  4. retryCount > 0 (indicates retries were attempted)

Test logs:
  1. PFGameSaveFilesUploadWithUiAsync hr != 0x0
  2. AddUserWithUiAsync hr != 0x0
  3. UiProgress stuck at 'Downloading' or 'Uploading'
```

**Deep Dive Telemetry Analysis:**

1. **Identify the failing operation:**
   ```kql
   ['events.all']
   | where Timestamp > ago(24h)
   | where EntityLineage_title == 'YOUR_TITLE'
   | where Entity_Id == 'PLAYER_ID'
   | where FullName_Name in ('context_sync_error', 'context_activation_failure')
   | project Timestamp, FailureName=FullName_Name, EventData
   | order by Timestamp asc
   ```
   
   Extract: `hresult`, `syncDownload` (if sync error), `callingLocation` (if activation failure), `retryCount`.

2. **Correlate with ETL:**
   - Find ETL events ±30s around failure timestamp.
   - Look for:
     - `PFResponse` with non-200 HttpStatus (most common root cause)
     - `PFUploadContextUploadError` or `PFDownloadContextDownloadError` event
     - Multiple attempts at same RequestType (retry loop)
   
   ```
   If HttpStatus: 500, 503 → Server error (transient, likely)
   If HttpStatus: 400, 401, 403 → Client error (credentials, permissions)
   If HttpStatus: timeout (RequestElapsedMs > 30000) → Network or server latency
   If hr: 0x80... in ETL → Client SDK error (bug or resource exhaustion)
   ```

3. **Determine failure scope:**
   - **Single operation failing:** Check if retry succeeded (second PFResponse with 200 status).
   - **Multiple retries failing:** Check HttpStatus pattern. If 503 (Service Unavailable): server issue. If 500 (Internal Error): check PlayFab service status.
   - **Timeout (ElapsedMs > 30000):** Check if subsequent attempts succeed (network glitch) or all fail (persistent issue).

4. **Check data integrity after failure:**
   - If upload failed: query `gamesave_version_finalized` events around failure time. Version should NOT advance.
   - If download failed: check manifest version on device. Should be stale, not updated.
   - If version advanced despite error: bug in error handling (version committed prematurely).

5. **Analyze hresult code:**
   - `0x80004005` (E_FAIL) — Generic failure, check underlying HTTP status.
   - `0x80070780` (ERROR_NOT_FOUND) — Version or blob not found on server (data loss or stale manifest).
   - `0x800705b4` (ERROR_IO_INCOMPLETE) — Partial upload/download (connection lost mid-transfer).
   - `0x80072f78` (WININET_E_OPERATION_CANCELLED) — User canceled or timeout triggered (UI closed app).
   - Query PlayFab error code docs for exact mapping.

**Known Telemetry Gaps:**
- ETL does not log request body/response body (privacy), so exact error reason hidden.
- Kusto retryCount field may be inaccurate if retry happened outside telemetry pipeline.
- No telemetry on client-side timeouts (if app crashed, no error event emitted).
- Network quality (latency, packet loss) not captured; must infer from retry pattern.
- File-level chunk upload/download errors not individually logged (aggregated in final error).

**Mitigation Checklist for Upload Failure:**
- [ ] Check ETL for PFResponse(PFR_INIT_UPLOAD, PFR_PUSH_UPLOAD, PFR_FINALIZE_MANIFEST) HttpStatus.
- [ ] Confirm PFUploadContextFinalized NOT present (upload did not complete).
- [ ] Verify Kusto: gamesave_version_finalized did NOT fire (version not committed).
- [ ] Check hresult: if 0x8... then client SDK issue; if 500/503 then server issue.
- [ ] If retries succeeded: operation was transient (network glitch), not persistent bug.
- [ ] If all retries failed: escalate to service team (server-side root cause likely).

**Mitigation Checklist for Download Failure:**
- [ ] Check ETL for PFResponse(PFR_LIST_MANIFESTS, PFR_GET_MANIFEST, PFR_DOWNLOAD_BLOBS) HttpStatus.
- [ ] Confirm PFContextCreated NOT present (activation did not complete).
- [ ] Verify Kusto: syncState in context_activation remains NotStarted or Downloading (not Completed).
- [ ] Check hresult: if 0x80070780 then version missing on server (data loss); if 0x800705b4 then connection lost.
- [ ] If retries succeeded: transient issue, player can retry manually.
- [ ] If all retries failed and server shows 503: wait for service recovery; if 400-level error: account/permission issue.

**Review Full Analysis:**
- Benjamin Dow ETL traces: `investigations/05-08-forte-investigation/user-reports/Benjamin_Dow_etl_files/etl-analysis.md`

---

### 11.4 isWinner Stomp — Post-Conflict Upload Data Loss

**Overview:**
After a GRTS conflict is resolved with **UseCloud (TakeRemote)**, the next upload from that device carries stale conflict metadata in the HTTP body's Conflict JSON block (the `PendingConflictIsWinner` flag set in GRTS service memory). The upload appears to succeed (HTTP 200 on FINALIZE_MANIFEST), but when other devices download, they receive the **old conflict-winner data** instead of the newly uploaded data. This is silent data loss — no error is reported to the game or the player.

**Bug:** 4377394 (isWinner inversion). Server-side fix in commits `bc220a2` (version retention) and `69e0b63` (isWinner flip for non-SDK callers). See §21 for commit details.

**Symptoms:**
- Player saves new data after resolving a cloud conflict (chose "Use Cloud" / TakeRemote).
- Upload completes successfully from the player's perspective (no error, HTTP 200).
- Other devices (or same device after clean re-init) download and get **old data** — the conflict-winner version, not the newly saved data.
- Player reports: "I saved after the conflict prompt, but my progress is gone."

**First Look:**

```text
ETL (device that resolved conflict):
  1. PFContextConflictResolution with Choice: 1 (UseCloud)
  2. PFActivatorTakeRemoteLoserUploadDisabled (loserVersion, winnerVersion)
  3. LATER: PFXGameSaveServiceUploadContext ← upload starts
     NOTE: UploadOptions is the UPLOAD MODE (0=KeepActive, 1=ReleaseActive), NOT the conflict flag
  4. PFUploadContextFinalized with HTTP 200 ← appears successful

In-Memory (GRTS service process):
  1. PendingConflictIsWinner flag set after conflict resolution
  2. Flag carried in HTTP Conflict JSON block on subsequent uploads
  3. Flag persists within session, cleared on clean PFGameSaveFilesUninitialize

Other device (downloading after the stomp):
  1. Downloads data with LastModified timestamp matching the CONFLICT WINNER, not the new save
  2. File content matches the old cloud version, not what the player just wrote
```

**Deep Dive Telemetry Analysis:**

1. **Confirm conflict resolution happened:**
   - ETL: Search for `PFContextConflictResolution` — note the `ConflictVersion`, `Choice` (1=UseCloud)
   - ETL: Immediately after, look for `PFActivatorTakeRemoteLoserUploadDisabled` — this sets the stomp flag

2. **Find the next upload after conflict resolution:**
   - ETL: Search for `PFXGameSaveServiceUploadContext` after the conflict events
   - Note: The `UploadOptions` field is the **upload mode enum** (KeepDeviceActive=0, ReleaseDeviceAsActive=1), NOT the conflict flag. The conflict metadata is in the HTTP body and NOT visible in ETL.

3. **Verify the upload finalized:**
   - ETL: `PFUploadContextFinalized` with HTTP 200 — upload "succeeded" from client perspective
   - ETL: `PFUploadContextComplete` — upload workflow completed normally

4. **Verify data loss on other device:**
   - Check the downloading device's `AddUserWithUiAsync` → examine the extended manifest
   - Compare `LastModified` timestamp: does it match the conflict winner (old) or the new upload?
   - Compare file content hash if available

**ETL Event Chain (from test 125 v9 reproduction):**

```text
01:08:12  PFContextSyncConflict
            localVersion: 4497  (DIVERGENT data, modified 01:07:39)
            targetManifest: 4498 (CLOUD_V2 data, modified 01:07:31)
01:08:12  PFContextConflictResolution
            ConflictVersion: 4498, Choice: 1 (UseCloud)
01:08:12  PFActivatorTakeRemoteLoserUploadDisabled
            loserVersion: 4497, winnerVersion: 4498
         ... GRTS auto-uploads manifest-only acknowledgment (~0.2s) ...
         ... game writes new data (POST_TAKEREMOTE_V3) ...
01:08:13  PFXGameSaveServiceUploadContext
            UploadOptions: 0  ← upload MODE (KeepDeviceActive), NOT conflict flag
                              ← conflict metadata is in HTTP body, invisible to ETL
01:08:13  PFUploadCopyGameLocation → pgs/4499/
01:08:14  PFUploadContextFinalized (HTTP 200) ← appears successful
         ... DeviceA downloads ...
          Pre-fix: Gets CLOUD_V2 data (timestamp 01:07:31), NOT POST_TAKEREMOTE_V3
          Post-fix: Gets POST_TAKEREMOTE_V3 correctly ✅
```

**Root Cause Chain:**
1. Conflict resolution (UseCloud) calls `PFActivatorTakeRemoteLoserUploadDisabled`
2. GRTS sets `PendingConflictIsWinner` flag **in service memory** (persists across uploads within session)
3. GRTS immediately auto-uploads manifest-only acknowledgment (~0.2s after conflict resolution)
4. The next game-driven upload includes the flag in the HTTP body's Conflict JSON block
5. The service's `FinalizeManifest` receives the conflict metadata (isWinner value from GRTS)
6. Pre-fix: GRTS sends inverted isWinner semantics → service misinterprets → treats upload as loser → data lost
7. Post-fix (`69e0b63`): service flips `isWinner` for non-SDK callers → upload treated correctly → data preserved

**Known Telemetry Gaps:**
- The conflict metadata (isWinner, PendingConflictVersion) is in the HTTP body only — **NOT visible in ETL**
- The `UploadOptions` ETL field is the upload mode enum, NOT the conflict flag (corrected May 2026)
- Kusto does not log whether `isWinner` was flipped by the server fix
- No client-side error — the stomp is completely silent from the game's perspective
- The service metrics (`IsWinnerFlippedForNonSdkCallerMetric`, `LoserUploadDroppedForSdkCallerMetric`) confirm the fix is active but are only in service-side telemetry

**Mitigation Checklist:**
- [ ] Confirm conflict occurred: ETL `PFContextConflictResolution` with `Choice: 1` (UseCloud)
- [ ] Confirm stomp flag set: ETL `PFActivatorTakeRemoteLoserUploadDisabled`
- [ ] Verify upload finalized: ETL `PFUploadContextFinalized` HTTP 200
- [ ] Check downloading device: Does it get old data (conflict winner) or new data?
- [ ] Check server fix: Is `69e0b63` deployed? Check `IsWinnerFlippedForNonSdkCallerMetric` in service telemetry
- [ ] If fix NOT deployed: Player's new save is lost. No recovery path — data was overwritten.

**Reproduction:**
- Test 125 (`gamesave-pc-125-takeremote-next-save-iswinner-stomp.yml`) reproduces this reliably
- Test 126 confirms flag persists across multiple uploads within same session
- Test 129 confirms flag is cleared by proper PFGameSaveFilesUninitialize + relaunch
- Test 130 confirms GRTS auto-uploads immediately after conflict resolution (can't crash before it)
- Test 132 confirms no self-reinforcing 409 loop with server fix deployed
- See `ai-grts-internals.md` Rules 9-10, 13-14 for full behavioral details

---

## 12. Error Code Reference: HRESULT Mapping & Diagnostic Paths

This section documents the most common HRESULT codes you'll encounter in PFGameSave debugging, along with root causes and diagnostic paths.

### 12.1 PlayFab-Specific Error Codes

| Code | Name | Meaning | Root Cause | Diagnostic Path |
|------|------|---------|-----------|-----------------|
| 0x89237004 | E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD | Network unreachable at activation | No internet, VPN down, firewall blocking | Check device network status; try manual network reconnect |
| 0x89237005 | E_PF_GAMESAVE_NETWORK_FAILURE | HTTP request failed (timeout, DNS, cert) | Network latency >30s, DNS failure, SSL error | Check ETL PFResponse times; validate cert chain |
| 0x89237007 | E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE | Device removed from Xbox account | User removed device from account mid-sync | Verify device still registered; check Xbox account settings |
| 0x89237008 | E_PF_GAMESAVE_DISK_FULL | Blob cache full or temp storage full | Local storage <5MB free | Check C:\XboxGames\GameSave\pgs\ size; free disk space |
| 0x800704c7 | E_PF_GAMESAVE_USER_CANCELLED | User canceled upload/download UI | SDK timeout or player force-closed | Check for UiProgress hung state in inproc logs |

### 12.2 XAL (Xbox Authentication Library) Error Codes

| Code | Name | Meaning | Root Cause | Diagnostic Path |
|------|------|---------|-----------|-----------------|
| 0x89235106 | E_XAL_NETWORK | XAL cannot reach Xbox Live services | Network just re-enabled (firewall rules removed); XAL needs ~3s to re-establish connectivity | Add `SmokeDelay 3000` between `EnableNetwork` and first XAL-dependent call (`XUserAddAsync`, `LinkXboxAccountAsync`, `LoginWithXboxAsync`). Note: `LoginWithCustomIDAsync` uses PlayFab HTTP (not XAL) and works immediately. |

### 12.2a XUser (Gaming Runtime) Error Codes

| Code | Name | Meaning | Root Cause | Diagnostic Path |
|------|------|---------|-----------|-----------------|
| 0x89245106 | E_GAMEUSER_NO_DEFAULT_USER | No user signed in for silent login | `XUserAddAsync` called with `AddDefaultUserSilently` but no Xbox user is signed in on this device (common on PC-GRTS engine) | Auto-handled in test harness: `XUserHandlers.cpp` retries with `AddDefaultUserAllowingUI` which shows the account picker. For unattended overnight runs, ensure a user is signed in before starting. |
| 0x89245110 | E_GAMEUSER_NO_PACKAGE_IDENTITY | App lacks package identity | Running a loose (non-packaged) build that hasn't registered its package identity | Ensure app runs from a packaged context or has registered identity via `wdapp register` |
| 0x80070032 | ERROR_NOT_SUPPORTED | API not supported on this platform | `PFLocalUserCreateHandleWithPersistedLocalId` called on Xbox (only works on PC-GRTS) | Use `XUserAddAsync` → `PFLocalUserCreateHandleWithXboxUser` chain on Xbox |

### 12.2b Generic Windows HRESULT Codes

| Code | Name | Meaning | Common in PFGameSave | Next Step |
|------|------|---------|---------------------|-----------|
| 0x80004005 | E_FAIL | Generic failure | YES (most common) | Check underlying HTTP status or ETL error event |
| 0x80070780 | ERROR_NOT_FOUND | File/resource missing | YES (blob download) | Verify version exists on server; check manifest integrity |
| 0x800705b4 | ERROR_IO_INCOMPLETE | Partial I/O (read/write interrupted) | YES (chunk transfer) | Check ETL for PFResponse truncation; retry |
| 0x80072f78 | WININET_E_OPERATION_CANCELLED | HTTP op cancelled (timeout or user) | YES (timeout) | Increase timeout; check network latency |
| 0x80072ee2 | WININET_E_INVALID_URL | URL parse failed (service bug) | RARE | Escalate to service team; check URL in ETL |
| 0x80070005 | E_ACCESSDENIED | Permission denied (ACL issue) | RARE (PGS write) | Check file ACL in C:\XboxGames\GameSave\pgs\; repair permissions |

### 12.3 HTTP Status Codes (from ETL PFResponse)

| HttpStatus | Meaning | Root Cause | Retry Strategy |
|-----------|---------|-----------|-----------------|
| 200 | OK | Success (no error) | N/A |
| 400 | Bad Request | SDK sent malformed request (bug) | DO NOT RETRY; escalate |
| 401 | Unauthorized | Token invalid/expired | RETRY after re-auth; check token endpoint |
| 403 | Forbidden | Permission denied or rate-limited | RETRY with exponential backoff (rate-limit recovery) |
| 500 | Internal Server Error | Server bug (transient or persistent) | RETRY with exponential backoff; check service status |
| 503 | Service Unavailable | Server overloaded or under maintenance | RETRY with exponential backoff; check scheduled maintenance |
| timeout (>30000ms) | Request timeout | Network latency, server slow, SDK timeout configured low | RETRY; increase timeout if possible; check network quality |

### 12.4 Diagnostic Decision Tree

When debugging, use this flowchart to identify root cause:

1. Is it a PlayFab error code (0x8923...)?
   YES: Go to 12.1 table; use Root Cause column to investigate device state
   NO: Continue to 2

2. Is it a Windows HRESULT (0x8007... or 0x8002...)?
   YES: Go to 12.2 table; use Common in PFGameSave to prioritize
   NO: Continue to 3

3. Check ETL for PFResponse events ±30s around failure:
   - If PFResponse exists with non-200 HttpStatus: Go to 12.3 table
   - If no PFResponse: Network connectivity issue (device offline?); check 12.1 codes
   - If multiple PFResponse with retries: Go to 12.3 table; examine retry pattern

4. If root cause not obvious from error code or HTTP status:
   - Check Kusto context_sync_error event for retryCount and error details
   - Correlate with test log timestamps (inproc SDK debug log)
   - Examine ETL state machine (PFActivatorStateChanged, PFUploadWorkerStateChanged events)
   - Escalate to service team with error code + timestamp + test case repro

---

## 13. Manifest Version Lifecycle: Fields & State Transitions

Understanding version numbers and metadata is critical for diagnosing version selection bugs. This section documents the fields and their progression.

### 13.1 Version Fields — Definitions

**On disk** (in {Version}.json manifest):
- **Version** — Unique integer; incremented on every finalize (immutable once committed)
- **ManifestState** — MS_Initialized (uncommitted) or MS_Finalized (committed, permanent)
- **BaseVersion** — Parent version number (tracks sync ancestry; used for delta detection)
- **Timestamp** — When version was created (server time; used for conflict tie-breaking)
- **isWinner** — Flag indicating conflict resolution result (only present if conflictVersion > 0)

**In Kusto** (from gamesave_version_finalized event):
- **contextVersion** — Version being finalized
- **manifestState** — MS_Finalized (only finalized versions emitted to Kusto)
- **conflictVersion** — Non-zero if this version resolved a conflict (identifies conflict partner version number)
- **baseVersion** — Parent version
- **DeviceType** — Which device initiated this version

**In context state** (from context file):
- **SyncStatus** — Which version the device currently believes is "latest"
- **LatestLocal.version** — Most recent version created locally (immutable during this activation)
- **LatestRemote.version** — Most recent version fetched from server (immutable during this activation)

### 13.2 Single-Device Version Progression (Clean Path)

Clean sync progression:

1. Context initialized (no conflict):
   - SyncStatus points to last known version (e.g., V1)
   - LatestRemote fetched from server (e.g., V1, same as local)
   - No divergence detected

2. User calls SaveAsync:
   - New version V2 created locally (baseVersion = V1)
   - V2 marked as MS_Initialized (not yet committed)
   - Upload starts

3. Upload succeeds:
   - V2 marked as MS_Finalized (committed to server)
   - Kusto emits gamesave_version_finalized: contextVersion=V2, conflictVersion=0
   - SyncStatus updated to V2

4. Next activation on same device:
   - SyncStatus points to V2
   - LatestRemote fetched from server (V2)
   - No conflict (versions match, no file divergence)

---

### 13.3 Diagnostic Queries

#### Get manifest versions from server-side telemetry (PlayFabInternal)

The version number and size live inside `EventData.Payload` (a nested JSON object).
You must double-parse: first `EventData` → then `.Payload`.

**Full manifest version history for a player** (run with `--cluster playfab`):
```kql
// Replace ENTITY_ID with the title_player_account Entity_Id (hex string)
['events.all']
| where Timestamp > ago(24h)
| where Entity_Id == 'ENTITY_ID'
| where FullName_Name == 'gamesave_version_finalized'
    or FullName_Name == 'gamesave_version_marked_known_good'
| extend d = parse_json(tostring(EventData))
| extend payload = parse_json(tostring(d.Payload))
| project
    Timestamp,
    Event = FullName_Name,
    Ver = toint(payload.Version),
    DeviceType = tostring(payload.DeviceType),
    SizeBytes = tolong(payload.TotalSizeBytes)
| order by Timestamp asc
```

**How to interpret the output:**
- `gamesave_version_finalized` = new version N uploaded and committed (this is the upload)
- `gamesave_version_marked_known_good` = previous version N-1 confirmed stable (always lags by 1)
- Both events fire at the same timestamp (server processes them together)
- `DeviceType` appears only on finalized events (e.g., "Xbox Series", "Windows Desktop")
- `TotalSizeBytes` = total container size after this upload

**Correlate with client-side contextVersion** (run against Gaming cluster):
```kql
// Replace USER_ID with the XUID (numeric string)
UTCEvents
| where timestamp > ago(24h)
| where name startswith 'Microsoft.Gaming.PlayFab.GameSave'
| extend d = parse_json(tostring(data))
| where d.userId == 'USER_ID'
| project
    timestamp,
    event = substring(name, 40, 50),
    hr = d.hresult,
    contextVer = toint(d.contextVersion),
    elapsed = d.elapsedMs,
    sessionId = d.sessionId
| order by timestamp asc
```

**How client contextVersion maps to server Version:**
- `ContextSync` event shows `contextVer = N` → this is the version **downloaded** from cloud
- `ContextActivation` event shows `contextVer = N+1` → the new version being **written locally**
- `ContextSync` (upload) shows `contextVer = N+1` → the version **uploaded** to cloud
- Server `gamesave_version_finalized` confirms `Version = N+1`

**Example healthy cycle:**
```
Client: ContextSync (contextVer=17)        ← downloaded v17
Client: ContextActivation (contextVer=18)  ← writing new v18
Client: ContextSync (contextVer=18)        ← uploaded v18
Server: version_finalized (Ver=18, 16MB)   ← server confirmed v18
Server: marked_known_good (Ver=17)         ← v17 now safe to GC
```

#### Identify conflict versions

```kql
['events.all']
| where Timestamp > ago(7d)
| where Entity_Id == 'ENTITY_ID'
| where FullName_Name == 'gamesave_version_finalized'
| extend d = parse_json(tostring(EventData))
| extend payload = parse_json(tostring(d.Payload))
| where toint(payload.conflictVersion) > 0
| project
    Timestamp,
    Ver = toint(payload.Version),
    ConflictPartner = toint(payload.conflictVersion),
    IsWinner = payload.IsWinner,
    DeviceType = tostring(payload.DeviceType)
| order by Timestamp asc
```

#### Find the title_player_account Entity_Id for a given XUID

If you only have a XUID, find the Entity_Id first (run with `--cluster playfab`):
```kql
['events.all']
| where Timestamp > ago(24h)
| where FullName_Name == 'player_logged_in'
| where EventData contains 'XUID_HERE'
| project Entity_Id, Entity_Type
| take 1
// This gives the master_player_account Entity_Id.
// Then find the title_player_account:
```
```kql
['events.all']
| where Timestamp > ago(24h)
| where EventData contains 'MASTER_ENTITY_ID'
| where Entity_Type == 'title_player_account'
| distinct Entity_Id
```

#### Check current device SyncStatus (on disk)

```powershell
# On the device with the issue:
$path = 'C:\XboxGames\GameSave\pgs\u_{XUID}_{TITLE_ID}\{XUID}_{TITLE_ID}.json'
$state = Get-Content $path | ConvertFrom-Json
$state.SyncStatus          # Current version pointer
$state.LatestLocal.Version  # Most recent local
$state.LatestRemote.Version # Most recent remote
```


---

## 14. Timeline Reconstruction Methodology: From Device to Global Timeline

Debugging cross-device save issues requires reconstructing a unified timeline across Kusto (global), ETL (GRTS-local), and PGS (device-local). This section documents the methodology.

### 14.1 Three-Layer Timeline Architecture

Global Timeline (Kusto):
  - Entity: player/title
  - Events: context_activation, gamesave_version_finalized, context_sync_error
  - Time source: Server time (UTC)
  - Granularity: ~100ms (batched event emission)
  - Use for: Cross-device patterns, failure correlation, historical analysis

GRTS Local Timeline (ETL):
  - Entity: device/GRTS process
  - Events: PFContextCreated, PFUploadWorkerEntered, PFResponse, state machine transitions
  - Time source: Device clock (may drift from server)
  - Granularity: ~1ms (captured at event emission)
  - Use for: HTTP errors, upload chunking, state machine failures, precise timing

Device Local Timeline (PGS):
  - Entity: device/player
  - State snapshot: Manifest version, SyncStatus, blob cache, extended metadata
  - Time source: N/A (state, not timeline)
  - Use for: Current device state verification, stale data detection, version selection bug confirmation

### 14.2 Reconstruction Workflow

**Step 1: Anchor to Kusto (Global)**
- Start with Kusto query for the player/title around reported problem time window
- Identify key events: context_activation, context_sync_error, gamesave_version_finalized
- Note timestamps (server time) and device types involved

**Step 2: Fetch ETL for each device**
- For each device involved (from Kusto), collect ETL from that device ±30s around Kusto timestamp
- Note: ETL timestamp may differ by ±1-2s from Kusto (clock drift)
- Match ETL timestamps using contextVersion from Kusto as anchor (search ETL for matching version number)

**Step 3: Verify PGS on each device**
- SSH/RDP into each device
- Check current SyncStatus, LatestLocal, LatestRemote from context file
- Compare with versions from Kusto (should match LatestLocal <= current Kusto version)
- If stale (LatestLocal far behind Kusto), indicates activation didn't complete or device offline

**Step 4: Correlate uploads and downloads**
- Match ETL PFResponse(PFR_INIT_UPLOAD, PFR_PUSH_UPLOAD, PFR_FINALIZE_MANIFEST) events with Kusto gamesave_version_finalized
- Check timing alignment: ETL should precede Kusto by ~1-5s (network + batch delay)
- If misaligned (ETL shows upload but no Kusto event): version may have failed to commit

**Step 5: Identify clock skew**
- Calculate offset: Kusto_timestamp - ETL_timestamp for matched events
- Expected offset: ±2s (device clock drift + network time)
- If offset >5s: severe clock skew; may indicate device time zone issue or cached DNS


---

## 15. Quota & Storage Debugging: Blob Counts, Size Limits, & Recovery

PFGameSave includes quota enforcement per player/title. Understanding quota limits and recovery is critical for diagnosing "disk full" or "over quota" errors.

### 15.1 Quota Fields & Calculations

**Quota fields in manifest:**
- **perPlayerQuotaBytes** — Per-player limit (default 256 MB if unset)
- **totalLocalGameSaveBytes** — Sum of all blob sizes on device (cumulative)
- **remaining = perPlayerQuotaBytes - totalLocalGameSaveBytes**

**Quota enforcement:**
- Device rejects new uploads if remaining < size_of_new_blob
- Server enforces same quota; will reject if player over-quota
- Negative remaining = over-quota (shouldn't happen if client enforced)

**Related fields:**
- **blobCount** — Number of files in save (affects container size)
- **containerCount** — Number of containers (each has overhead)
- **totalContainerMetadataBytes** — Overhead of container structure

### 15.2 Quota Error Scenarios

| Scenario | Error Code | Root Cause | Recovery |
|----------|-----------|-----------|----------|
| **Quota exceeded (client)** | 0x89237008 (DISK_FULL) or E_FAIL | New blob exceeds remaining quota | Delete old versions or game data; try again |
| **Quota exceeded (server)** | 400 Bad Request or E_FAIL | Server detects player over-quota | Investigate player's save history; manual cleanup |
| **Container full** | E_FAIL | Too many blobs in single container | Split blobs across containers (game design change) |
| **Device storage full** | 0x89237008 | OS partition full (not PGS quota) | Free OS disk space (not PGS cleanup) |
| **Quota set to 0** | E_FAIL (immediate on any upload) | Game title misconfigured (quota=0) | Contact title team; set perPlayerQuotaBytes >0 |

### 15.3 Quota Diagnostics

**Check current quota usage (Kusto):**
```kql
['events.all']
| where Timestamp > ago(7d)
| where FullName_Name == 'context_activation'
| where Entity_Id == 'PLAYER_ID'
| project Timestamp, TotalBytes=EventData.Payload.totalGameSaveBytes, Quota=EventData.Payload.perPlayerQuotaBytes, Remaining=EventData.Payload.remaining
| order by Timestamp desc
| limit 1
```

**Check device local quota (on disk):**
```powershell
# On device:
$path = 'C:\XboxGames\GameSave\pgs\u_{XUID}_{TITLE_ID}\{XUID}_{TITLE_ID}.json'
$state = Get-Content $path | ConvertFrom-Json
"Total: " + $state.totalGameSaveBytes + " bytes"
"Quota: " + $state.perPlayerQuotaBytes + " bytes"
"Remaining: " + ($state.perPlayerQuotaBytes - $state.totalGameSaveBytes) + " bytes"
```

**List all blobs and sizes:**
```powershell
# On device — for each version:
$versions = Get-ChildItem 'C:\XboxGames\GameSave\pgs\u_{XUID}_{TITLE_ID}\' | Where-Object { $_.Name -match '^[0-9]+$' }
$totalSize = 0
$versions | ForEach-Object {
    $size = (Get-ChildItem $_.FullName -Recurse | Measure-Object -Property Length -Sum).Sum
    Write-Host ($_.Name + ": " + $size + " bytes")
    $totalSize += $size
}
Write-Host ("Total blob cache: " + $totalSize + " bytes")
```

### 15.4 Quota Recovery Strategies

**Strategy 1: Delete old versions**
```powershell
# On device — keep only latest 2 versions:
$versions = Get-ChildItem 'C:\XboxGames\GameSave\pgs\u_{XUID}_{TITLE_ID}\' | Where-Object { $_.Name -match '^[0-9]+$' } | Sort-Object Name -Descending
$keep = $versions | Select-Object -First 2
$remove = $versions | Select-Object -Skip 2
$remove | ForEach-Object { Remove-Item $_.FullName -Recurse -Force }
# Update context file's totalGameSaveBytes
```

**Strategy 2: Verify server doesn't have orphaned blobs**
Check Kusto for version progression - totalGameSaveBytes should decrease as old versions removed.

**Strategy 3: Trigger cleanup via SDK**
In game code, explicitly manage quota:
```text
var remaining = context.perPlayerQuotaBytes - context.totalGameSaveBytes;
if (remaining < newBlobSize) {
    DeleteOldGameData();  // Game-specific cleanup
}
```

### 15.5 Quota Debugging Checklist

- [ ] Confirm perPlayerQuotaBytes is set correctly (>0) for the title
- [ ] Check Kusto for totalGameSaveBytes trend (should stabilize or decrease over time)
- [ ] Verify device disk space (not PGS quota) has >1GB free
- [ ] List all versions on device; count blobs and total size
- [ ] Check for orphaned versions (not in SyncStatus, not reachable)
- [ ] Correlate Kusto totalGameSaveBytes with actual filesystem size (should match ±5%)
- [ ] If over-quota: identify largest blobs and whether they're actively used
- [ ] If quota repeatedly exceeded: game may need larger quota or blob size limits

---

## 16. Conflict Scenario Telemetry Baseline (xplat-14)

This section documents every conflict-specific telemetry marker across all four data sources, using xplat test 14 ("Conflict Resolution Local Wins") from pass155 as the golden reference. Compare with the golden path (Section 10) to isolate conflict-specific signals.

**Test summary**: Two devices (DeviceA=GRTS, DeviceB=inproc) contend over the same save slot. DeviceB detects a conflict with the cloud and resolves it with `UseLocal`. DeviceA later re-syncs and accepts the cloud version with `UseCloud`. Test duration: 70s, 140 ETL events, 18 correlation IDs, all passed.

### 16.1 Version Progression Timeline

```text
Version  Who           Action                              Time (UTC)
───────  ────────────  ──────────────────────────────────  ──────────────
4434     (pre-existing) Starting cloud state               before test
4435     DeviceA (GRTS) Seed upload "CLOUD_BASELINE" (14B) 04:54:27
4436     DeviceB (inproc) Initial download + pending init   04:54:37
4437     DeviceA (GRTS) DeviceA writes local, uploads (13B) 04:54:49
         ── conflict introduced: DeviceB has v4436 local, cloud is now v4437 ──
4438     DeviceB (inproc) Detects conflict v4437 vs local   04:54:59
                         Chooses UseLocal → uploads (14B)   04:55:01-04:55:05
         ── DeviceA now stale: local=4437, cloud=4438 ──
4439     DeviceA (GRTS) Re-sync detects conflict            04:55:11
                         Chooses UseCloud → downloads (14B) 04:55:12
                         Uploads finalized manifest          04:55:19
```

### 16.2 Conflict-Specific ETL Events

These events appear ONLY during conflict scenarios (never in golden path):

| ETL Event | Level | Key Fields | Meaning |
|-----------|-------|------------|---------|
| `PFContextLockContention` | INFO | `ConflictVersion`, `LocalVersion`, `RemoteVersion`, `Choice` | Device detected another device holds the lock. `Choice`: 0=SyncLastSavedData |
| `PFActivatorBreakLockLoserUploadDisabled` | INFO | `loserVersion`, `conflictVersion` | Loser's upload disabled during lock break (active device contention path) |
| `PFActivatorTakeRemoteLoserUploadDisabled` | INFO | `loserVersion`, `winnerVersion` | Loser's upload disabled after TakeRemote resolution |
| `UIProviderQueuedLockContentionWithContext` | INFO | `UserContext`, `LockHolderDeviceId`, `PreviousLockHolderDeviceId` | UI callback queued for lock contention |
| `UIProviderConflictResolutionWithContext` | INFO | `UserContext` | UI callback queued for conflict resolution |
| `PFContextConflictResolution` | INFO | `ConflictVersion`, `LocalVersion`, `RemoteVersion`, `Choice` | User's conflict resolution choice. `Choice`: 0=UseCloud, 1=UseLocal |
| `PFContextSyncConflict` | WARN | `targetManifest`, `localVersion`, `chunkId`, blob details | Per-file conflict detail with local vs remote blob sizes and timestamps |
| `PFContextCheckSyncConflicts` | VERB | `conflictVersion` (populated vs empty) | Empty `conflictVersion` = no conflict; populated = conflict detected |

**Key difference from golden path**: `PFContextInitStart` field `conflictResolution` is `3` (conflict detected during init) vs `0` (no conflict) in golden path. After TakeRemote, it becomes `2`.

### 16.3 Active Device Contention vs Conflict Resolution

Two distinct UI flows appear in conflict scenarios:

**Active Device Contention** (lock break):
```text
ETL:  PFContextLockContention → UIProviderQueuedLockContentionWithContext
SDK:  MyPFXPALGameSaveActiveDeviceContentionUiCallback
Log:  "PFGameSaveFilesUiActiveDeviceContentionCallback (local) time=... deviceId=DeviceB"
Auto: "SyncLastSavedData" → breaks lock, continues sync
```

**Conflict Resolution** (file-level conflict):
```text
ETL:  PFContextSyncConflict → UIProviderConflictResolutionWithContext → PFContextConflictResolution
SDK:  MyPFXPALGameSaveConflictUiCallback  
Log:  "PFGameSaveFilesUiConflictCallback (local) time=... bytes=13"
Auto: "UseLocal" or "UseCloud" → resolves conflict
```

These can fire in sequence: DeviceA Session 2 sees contention THEN conflict during the same sync.

### 16.4 Inproc (DeviceB) Conflict Detection Flow

DeviceB uses the inproc SDK and detects conflict during its second `AddUserWithUiAsync`:

```text
[21:54:56] LockStep: Latest Finalized Manifest v:4437       ← cloud advanced by DeviceA  
[21:54:56] LockStep: No previous manifest. New v:4438       ← DeviceB creates pending v4438
[21:54:58] CompareStep: FilesToUpload: 1                    ← local file differs from remote
[21:54:58] CompareStep: CompressedFilesToDownload: 1        ← remote file also available
[21:54:59] MarkFilesToSync done conflictFound=1 downloading=1  ← CONFLICT DETECTED
[21:54:59] ShowConflictUI - localDesc='' remoteDesc=''
[21:54:59] UICallbackManager::SetAction: user chose 'UIConflictTakeLocal'
[21:54:59] CompareStep - WaitForConflictUI: user chose KEEP LOCAL save
```

After UseLocal, inproc re-reads local manifest and proceeds to upload:
```text
[21:54:59] MarkFilesToSync done conflictFound=1 downloading=1  ← still flagged
[21:55:00] MarkFilesToSync done conflictFound=1 downloading=0  ← upload path only
[21:55:00] UploadStep - CompressFiles: character.sav → 3320C9EB...zip (10240B)
[21:55:01] InitiateUpload → blob upload → FinalizeManifest
[21:55:05] LockStep: Latest Finalized Manifest v:4438       ← DeviceB's upload is now cloud HEAD
[21:55:05] UploadStep: TakeLock ReleaseDeviceAsActive. FinalizedManifest v4438
```

Key inproc markers for conflict:
- `conflictFound=1` in `MarkFilesToSync` (vs `conflictFound=0` in golden path)
- `ShowConflictUI` log line only appears during conflicts
- `UIConflictTakeLocal` / `UIConflictTakeRemote` indicates user choice
- After UseLocal: `FilesToUpload: 1` + `CompressedFilesToDownload: 1` → upload proceeds, download skipped

### 16.5 GRTS (DeviceA) Conflict Detection Flow

DeviceA uses GRTS and encounters two events in Session 3 re-sync:

**Active Device Contention** (Session 2, 04:54:46):
```text
[21:54:46] MyPFXPALGameSaveActiveDeviceContentionUiCallback
[21:54:46]   local time=1778302477, totalBytes=0      ← DeviceA (current device, no data yet)
[21:54:46]   remote time=1778302468, totalBytes=14     ← DeviceB holds the lock
[21:54:47] Auto responder: SyncLastSavedData           ← break lock, proceed
[21:54:48] syncState=2 (Downloading), total=244        ← downloads DeviceB's data
[21:54:49] syncState=5 (SyncComplete)                  ← sync succeeded
```

**Conflict Resolution** (Session 3, 04:55:11):
```text
[21:55:11] MyPFXPALGameSaveConflictUiCallback
[21:55:11]   local time=1778302490, totalBytes=13      ← DeviceA's local data (smaller)
[21:55:11]   remote time=1778302503, totalBytes=14     ← DeviceB's cloud data (larger, newer)
[21:55:11] Auto responder: UseCloud                    ← accept remote
[21:55:11] syncState=1 (PreparingForDownload)
[21:55:12] syncState=5 (SyncComplete), total=10240     ← downloaded DeviceB's version
```

ETL events during DeviceA Session 3:
```text
PFContextCheckSyncConflicts: conflictVersion="" (first pass)
PFContextSyncConflict: localVersion=4437, targetManifest=4438
  localBlobSize=13 vs targetBlobSize=14               ← file-level diff
PFContextConflictResolution: Choice=1 (UseLocal? — actually this is GRTS "TakeRemote")
PFActivatorTakeRemoteLoserUploadDisabled: loserVersion=4437, winnerVersion=4438
PFContextInitStart: conflictResolution=2, conflictVersion=4438
PFContextCreated: Version=4439                         ← new version after resolution
```

### 16.6 Manifest State Snapshots

**After DeviceB conflict resolution** (v4437 extended manifest via GRTS, v4435 via inproc cache):
```json
// GRTS PGS manifest (u_2814640093565666_E18D7) — reflects DeviceA's upload at v4437
{"v1":{"DeviceId":"{8D41D6C0-8EED-4182-B329-BC8351A89B5E}","Version":"4437",
  "Files":[{"FileId":"{127C5933-...}","Size":13,"Extract":[{"Name":"character.sav","Size":13}]}]}}

// Inproc cached manifest (cloudsync/) — still at v4435 baseline
{"v1":{"DeviceId":"{8D41D6C0-8EED-4182-B329-BC8351A89B5E}","Version":"4435",
  "Files":[{"FileId":"{E2F6BA21-...}","Size":14,"Extract":[{"Name":"character.sav","Size":14}]}]}}
```

**After DeviceA re-sync** (v4439 final state):
```json
{"v1":{"DeviceId":"{8D41D6C0-8EED-4182-B329-BC8351A89B5E}","Version":"4439",
  "Files":[{"FileId":"{3320C9EB-...}","CompressSize":10240,"Size":14,
    "Extract":[{"Name":"character.sav","Size":14}]}]}}
```

Note: After conflict resolution, the file IDs change (`E2F6BA21` → `127C5933` → `3320C9EB`) because each upload creates a new blob.

### 16.7 Conflict vs Golden Path Comparison

| Aspect | Golden Path (xplat-01) | Conflict (xplat-14) |
|--------|----------------------|---------------------|
| Devices | 1 (DeviceA GRTS) | 2 (DeviceA GRTS + DeviceB inproc) |
| ETL events | 71 | 140 |
| Correlation IDs | 8 | 18 |
| Version transitions | 4433→4434 (single upload) | 4434→4435→4436→4437→4438→4439 |
| `PFContextInitStart.conflictResolution` | `0` | `3` (first detect), `2` (after TakeRemote) |
| `PFContextCheckSyncConflicts.conflictVersion` | `""` (empty) | `"4438"` (populated) |
| `PFContextSyncConflict` | Not present | Present with blob-level details |
| `PFContextLockContention` | Not present | Present (active device contention) |
| `PFContextConflictResolution` | Not present | Present with `Choice` field |
| Active device contention UI | Not triggered | Fires when second device breaks lock |
| Conflict resolution UI | Not triggered | Fires with local/remote blob sizes |
| `PFModifiedFile` vs `PFAddedFile` | `PFAddedFile` (new blob) | `PFModifiedFile` (existing blob overwrite) |
| Upload after sync | Direct upload | Upload only if UseLocal; download if UseCloud |
| `conflictFound` (inproc) | `0` | `1` |

### 16.8 Debugging Checklist for Conflict Issues

When investigating a suspected conflict scenario:

1. **Identify conflict presence**: Search ETL for `PFContextSyncConflict` or `PFContextConflictResolution`. If absent, it's not a conflict — check Section 10 (golden path).

2. **Determine conflict type**: 
   - `PFContextLockContention` → active device contention (another device holds the lock)
   - `PFContextSyncConflict` → file-level conflict (different blob content at same path)
   - Both can appear in sequence during the same sync

3. **Check resolution choice**:
   - `PFContextConflictResolution.Choice`: `0`=UseCloud/SyncLastSavedData, `1`=UseLocal
   - Inproc log: `UICallbackManager::SetAction: user chose 'UIConflictTakeLocal'` or `'UIConflictTakeRemote'`
   - GRTS log: `Auto responder: ... action=UseLocal/UseCloud`

4. **Verify version chain**: Each conflict resolution should produce a new version. Map:
   - `PFContextInitStart.oldVersion` → what device had before
   - `PFContextInitStart.conflictVersion` → the version that caused conflict
   - `PFContextCreated.Version` → new version after resolution
   - `PFUploadContextFinalized` → confirms upload completed

5. **Check blob integrity**: Compare `localBlobSize` vs `targetBlobSize` in `PFContextSyncConflict`. Mismatched sizes confirm the files differ.

6. **Verify upload disabled correctly**: After `PFActivatorTakeRemoteLoserUploadDisabled` or `PFActivatorBreakLockLoserUploadDisabled`, the losing device should NOT upload — only download.

7. **Check inproc `conflictFound` flag**: In inproc logs, `MarkFilesToSync done conflictFound=1` confirms conflict was detected. If `conflictFound=0` but conflict was expected, check manifest versions.

8. **Cross-reference Kusto**: Query `gamesave_version_finalized` for the user/title and verify the version chain matches ETL. Gaps in version numbers indicate another device uploaded between syncs.

---

## 17. Test Failure Debugging

This section covers the daily workflow for triaging xplat test failures — from identifying which test failed to re-running it in isolation. For debugging the *PFGameSave behavior* exposed by a failing test, use the data source sections (§2–§6) and analysis checklist (§9). This section focuses on the test infrastructure itself.

### 17.1 Output Directory Structure

```text
C:\git\PlayFab.C\Out\gamesave-pc-tests\
├── combined-summary.csv                # All-passes aggregate
├── test-history.csv                    # Historical trend data
├── pass159\
│   ├── summary.csv                     # This pass: test_id, scenario, status, duration_s, error, exit_code
│   └── 01\
│       ├── controller.log              # Orchestrated command sequence (JSON command/response pairs)
│       ├── test-results.json           # Pass/fail summary with timing
│       ├── controller-stdout.txt       # Raw stdout
│       ├── controller-stderr.txt       # Raw stderr (usually empty)
│       ├── device-DeviceA-log.txt      # Full SDK debug log (see §6)
│       └── device-DeviceA-summary.txt  # Filtered: command results only (see §6.2)
```

### 17.2 Triage Workflow

1. **Check `summary.csv`** — scan the `status` column for `FAIL`, `TIMEOUT`, or `SKIP`. Note which test IDs failed and whether they cluster (e.g., tests 42, 46, 47, 49 failing together suggests a shared root cause — see §17.5).

2. **Open the failing test's `controller.log`** — search for `Scenario '...' failed:` or `RESULT: N test(s) FAILED`. The controller log shows which step in the orchestrated sequence failed and what it expected vs. got.

3. **Quick-scan `device-DeviceA-summary.txt`** — grep for `status=failed`. This shows the SDK-level result without internal noise. Note the `hresult` and `elapsed` values.

4. **Deep-dive `device-DeviceA-log.txt`** — search for the failing command's `commandId` (from `controller.log`). Read all `[PlayFab]` lines between the `[Command] Received` and `[Command] <<...>>` result lines. This is where you see SDK internals (see §6.3–§6.5 for log format).

5. **Decide next step:**
   - If the failure is SDK/PFGameSave behavior → continue with §9 Analysis Checklist
   - If the failure is test infrastructure → check §17.5 (common non-bug failures)
   - If you need historical context → check §17.4 (trend analysis)

### 17.3 Re-Running Tests

```powershell
# Single test
py tests-run.py gamesave-pc --only 42

# Multiple specific tests
py tests-run.py gamesave-pc --only 42,46,47,49

# Only known-failing tests
py tests-run.py gamesave-pc --failing

# Stable regression suite only
py tests-run.py gamesave-pc --passing

# Single test with shorter timeout
py tests-run.py gamesave-pc --only 42 --timeout 120
```

### 17.4 Historical Trend Analysis

```powershell
# Show flaky tests (intermittent pass/fail)
py combine-xplat-results.py --flaky

# History for specific tests
py combine-xplat-results.py --test 42,46

# Recent 10 passes
py combine-xplat-results.py --recent 10
```

Output includes:
- **Stability classification**: `stable-pass`, `flaky`, `stable-fail`
- **Streak tracking**: `Px5` (5 consecutive passes), `Fx3` (3 consecutive failures)
- **ASCII timeline**: `.` = pass, `X` = fail, `T` = timeout

---

## 18. GRTS Service Debugging — Crash Dumps & Reinstallation

When GRTS itself is the problem — crashes, hangs, wrong version — use the GRTS debugging scripts in `C:\git\PlayFab.C\Utilities\Scripts\`. All scripts named `grts-*.ps1`.

### 18.1 Scripts

| Script | Purpose | Admin? |
|--------|---------|--------|
| `grts-enable-dumps.ps1` | Configure WER to capture full heap crash dumps for `gamingservices.exe` | Yes (auto-elevates) |
| `grts-disable-dumps.ps1` | Remove the WER LocalDumps registry key | Yes |
| `grts-crash-report.ps1` | Generate crash timeline report from Windows Event Logs | No |
| `grts-collect-dump.ps1` | Package latest crash dump + metadata into a zip | No |
| `grts-reinstall.ps1` | Full uninstall/reinstall of GamingServices APPX package | Yes |

### 18.2 Crash Dump Capture

**`grts-enable-dumps.ps1`** configures Windows Error Reporting to capture full heap dumps when `gamingservices.exe` crashes.

```powershell
# Arm WER — default dump folder is C:\CrashDumps\GRTS
.\grts-enable-dumps.ps1

# Custom dump folder
.\grts-enable-dumps.ps1 -DumpFolder D:\Dumps\GRTS
```

Sets 3 registry values under `HKLM:\SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps\gamingservices.exe`:

| Key | Value | Meaning |
|-----|-------|---------|
| `DumpFolder` | `C:\CrashDumps\GRTS` | Where dumps are written |
| `DumpType` | `2` | Full heap dump (includes all memory) |
| `DumpCount` | `10` | Keep last 10 dumps |

### 18.3 Crash Report

**`grts-crash-report.ps1`** scans Windows Event Logs (Application Error + Windows Error Reporting) for GamingServices crashes and generates a structured report.

```powershell
# Last 24 hours (default)
.\grts-crash-report.ps1

# Last 1 hour, save to file
.\grts-crash-report.ps1 -Hours 1 -OutputFile crash-report.txt

# CSV output for analysis
.\grts-crash-report.ps1 -Csv -OutputFile crashes.csv
```

The report includes:
- **Crash summary**: total count, time range
- **Breakdowns**: by exception code (`ACCESS_VIOLATION`, `HEAP_CORRUPTION`, etc.), by faulting module
- **Crash-loop detection**: 3+ crashes within 5 minutes flagged as a crash loop
- **Full timeline table**: each crash with timestamp, exception, faulting module
- **WER archive locations**: paths to collected crash data

### 18.4 Crash Dump Collection

**`grts-collect-dump.ps1`** packages the newest crash dump and metadata into a single zip for sharing.

```powershell
.\grts-collect-dump.ps1
# Output: grts-crash-YYYYMMDD-HHmmss.zip on Desktop
```

The zip contains: `.dmp` file, machine name, OS version, GRTS package version, and `Report.wer` metadata.

### 18.5 GRTS Reinstallation

**`grts-reinstall.ps1`** performs a full uninstall/reinstall of the GamingServices APPX package.

```powershell
# Full uninstall + install
.\grts-reinstall.ps1 -PackagePath C:\builds\GamingServices.appx

# Install only (skip uninstall)
.\grts-reinstall.ps1 -PackagePath C:\builds\GamingServices.appx -SkipUninstall
```

Uninstall sequence: kills processes → stops services → removes provisioned and user packages.
Install: `Add-AppxPackage` with fallback to `-AllowUnsigned`.
Verification: confirms installed version, starts service, lists running GRTS processes.

### 18.6 Typical Crash Investigation Workflow

```text
1. grts-enable-dumps.ps1              # arm WER to capture full dumps (one-time setup — leave it enabled)
2. (reproduce the crash or wait)
3. grts-crash-report.ps1 -Hours 1     # see crash timeline + loop detection
4. grts-collect-dump.ps1              # package dump + metadata into zip
5. grts-reinstall.ps1 -PackagePath <new-build>   # deploy a fix
```

> **Note:** Once `grts-enable-dumps.ps1` has been run, crash dump collection stays enabled permanently — there's no need to disable it. Leave it armed so future crashes are automatically captured.

> **Cross-reference:** GRTS produces ETL traces (§2) and on-disk state (§5). If GRTS crashes mid-sync, expect truncated ETL events (missing `PFUploadContextFinalized`) and stale PGS state (`SyncStatus=0` in the context state file — see §5.2).

---

## 19. Offline→Online Transition Debugging

The xplat test suite includes scenarios that toggle network connectivity mid-test to validate offline saves, return-to-online sync, and network disruption recovery. These tests use **AdminHelper** — an elevated helper process that manages network and service state via named pipe commands.

### 19.1 AdminHelper Architecture

| Property | Value |
|----------|-------|
| Source | `Test\AdminHelper\Program.cs` |
| Named pipe | `PlayFabTestAdminHelper` |
| Protocol | JSON over named pipe: `{"action":"ActionName","parameters":{...}}\n` |

**Supported actions:**

| Action | What It Does |
|--------|-------------|
| `DisableNetwork` | `Get-NetAdapter -Physical \| Where-Object { $_.Status -eq 'Up' } \| Disable-NetAdapter -Confirm:$false` |
| `EnableNetwork` | `Get-NetAdapter -Physical \| Where-Object { $_.Status -eq 'Disabled' } \| Enable-NetAdapter -Confirm:$false` — then waits for connectivity via `ping 8.8.8.8` |
| `NetworkFlapping` | Rapidly toggles network on/off (stress test) |
| `StopGamingServices` | Stops the GamingServices service |
| `StartGamingServices` | Starts the GamingServices service |
| `Ping` | Connectivity check (returns success/failure) |

### 19.2 FlushGrtsAndGoOffline Sequence

The `FlushGrtsAndGoOffline` composite action (in `DeviceStateController.cs:458–685`) prepares a device for offline testing by clearing all GRTS state and disabling network:

1. `StopGamingServices` — stop GRTS service
2. Delete `C:\XboxGames\GameSave\pgs` — remove all on-disk state (see §5)
3. `DisableNetwork` — disable all physical adapters
4. `StartGamingServices` — restart GRTS (now offline, no cached state)
5. Relaunch device app — fresh process with offline GRTS

### 19.3 Test YAML Usage

```yaml
# Go offline
- ChangeTargetDeviceState:
    action: FlushGrtsAndGoOffline

# ... offline operations (saves, reads, etc.) ...

# Return online
- ChangeTargetDeviceState:
    action: EnableNetwork
```

### 19.4 Network Cleanup

After every test, `testrunner.py` calls `AdminHelpCli EnableNetwork` via `reset_network()` to ensure network is restored. If AdminHelper dies mid-test, network state leaks — subsequent tests may run with the network disabled (see §17.5, failure #4).

### 19.5 Debugging Offline→Online Test Failures

1. **Verify AdminHelper is running** — if dead, all network state management fails silently.
2. **Check `EnableNetwork` waited for connectivity** — the action pings `8.8.8.8` to confirm. If DNS or routing is still recovering, the first post-online API call may fail with `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` (§12.1).
3. **Add delay for XAL-dependent calls** — after `EnableNetwork`, Xbox Authentication Library (XAL) needs ~3 seconds to re-establish Xbox Live connectivity. Without a delay, `XUserAddAsync`, `LinkXboxAccountAsync`, and `LoginWithXboxAsync` will fail with `E_XAL_NETWORK (0x89235106)` (§12.2). PlayFab HTTP calls (`LoginWithCustomIDAsync`) work immediately — they use libHttpClient, not XAL. **Fix:** Add `SmokeDelay 3000` between `EnableNetwork` and the first XAL call.
4. **Check PGS state after `FlushGrtsAndGoOffline`** — the PGS directory (`C:\XboxGames\GameSave\pgs`) should be empty. If files remain, the delete step failed (permissions or locked files).
5. **Check GRTS service restarted** — after `StartGamingServices`, GRTS should be running but with no cached state. ETL (§2) should show a fresh `NewPlayFabCaller` event on the next SDK call.
6. **Check `isConnectedToCloud`** — in inproc logs (§6.4), `PFXPALGetFolderComplete: ... isConnectedToCloud=false` confirms the SDK detected offline mode. After `EnableNetwork`, the next activation should show `isConnectedToCloud=true`.

---

## 20. Inproc ↔ GRTS Provider Switching

The test suite runs scenarios on different SDK providers — GRTS (out-of-proc) and inproc (in-process). Understanding how provider selection works is essential when a failure is provider-specific.

### 20.1 Engine Strings

Each test scenario declares its target engine as a plain string:

| Engine String | Provider | Platform |
|--------------|----------|----------|
| `pc-grts` | GRTS (out-of-proc) | PC with GamingServices |
| `pc-inproc` | Inproc (in-process) | PC without GRTS |
| `pc-inproc-gamesaves` | Inproc (in-process) | PC, game save specific |
| `xbox` | GRTS (out-of-proc) | Xbox console |

### 20.2 Scenario YAML Declaration

Each test scenario declares its engine in the YAML header:

```yaml
platforms:
  - pc-inproc
devices:
  role1:
    engine: pc-inproc
```

### 20.3 Controller Logic

The test controller (`GameTestController`) determines the provider path from the engine string:

- `RequiresInproc` = `Engine.Any(e => e.StartsWith("pc-inproc"))`
- When `RequiresInproc == true`, the device app is launched with the `/forceinproc` flag
- This causes the SDK to select the inproc provider instead of the GRTS provider (see §8, Platform Type Nuances)

### 20.4 CLI Filtering

To run only scenarios for a specific provider:

```powershell
# Run only inproc scenarios
GameTestController.exe --headless --run-tag passing --platform-filter pc-inproc

# Run scenarios for specific engines
GameTestController.exe --headless --allowed-engines pc-grts,pc-inproc
```

| Flag | Purpose |
|------|---------|
| `--platform-filter <key>` | Only run scenarios whose engine matches `<key>` (e.g., `pc-inproc`) |
| `--allowed-engines <csv>` | Restrict to listed engines (e.g., `pc-grts,pc-inproc`) |

> **UI shortcut normalization:** The controller UI normalizes common shortcuts: `pc/grts` → `pc-grts`, `inproc` → `pc-inproc`.

### 20.5 Debugging Provider-Specific Failures

When a test passes on one provider but fails on another:

1. **Confirm the provider** — check the scenario YAML `engine:` field and the inproc log for `PlatformGetPlatformType: ... -> platformInfo=GRTSAvailable` (GRTS) vs `platformInfo=InProc` (inproc) — see §6.4.
2. **Check available data sources** — GRTS produces ETL (§2) and PGS (§5). Inproc does NOT produce ETL or PGS — use inproc SDK logs (§6) and Kusto (§3) only.
3. **Check manifest location** — GRTS manifests: `C:\XboxGames\GameSave\pgs\u_{XUID}_{TitleID}\`. Inproc manifests: `<saveFolder>\cloudsync\` (only if `SetWriteManifestsToDiskForDebug(true)` — see §6.7).
4. **Re-run with the other provider** — use `--platform-filter` to isolate. If a test only exists for one engine, check if the YAML can be duplicated with a different `engine:` value.

---

## 21. isWinner Inversion — Commit Facts

This section documents server-side changes to conflict resolution logic that affect how `isWinner` is interpreted in Kusto telemetry and version retention. These are the facts of what changed — refer to the linked investigation reports for analysis.

> **Cross-reference:** For conflict detection flow and telemetry markers, see §16. For the isWinner field definition, see §13.1.

### 21.1 Commit `bc220a2` — Version Retention Change

**"Keep both latest finalized and latest non-loser in version retention"**

| Field | Value |
|-------|-------|
| Author | Aleksandra Zakrzewska |
| Date | 2026-05-01 |
| Files | `ManifestDocumentProcessingLogic.cs`, `ManifestDocumentProcessingLogicTests.cs` |

**What changed:** The server's version trimming logic was changed from **two** anchor versions to **three**:

- **Before:** `latestFinalizedVersion` was computed from non-loser manifests only. Deletion trimmed everything except this anchor and its base.
- **After:** `latestFinalizedVersion` is now the overall maximum of ALL finalized versions (including losers). A new nullable `latestNonLoserFinalizedVersion` is computed from non-losers only. The deletion filter excludes **both** anchors.

**Test changes:**
- Expected deletions changed from `{10, 15, 99}` → `{10, 15}` — version 99 (a loser) is now retained instead of trimmed.
- New test: `AllThreeAnchorsDistinctDeletesNothing` — when all 3 manifest anchors are distinct, nothing is deleted.

### 21.2 Commit `69e0b63` — isWinner Flip for Non-SDK Callers

**"Conflict loser bug workaround — flip non SDK caller isWinner values and silently drop SDK losers"**

| Field | Value |
|-------|-------|
| Author | Aleksandra Zakrzewska |
| Date | 2026-05-02 |
| Files | 6 files changed |
| Design doc | `ConflictResolutionBugMitigation.md` (links to SharePoint) |

**Core change in `ManifestServiceLogic.cs` → `FinalizeManifestAsync`:**

The `IsWinner` field is now mutated based on the caller type:

| Caller | `IsWinner` Value | Action |
|--------|-----------------|--------|
| `IsCallerPlayFabSDK == null` | Any | No change (existing behavior preserved) |
| `IsCallerPlayFabSDK == true` | `true` | Normal finalization (no change) |
| `IsCallerPlayFabSDK == true` | `false` | Manifest set to `PendingDeletion`, `dropAsConflictLoser = true` |
| `IsCallerPlayFabSDK == false` | Any | `IsWinner` is **flipped** to its boolean negation |

**Additional changes:**
- `FinalizeManifestConflict.cs`: `IsWinner` property changed from `{ get; init; }` → `{ get; set; }` to allow mutation
- Added idempotent retry path: if manifest is already `PendingDeletion`, returns `Success` instead of erroring

**New metrics:**

| Metric | Fires When |
|--------|-----------|
| `IsWinnerFlippedForNonSdkCallerMetric` | `IsWinner` flipped for non-SDK caller |
| `LoserUploadDroppedForSdkCallerMetric` | SDK caller's loser upload dropped to PendingDeletion |
| `BaseVersionNotFoundMetric` | Base version missing during finalization |
| `ManifestConflictRetryMetric` | Idempotent retry path taken |
| `MissingFileInFileMapMetric` | File referenced in manifest not found in file map |

**New tests (5):**
1. Null `IsCallerPlayFabSDK` preserves existing behavior
2. Non-SDK caller: `IsWinner` is flipped
3. SDK caller with `IsWinner=true`: normal finalization
4. SDK caller with `IsWinner=false`: dropped to `PendingDeletion`
5. Retry on already-`PendingDeletion` manifest: returns `Success` idempotently

> **Investigation reports:**
> - `investigations/05-08-forte-investigation/reports/forte-iswinner-5step-failure.md`
> - `investigations/05-08-forte-investigation/reports/rakesh-workaround-explained.md`
> - `investigations/05-08-forte-investigation/reports/iswinner-non-conflict-analysis.md`

### 21.3 Client-Side Confirmation: PendingConflictIsWinner Stomp (Test 125)

**"GRTS sends PendingConflictIsWinner in HTTP Conflict block after TakeRemote, causing data loss on next upload"**

| Field | Value |
|-------|-------|
| Confirmed by | Bishop (test 125 v9), ETL trace analysis |
| Date | 2026-05-10 |
| Test | `gamesave-pc-125-takeremote-next-save-iswinner-stomp.yml` |
| ETL trace | `investigations/test125-v9-etl.txt` |
| GRTS internals | `ai-grts-internals.md` Rules 9-10, 13-14 |

**What happens (client-side, verified via ETL):**

1. Conflict detected during `AddUserWithUiAsync` → GRTS shows conflict UI
2. User/test chooses UseCloud (TakeRemote) → `PFContextConflictResolution(Choice: 1)`
3. `PFActivatorTakeRemoteLoserUploadDisabled` fires → sets `PendingConflictIsWinner` flag **in GRTS service memory**
4. GRTS downloads cloud winner data, replaces save folder
5. GRTS immediately auto-uploads manifest-only acknowledgment (~0.2s, see §24.4)
6. Game writes new data, calls `UploadWithUiAsync`
7. GRTS includes the PendingConflictIsWinner flag in the HTTP body's Conflict JSON block
8. Upload finalizes HTTP 200 (appears successful from client)
9. Pre-fix: Other devices download → get **old conflict winner data**, not the new upload
10. Post-fix: Server flips isWinner → upload treated as normal → data preserved ✅

> ⚠️ **CORRECTION (May 2026, tests 126-133):** The ETL field `UploadOptions` in `PFXGameSaveServiceUploadContext` is the **upload mode enum** (0=KeepDeviceActive, 1=ReleaseDeviceAsActive), NOT the conflict flag. The actual conflict metadata (isWinner, PendingConflictVersion) is carried in the HTTP request body and is NOT visible in ETL. Prior analysis incorrectly attributed the stomp to UploadOptions:1.

**Relationship to server-side fix (`69e0b63`):**

The server fix (§21.2) addresses this by flipping `isWinner` for non-SDK callers:
- GRTS is a non-SDK caller (`IsCallerPlayFabSDK == false`)
- GRTS sends `isWinner=false` in the Conflict block (GRTS uses inverted semantics)
- Server flips it to `isWinner=true` → upload treated as winning version (preserved)
- This prevents the data loss

**Without the server fix:**
- `isWinner=false` reaches the service as-is
- Service treats the upload as a conflict loser (should be dropped)
- Version retention logic (§21.1) keeps the old winner → new data is effectively invisible
- Other devices download stale data → **silent data loss**

**Server fix verification (tests 126, 132):**
- With fix deployed, multiple consecutive uploads after TakeRemote all succeed
- Data propagates correctly to other devices (test 126 ETL confirmed)
- No self-reinforcing 409 loop (test 132: 3 consecutive uploads, all HTTP 200)

**PendingConflictIsWinner flag lifecycle (corrected from tests 126-133):**

| Scenario | Flag persists? | Test |
|----------|---------------|------|
| Same session, multiple uploads | **YES** — all carry conflict metadata | Test 126, 132 |
| Clean PFGameSaveFilesUninitialize + relaunch | **NO** — flag cleared | Test 129 |
| TakeRemote → TakeLocal in same session | **YES** — TakeLocal doesn't clear it | Test 133 |
| Process Terminate (crash) | Flag survives in GRTS service memory | Test 130 |

**Key ETL markers (for debugging whether the fix is working):**

| ETL Event | Field | Meaning |
|-----------|-------|---------|
| `PFActivatorTakeRemoteLoserUploadDisabled` | (present) | PendingConflictIsWinner flag was set |
| `PFXGameSaveServiceUploadContext` | `UploadOptions` | **Upload MODE only** (0=KeepActive, 1=ReleaseActive) — NOT the conflict flag |
| `PFUploadContextFinalized` | HTTP status | Both normal and stomp uploads show 200 — cannot distinguish from ETL alone |

**Debugging notes:**
- The conflict metadata (isWinner, PendingConflictVersion) is in the HTTP body only — invisible to ETL
- Server-side metrics (`IsWinnerFlippedForNonSdkCallerMetric`) confirm the fix is active
- To confirm stomp occurred pre-fix: verify data on downloading device matches old conflict winner, not new save

---

## 22. Platform Type Reference

This table maps every supported environment to its `platformType` telemetry value, as reported in Kusto events (§3.3).

| Environment | `platformType` Telemetry Value | Provider |
|-------------|-------------------------------|----------|
| Xbox console (GDK) | `Xbox` | GRTS (out-of-proc) |
| PC with GRTS | `PC` | GRTS (out-of-proc) |
| PC without GRTS (inproc) | `Windows` | Inproc |
| PC forced local services | `Windows` | Inproc |
| Steam PC | `Windows` | GRTS (PC GRTS — not inproc) |
| Steam Deck (no GRTS) | `Windows` | Inproc |
| PlayStation | `PlayStation` | Platform-native |
| Nintendo | `Nintendo` | Platform-native |

**Key facts:**
- Steam on PC uses PC GRTS (not inproc) — Steam saves go through the same GRTS path as non-Steam PC.
- Steam Deck uses inproc (no GRTS available on Linux/SteamOS).
- Both Steam PC and Steam Deck report `platformType=Windows` — distinguish by checking whether ETL/PGS data sources exist (GRTS path) or only inproc logs (inproc path).

<!-- REVIEW: §8 Platform Type Nuances table lists different values (e.g., "SteamDeck" as platformType, "WindowsInproc" for forced inproc). This §22 table was provided by Jason directly. Reconcile which reflects current production telemetry. -->

> **See also:** §8 (Telemetry Gaps — Platform Type Nuances) for the values from the telemetry codebase review. §20 (Inproc ↔ GRTS Provider Switching) for how provider selection works in tests.

---

## 23. SteamDeck Workflow (TBD)

> **This section is TBD — SteamDeck workflow is not yet fully documented.**

SteamDeck uses the **inproc** provider (no GRTS available on SteamOS). This means:

- **No ETL traces** — GRTS does not run on SteamDeck, so §2 does not apply
- **No PGS on-disk state** — the `C:\XboxGames\GameSave\pgs\` directory does not exist; inproc manifests are in `<saveFolder>\cloudsync\` (see §6.7)
- **Kusto telemetry available** — PlayFabInternal events (§3) work regardless of platform
- **Inproc SDK logs available** — if the game integrates verbose logging (see §6)

For the platform type mapping, see §22.

<!-- TODO: Document SteamDeck-specific debugging workflow:
  - How to collect inproc logs from SteamDeck
  - SteamDeck file paths and save folder locations
  - Steam Cloud integration (if any) vs PFGameSave
  - Known SteamDeck-specific failure modes
  - How to force inproc on PC to simulate SteamDeck behavior
-->

---

## 24. GRTS Upload & Conflict Lifecycle

> How GRTS detects dirty saves, uploads data to the cloud, detects conflicts, and resolves them. This section explains the full lifecycle so debuggers can understand GRTS ETL traces and diagnose failures at any stage. For deep source-level details, see `ai-grts-internals.md`.

### 24.1 Architecture Overview

GRTS (Gaming Runtime Services, `GamingServices.exe`) is an **out-of-process Windows service** that manages game save cloud sync. It runs independently of the game — the game process can exit and GRTS continues uploading. The SDK communicates with GRTS via COM/RPC; GRTS communicates with PlayFab's cloud service via HTTP.

```text
Game Process                           GRTS Service Process
┌──────────────────┐                   ┌──────────────────────────────────┐
│  PFGameSave SDK  │                   │  ConnectedStorage Service        │
│  (PlayFab.C)     │   COM/RPC         │                                  │
│  Provider_GRTS ──┼──────────────────►│  PFActivator (download/sync)     │
│                  │◄──────────────────┤  PFContext (active save state)    │
│  Registers:      │   Callbacks       │  PFUploadContext (upload)         │
│  - UIProvider    │                   │                                  │
│  - FileSpaceHdlr │                   │  NtmWebService ──► PlayFab API  │
└──────────────────┘                   └──────────────────────────────────┘
```

**Key fact:** `PFGameSaveFilesUninitializeAsync` does NOT stop GRTS tracking. GRTS monitors save folders independently. A user can write to the game save folder when the game isn't even running, and GRTS will detect it and upload.

### 24.2 Upload Lifecycle — From File Write to Cloud

When a game writes save data, here's the full chain from file write to cloud commit:

#### Step 1: Dirty Detection

GRTS monitors the game's save folder. After a successful `UploadWithUi(KeepDeviceActive)`, GRTS re-initializes the context (allocates next version N+1). Any subsequent file changes create a "dirty" state relative to version N+1's committed state.

**Detection is NOT instant.** GRTS checks for dirty data:
- On process termination (kernel signals via process handle)
- On user sign-out
- On service boot (checks registry for pending uploads)
- On network reconnect (retries deferred uploads)

GRTS does NOT have a periodic upload timer for the PF path. Dirty data sits locally until one of the above events triggers upload.

ETL markers for dirty detection:
```
PFUploadWorkerEntered               — background upload worker starts
PFUploadCopyGameLocation            — copies save folder → pgs/{version}/
PFModifiedFile                      — identifies changed files
PFUploadPlanStart (ModifiedFiles: N) — N files differ
PFUploadPlanComplete (UploadChunks: N) — upload plan ready
```

#### Step 2: Upload Execution

GRTS creates compressed chunks from modified files and uploads to Azure Blob Storage:

```
PFR_INIT_UPLOAD → 200               — server allocates upload URIs
PFR_PUSH_UPLOAD → 201               — data pushed to blob storage
PFR_FINALIZE_MANIFEST → 200         — version committed to cloud
PFUploadContextFinalized             — upload complete
PFUploadContextComplete              — cleanup done
```

The full upload for 10KB of data typically completes in ~2 seconds.

#### Step 3: Re-initialization (KeepDeviceActive)

After `UploadWithUi(KeepDeviceActive)`, GRTS automatically re-initializes:
```
PFContextLoadedSession              — session reloaded
PFResponse (PFR_LIST_MANIFESTS)     — check for newer cloud versions
PFContextInitManifestRequest        — allocate next version (N+1)
PFContextCreated (Version: N+1)     — new context ready
```

Any file writes after this point create dirty state relative to N+1.

#### Upload Failure Modes

| Failure | ETL Marker | What Happens Next |
|---------|------------|-------------------|
| **HTTP 409 (Conflict)** on FINALIZE | `PFUploadContextAbandoned`, `PFUploadAbandonedClearSession` | Upload abandoned, session state cleared. Next AddUser sees no dirty state → no conflict (see §24.4 Rule 3) |
| **Network lost** | `PFUploadContextDeferredNoNetwork` | Upload deferred, dirty state preserved. Retries on network reconnect (~10s interval) |
| **HTTP 500/503** | `PFUploadRetryWait` | Exponential backoff retry |
| **Service shutdown** | Upload abandoned | Resumes on next service boot via registry `PendingPlayFabSession` |

### 24.3 Download/Sync Lifecycle — AddUserWithUiAsync

When the game calls `AddUserWithUiAsync` (via GRTS), here's the full sync flow:

```
PFAS_MountStorage                    — mount local storage
PFAS_PlayFabLogin                    — authenticate with PlayFab
PFAS_ListManifests                   — get available cloud versions
PFAS_ParseListManifests              — parse manifest list
PFAS_GetManifest                     — fetch specific version metadata
PFAS_ParseManifest                   — parse version details
PFAS_GetExtManifest                  — fetch extended manifest (file tree)
PFAS_CreateSyncPlan                  — determine what to download/upload
PFAS_StorageChecks                   — verify disk space
PFAS_HandleSyncConflicts             — check for conflicts (§24.4)
PFAS_SyncData                        — download/upload data
PFAS_InitManifest                    — allocate new version
PFAS_ContextCreated                  — context ready, folder returned to game
```

The sync plan compares:
- **Local save folder contents** (files, timestamps, sizes)
- **On-disk blob cache** (pgs/{version}/ data from last upload/download)
- **Cloud manifest** (latest finalized version from server)

If local save folder matches the blob cache → no modifications → download latest cloud version if newer.
If local save folder differs from blob cache → modifications detected → potential conflict if cloud also advanced.

### 24.4 Conflict Detection & Resolution

A conflict occurs when **both** local and cloud data have changed since the last sync. GRTS detects this during `PFAS_HandleSyncConflicts`.

#### When Does a Conflict Fire?

A conflict requires ALL of these conditions:
1. **Local dirty data**: The save folder has been modified since the last committed version
2. **Cloud advanced**: The server has a newer finalized version than the device's last known version
3. **Blob cache mismatch**: The save folder contents differ from the on-disk blob cache (pgs/{version}/)

If any condition is missing, GRTS just downloads (no local changes) or uploads (no cloud changes) — no conflict.

#### Conflict ETL Event Chain

```
PFContextCheckSyncConflicts
    conflictVersion: 4498            ← POPULATED = conflict detected
                                       (EMPTY = no conflict)
PFContextSyncConflict
    localVersion: 4497               ← local dirty version
    targetManifest: 4498             ← cloud winner version
UIProviderConflictResolutionWithContext  ← UI callback queued
PFContextConflictResolution
    ConflictVersion: 4498
    LocalVersion: 4497
    Choice: 0 (UseLocal/TakeLocal) or 1 (UseCloud/TakeRemote)
```

#### Conflict Resolution Choices

| Choice | ETL `Choice` Value | What Happens |
|--------|-------------------|--------------|
| **UseLocal (TakeLocal)** | `0` | Keep local data, upload as new version. Cloud version discarded. `PFActivatorTakeLocalConflict` fires. Does NOT set PendingConflictIsWinner. |
| **UseCloud (TakeRemote)** | `1` | Discard local data, download cloud version. Local changes lost. `PFActivatorTakeRemoteLoserUploadDisabled` fires. Sets PendingConflictIsWinner in memory. |
| **Cancel** | — | Go offline with local data (`PFAS_InitManifestOffline`). |

⚠️ **After UseCloud/TakeRemote**, the `PendingConflictIsWinner` flag is set **in GRTS service memory** (not on disk). It persists within the session and is carried in the HTTP body's Conflict JSON block on subsequent uploads. The flag is cleared on clean `PFGameSaveFilesUninitializeAsync`. This is the isWinner stomp bug — see §11.4 and §21.3. **With server fix deployed**, the server inverts isWinner for GRTS callers, preventing the stomp.

#### Post-Resolution Auto-Upload (Discovered Test 130)

After TakeRemote conflict resolution, GRTS **immediately** performs a manifest-only auto-upload within ~0.2s of `PFContextCreated`:
- `TouchedChunks: 0`, `TotalSize: 0` — no blob data, manifest acknowledgment only
- Commits conflict resolution state to server
- Cannot be prevented by game code or test harness Terminate
- Makes "crash before upload after conflict" scenarios nearly impossible
- TakeLocal does NOT trigger auto-upload (`reason: NoUploadNeeded`)

#### Active Device Contention (Different from Conflict)

Contention is NOT a data conflict — it's a **lock** conflict. It fires when another device is actively uploading:

```
PFContextLockContention              ← another device holds the upload lock
UIProviderQueuedLockContentionWithContext
```

User choices: Break Lock (proceed), Retry (re-check), Cancel (go offline). Contention and conflict can fire in sequence during the same sync.

### 24.5 How to Trigger a Conflict in Tests

Triggering a real GRTS conflict on a single machine is hard because of a fundamental race condition (see `ai-grts-internals.md` Rule 5). Here is the **verified working sequence** from test 125:

```text
Phase 1: DeviceA seeds cloud (SEED data)
Phase 2: DeviceB syncs + uploads INITIAL (SDK stays active, NO teardown)
Phase 3: DeviceA advances cloud (CLOUD_V2), DisableNetwork at END
Phase 4: DeviceB writes DIVERGENT data offline → Terminate process
Phase 5: Relaunch (still offline) → EnableNetwork → SmokeDelay(3s) → AddUser
         → CONFLICT FIRES ✅
```

**Critical design rules:**

1. **Each device MUST use a different save folder.** DeviceA uses `C:\gamesaves-test\DeviceA\`, DeviceB uses `C:\gamesaves-test\DeviceB\`. Shared folders cause `DeleteLocalFolder` cross-contamination.

2. **Do NOT `DeleteLocalFolder` before conflict-triggering `AddUserWithUiAsync`.** GRTS needs stale local data to detect divergence.

3. **Keep DeviceB offline from Phase 3 through relaunch.** If DeviceB has network during the write or after Terminate, GRTS may upload in the background and clear dirty state before AddUser — no conflict.

4. **Enable network immediately before AddUser with a 3s XAL delay.** This minimizes the window for GRTS to upload before AddUser runs.

5. **Do NOT call `StopGamingServices`.** It's machine-wide and kills GRTS for ALL titles. Just use `DisableNetwork` to prevent GRTS uploads.

#### Why Conflicts Fail to Trigger

| Symptom | Root Cause | Fix |
|---------|-----------|-----|
| `conflictVersion: (EMPTY)` in ETL | GRTS uploaded + got 409 → session cleared (Rule 3) | Keep device offline during divergent write |
| `conflictVersion: (EMPTY)`, no upload retry | `DeleteLocalFolder` wiped divergent data | Remove premature `DeleteLocalFolder` |
| `conflictVersion: (EMPTY)`, upload succeeded | GRTS uploaded dirty data before AddUser (race) | Minimize window between EnableNetwork and AddUser |
| Conflict fires but wrong resolution | Auto-response not set in YAML | Set `conflictAutoResponse: UseCloud` (or UseLocal) |

### 24.6 Version Tracking

GRTS tracks versions via `PFActivationVersions` in the context file:

| Field | Meaning |
|-------|---------|
| `LatestRemote` | Latest version from server (from ListManifests) |
| `UploadVersion` | Version being/last uploaded |
| `LastConflictWinner` | Version that won last conflict |
| `LastConflictLoser` | Version that lost last conflict |
| `PendingConflictIsWinner` | `1` if next upload carries isWinner flag |
| `PendingConflictVersion` | Conflict reference version (consumed during upload) |
| `SyncStatus` | `0`=BROKEN, `1`=SYNCHRONIZED, `2`=NOT_SYNCHRONIZED |

**Version numbers are monotonically increasing** and assigned by the PlayFab service. GRTS consumes 3-5 version numbers per test run:
- `AddUserWithUi`: allocates version N
- `UploadWithUi(KeepDeviceActive)`: uploads as N, allocates N+1
- Failed upload (409): consumes a version, cleared on abandon
- Next `AddUserWithUi`: allocates N+2 or N+3

### 24.7 On-Disk State Layout

GRTS stores all state under `C:\XboxGames\GameSave\pgs\`:

```
pgs/
├── t_{XUID}_{TitleID}/           # Title-level temp data (sync plans)
│   └── {Version}-sync/           # Download plan for a version
├── u_{XUID}_{TitleID}/           # User-level (save data + manifests)
│   ├── {XUID}_{TitleID}.json     # Context state file (versions, flags)
│   ├── {Version}.json            # Manifest per version (status, chunks)
│   ├── extended-{Version}-manifest.json  # File tree per version
│   └── {Version}/                # Actual save data (blob cache)
│       └── {folder}/{file}       # Game save files
```

**GRTS keeps multiple manifest versions** on disk (e.g., v498, v574, v609). This supports rollback, conflict resolution, and power-cycle recovery.

**The context file** (`{XUID}_{TitleID}.json`) is the most important diagnostic artifact. It records:
- Current upload version and sync status
- Conflict resolution history (`LastConflictWinner`, `LastConflictLoser`)
- **The stomp flag** (`PendingConflictIsWinner`) — check this when debugging isWinner issues

### 24.8 WaitForGameSaveSync Limitations

`WaitForGameSaveSync` polls GRTS status and returns `InSync=true` when GRTS believes its last committed state matches the cloud. However:

- It reflects the state of the **last completed upload**, not current save folder contents
- If the game writes data and is immediately terminated, GRTS may not have detected the dirty state yet
- In test 125 v9, `WaitForGameSaveSync` returned `InSync=true` in 76ms after Terminate — GRTS hadn't detected the write yet

**Use `WaitForGameSaveSync` only to confirm a previous SDK-driven upload has synced.** It cannot confirm that GRTS has detected and uploaded background file changes.

---

## References

- **ETL reading script**: `C:\git\PlayFab.C\Utilities\Scripts\read-grts-etl.py`
- **GRTS debugging scripts**: `C:\git\PlayFab.C\Utilities\Scripts\grts-*.ps1` (enable-dumps, disable-dumps, crash-report, collect-dump, reinstall)
- **GRTS internals guide**: `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\ai-grts-internals.md`
- **Telemetry event spec**: `C:\git\PlayFab.C\specs\playfab-gamesave\design\telemetry-spec.md`
- **Telemetry codebase review**: `C:\git\PlayFab.C\specs\playfab-gamesave\telemetry-review.md`
- **Kusto query library**: `.squad/skills/kusto-telemetry/queries/`
- **Kusto skill reference**: `.squad/skills/kusto-telemetry/SKILL.md`
- **Test output directory**: `C:\git\PlayFab.C\Out\gamesave-pc-tests\` (pass directories with per-test log files)
- **Golden path test logs**: `C:\git\PlayFab.C\Out\gamesave-pc-tests\pass154\01\` (device-DeviceA-log.txt is the primary inproc log)
- **AdminHelper source**: `Test\AdminHelper\Program.cs` (network/service control via named pipe)
- **DeviceStateController**: `DeviceStateController.cs` (FlushGrtsAndGoOffline, network actions)
- **ConflictResolutionBugMitigation doc**: linked from `ConflictResolutionBugMitigation.md` in service repo
- **Benjamin Dow ETL analysis**: `investigations/05-08-forte-investigation/user-reports/Benjamin_Dow_etl_files/etl-analysis.md`
- **isWinner 5-step failure report**: `investigations/05-08-forte-investigation/reports/forte-iswinner-5step-failure.md`
- **Rakesh workaround explained**: `investigations/05-08-forte-investigation/reports/rakesh-workaround-explained.md`
- **isWinner non-conflict analysis**: `investigations/05-08-forte-investigation/reports/iswinner-non-conflict-analysis.md`
- **Conflict test ETL (xplat-14)**: `investigations/05-08-forte-investigation/reports/conflict-test-14-etl.txt` (920 lines, 140 events)
- **Conflict test logs (xplat-14)**: `C:\git\PlayFab.C\Out\gamesave-pc-tests\pass155\14\` (DeviceA 145KB GRTS, DeviceB 280KB inproc)
- **isWinner stomp ETL (test 125 v9)**: `investigations/test125-v9-etl.txt` (232 events, UploadOptions=1 evidence)
- **isWinner stomp test**: `Test\GameTestScenarios\gamesave-pc\gamesave-pc-125-takeremote-next-save-iswinner-stomp.yml`
- **XAL types header (E_XAL_NETWORK)**: `C:\Program Files (x86)\Microsoft GDK\260400\GRDK\ExtensionLibraries\Xbox.Services.API.C\Include\Xal\xal_types.h:64`
- **ConnectedStorage source**: `C:\git\ConnectedStorage` (GRTS service source — see `ai-grts-internals.md` for annotated walkthrough)

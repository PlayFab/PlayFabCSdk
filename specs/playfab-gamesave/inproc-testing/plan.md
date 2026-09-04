# XPlat GameSave Test Audit Plan

## Goal

Run every test in `Test/GameTestScenarios/gamesave-pc/` one at a time, capture results, and produce a status report documenting pass/fail/skip for each scenario, with failure analysis and log evidence proving correctness.

## Scope

- **112 scenarios** in `gamesave-pc/`
- **40 single-device** (DeviceA only, engine: xbox or pc-grts)
- **72 two-device** (DeviceA: xbox/pc-grts, DeviceB: pc-inproc-gamesaves)
- All tagged `gamesave-pc`
- Run with `-Tag gamesave-pc -Scenarios NN`

## Rules

These rules are mandatory for all sessions continuing this audit.

### Rule 1: One test at a time, in order — NO BATCHING

Run tests in strict scenario-number order (01, 02, 03, ...). **Never batch multiple tests in one run.** Each test must complete (pass, fail, or timeout) before starting the next. This is critical for clean log isolation and accurate reporting.

### Rule 2: Clean environment per test

Before each test:
1. Kill any lingering `GameTestController` and `GameTestAppWindows` processes
2. Delete all files in `Out/logs/`
3. Then run: `py tests-run.py gamesave-pc --only <NN> --stop-on-fail`

### Rule 3: Update the report after EVERY test

After each test, **immediately** update `status-report.md` before moving to the next test. Never batch updates. The report is the source of truth and must always reflect the latest state.

### Rule 4: Log evidence for passing tests

A "PASS" is not just "all commands returned hr=0x00000000". For each passing test:

1. **Read the YAML scenario** to understand what the test claims to verify.
2. **Read the controller log** to confirm the interesting behavior actually happened.
3. **Add a Log Evidence subsection** (at the bottom of the report, under `## Log Evidence`) with:
   - **Claim**: What the test says it tests (from the YAML `description` field)
   - **Key log lines**: Extracted from `controller.log` showing the behavior occurred. Include:
     - The setup (init, write, upload)
     - The interesting event (contention fired, conflict resolved, error returned, etc.)
     - The verification (snapshot comparison result, expected error matched, etc.)
   - **Verdict**: Does the test actually prove what it claims? Note any gaps (e.g., "no content hash check", "weak verification").

The evidence section uses this format:
```markdown
### Test NN

**Claim**: <from YAML description>

**Key log lines**:
\```
<extracted log lines>
\```

**Verdict**: ✅ / ⚠️ description of confidence level and gaps
```

### Rule 5: Failure documentation

For failing tests, update the table row with:
- The specific command that failed
- The HRESULT returned vs. expected (if applicable)
- A one-line root cause if obvious

Also update the **Known Failure Patterns** table if the failure matches or creates a pattern.

**Exception — trivial YAML/scenario bugs:** If the failure is caused by a trivial bug in the test YAML itself (not an SDK or handler bug), **fix the YAML and re-run the test** rather than just documenting it. Examples of trivial YAML bugs: wrong verb name, missing required parameter, wrong field name (`expectedError` vs `expectedHr`), typo in a parameter value. After fixing, reset the test to ⬜ TODO, re-run, and update the report with the new result. See Rule 10 for the catalog of known YAML bugs.

**Do NOT fix** SDK bugs, handler implementation gaps, or test logic issues during the audit — just document those.

### Rule 6: Timeout handling

If a test runs for more than 10 minutes with no progress (check `controller.log` tail), kill it and record as FAIL with the reason (e.g., "WaitForGameSaveSync stuck — uploadedBytes=0 after 490 polls").

### Rule 7: Summary counts

Keep the summary table at the top of the report accurate after every update:
```markdown
| Status | Count | Description |
|--------|-------|-------------|
| ✅ PASS | N | ... |
| ❌ FAIL | N | ... |
| ⏭️ SKIP | N | ... |
| ⬜ TODO | N | ... |
```

The four counts must always sum to 112.

### Rule 8: ChangeTargetDeviceState tests → SKIP

Tests that use `ChangeTargetDeviceState` cannot run on a non-admin PC. Mark them ⏭️ SKIP. There are 55 such tests. The status report has a dedicated section documenting what each skipped test needs `ChangeTargetDeviceState` to do (network control, suspend/resume, standby, terminate, etc.) so a future session can evaluate alternatives.

### Rule 9: Update YAML `pc-grts:` tag when tests pass

When a test passes, update its YAML file's `platforms:` section:
```yaml
platforms:
  pc-grts: passing    # was 'ignore'
```
This marks the test as known-passing on the PC-GRTS engine. Only update after confirming a pass with log evidence.

### Rule 10: YAML bug fixes

When a test fails due to a YAML scenario bug (not an SDK bug), fix the YAML and re-run:

**Known YAML patterns that needed fixing:**
- `verb: CreateBinaryFile` with `content:` field → change verb to `CreateTextFile` (the handler supports `content:` only on `CreateTextFile`, not `CreateBinaryFile`)
- `CaptureSaveContainerSnapshot` missing `slot` parameter → add `parameters: { slot: left }` or `slot: right`
- `bytesMin:`/`bytesMax:` → replace with single `bytes:` value (handler doesn't support range)
- `expectedError: "SYMBOLIC_NAME"` → use `expectedHr: "0xHEXVALUE"` (controller only supports `expectedHr`)

After fixing YAML, reset the test to ⬜ TODO in the report, re-run, and update with new results.

### Rule 11: Take your time

This is a long, methodical process. Don't rush. Read logs carefully. Extract meaningful evidence. Update the report accurately. Quality over speed.

## Execution Commands

```powershell
# Kill leftover processes
$procs = Get-Process GameTestController, GameTestAppWindows -ErrorAction SilentlyContinue
foreach ($p in $procs) { Stop-Process -Id $p.Id -Force -ErrorAction SilentlyContinue }

# Clean logs
Remove-Item "C:\git\PlayFab.C\Out\logs\*" -Force -ErrorAction SilentlyContinue

# Run a single test (replace NN with scenario number)
cd C:\git\PlayFab.C
py Utilities\Scripts\tests-run.py gamesave-pc --only NN --stop-on-fail

# Read logs after test
pwsh -ExecutionPolicy Bypass -File Utilities\Scripts\read-test-logs.ps1
```

### Evidence extraction commands

```powershell
# Get command completion flow (the main evidence source)
Select-String -Path "C:\git\PlayFab.C\Out\logs\controller.log" -Pattern "Device[AB]:.*\[Completed\]"

# Get key operations (write, upload, snapshot, compare)
Select-String -Path "C:\git\PlayFab.C\Out\logs\controller.log" -Pattern "WriteGameSaveData|UploadWithUi|AddUserWithUi.*\[Completed|CaptureSave|Compare"

# Get snapshot comparison results
Select-String -Path "C:\git\PlayFab.C\Out\logs\controller.log" -Pattern "snapshotComparison|CompareSave"

# Find the specific failure point
Select-String -Path "C:\git\PlayFab.C\Out\logs\controller.log" -Pattern "(Scenario.*failed|status=failed|timed out)" -Context 2,2

# Check YAML test description
Select-String -Path "C:\git\PlayFab.C\Test\GameTestScenarios\gamesave-pc\gamesave-pc-NN-*.yml" -Pattern "description:"

# Validate YAML syntax (use node + yaml package)
node -e "const yaml = require('yaml'); const fs = require('fs'); yaml.parse(fs.readFileSync('path/to/file.yml','utf8')); console.log('OK');"
```

## Output Files

- `specs/playfab-gamesave/xplat-testing/status-report.md` — full audit results with evidence
- `specs/playfab-gamesave/xplat-testing/plan.md` — this file (rules and approach)

## Environment

- PC with GRTS available (Gaming Services installed)
- `ForceUseInprocGameSaves` NOT set (allows GRTS engine selection)
- Custom ID prefix: `KSNOOPY-HOME2-` (prevents entity collisions)
- Non-admin (tests requiring admin for network manipulation will fail)
- Controller auto-launches devices with correct engine flags (no `--no-auto-launch`)

## Progress

Full audit complete. All 112 tests processed.

Current totals: **22 PASS, 34 FAIL, 56 SKIP, 0 TODO**.

### YAML bugs fixed and re-run (Round 2)
- 11 files: `CreateBinaryFile` → `CreateTextFile` for `content:` fields (tests 14-16, 18-24, 35)
- 3 files: Added `slot` parameter to `CaptureSaveContainerSnapshot` (tests 71, 73, 74)
- 1 file: `bytesMin`/`bytesMax` → `bytes: 51200` (test 103)
- 1 file: `expectedError:` → `expectedHr: "0x80830010"` (test 13)
- Result: 3 additional passes (18, 71, 103), rest exposed underlying SDK/handler issues

### Key findings
- **55 tests** require `ChangeTargetDeviceState` (admin-only OS simulation) → SKIP
- **1 test** requires Xbox engine → SKIP
- **Contention on two-device conflict** is the most common real failure (tests 14, 15, 19-22)
- **Several handler commands not implemented**: `PFGameSaveFilesSetUiSyncConflictAutoResponse`, `PFGameSaveFilesSetUiProgressStateRecording`, `VerifyContentionDescriptor`, `VerifyQuotaDelta`, `SetExpiredEntityToken`, `PFGameSaveFilesSetUiProgressAutoWriteOnUpload`
- **PFLocalUserLoginAsync rejection** blocks all cross-platform two-device tests (76, 108, 109)

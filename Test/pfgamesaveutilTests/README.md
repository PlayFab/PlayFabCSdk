# pfgamesaveutil Integration Tests

Integration tests that drive the real `pfgamesaveutil` executable against a live
PlayFab title to verify the **upload** and **download** commands (including the
new `downloadall` layout and multi-zip splitting).

These are **integration** tests — they require PlayFab credentials supplied via
environment variables. Without a secret key the cloud tests are **skipped** (and
the reason is logged), so the suite is safe to run in any environment.

## Cross-tool transfer tests (pfgamesaveutil <-> inproc SDK)

`test_inproc_transfer.py` proves that a save transfers intact between
`pfgamesaveutil` and the inproc SDK (files **and** subfolders), in both directions.
It drives the GameTest harness via `Utilities/Scripts/tests-run.py --only` and runs
two scenarios in `Test/GameTestScenarios/gamesave-interactive/`:

| Test | Direction | Scenario | Verification |
|------|-----------|----------|--------------|
| `TransferTests.test_inproc_upload_util_download` | inproc uploads → pfgamesaveutil downloads | `gamesave-interactive-05` | Python compares the downloaded folder to the source by SHA-256 (files + subfolders). |
| `TransferTests.test_util_upload_inproc_download` | pfgamesaveutil uploads → inproc downloads | `gamesave-interactive-04` | The scenario snapshot-compares the downloaded container against the source folder. |

Both sides operate on the **same player**: the harness player's
`title_player_account` entity id is read from the scenario logs via
`get-player-id.py` (the same approach `tests-run.py` uses), then passed to
`pfgamesaveutil`. The source datasets are written by Python to
`C:\temp\pfgs-xfer\...`, which the scenarios load via `CopyTargetFolderToSaveFolder`
— a single source of truth for both tools.

**Extra prerequisites** (else these tests skip with a message): a built
`GameTestController.exe` and `GameTestAppWindows.exe` (the inproc harness).
Only `PFSECRETKEY` is needed for credentials — the player is auto-resolved from logs.

## Prerequisites

1. Build/publish the tool:
   ```
   Tools\pfgamesaveutil\publish.cmd
   ```
2. Set the title secret key (REQUIRED):
   ```powershell
   $env:PFSECRETKEY = "<your title secret key>"
   ```
3. Specify the test player (one of):
   ```powershell
   $env:PFPLAYERID = "A036BDD70192AEC6"        # title_player_account entity ID
   # or
   $env:PFMASTERPLAYERID = "<master player id>"
   ```

## Environment variables

| Variable | Required | Default | Purpose |
|----------|----------|---------|---------|
| `PFSECRETKEY` | **Yes** | — | PlayFab title secret key. If unset, cloud tests are skipped with a logged message. |
| `PFPLAYERID` | One of these | — | Player's `title_player_account` entity ID. |
| `PFMASTERPLAYERID` | One of these | — | Player's `master_player_account` ID (used instead of `PFPLAYERID`). |
| `PFTITLEID` | No | `E18D7` | PlayFab title ID. |
| `PFGSUTIL_EXE` | No | `Tools/pfgamesaveutil/bin/publish/pfgamesaveutil.exe` | Path to the executable. |
| `PFGS_ALLOW_RESET` | No | (off) | Set to `1` to let the suite delete the player's cloud saves during cleanup. |

> **Note:** The tests create new save versions for the configured player. They do
> not delete data unless `PFGS_ALLOW_RESET=1` is set.

## Running

```powershell
# Convenience runner (clean [PASSED]/[FAILED]/[SKIPPED] output)
Test\pfgamesaveutilTests\run-tests.cmd

# Or run the runner directly
py Test\pfgamesaveutilTests\run_tests.py

# Or via the test file
py Test\pfgamesaveutilTests\test_upload_download.py
```

The runner prints one line per test:

```
[PASSED]  LocalVerbTests.test_collect_local
[SKIPPED] CloudVerbTests.test_upload_download_round_trip - PFSECRETKEY ... not set
...
Total: 11   Passed: 5   Failed: 0   Errors: 0   Skipped: 6
RESULT: PASS
```

Cloud (end-to-end) tests show as `[SKIPPED]` until `PFSECRETKEY` (and a player id)
are set — so it is always clear that the passing tests are the local ones, not the
e2e upload/download tests.

## What is covered

At least one test per verb. Each test is reported individually as
`[PASSED]` / `[FAILED]` / `[SKIPPED]`.

| Verb | Test | What it verifies | Needs creds? |
|------|------|------------------|--------------|
| `upload` + `download` | `CloudVerbTests.test_upload_download_round_trip` | Upload a folder, download it, and confirm the downloaded folder **exactly matches** the upload (SHA-256 per file). | Yes |
| `upload` (multi-zip) | `CloudVerbTests.test_multizip_round_trip` | `--max-zip-mb 1` splits a large folder into multiple zip chunks and still round-trips. | Yes |
| `downloadall` | `CloudVerbTests.test_downloadall_layout` | Writes `list-manifest.json` + `v{N}/extracted/` matching the upload. | Yes |
| `info` | `CloudVerbTests.test_info_json` | `info --json` emits parseable JSON containing `manifests`. | Yes |
| `compare` | `CloudVerbTests.test_compare` | Runs cloud-vs-local comparison and reports without error. | Yes |
| `reset` (cloud) | `CloudVerbTests.test_reset_cloud` | `reset --cloud` removes all manifests (opt-in via `PFGS_ALLOW_RESET=1`). | Yes |
| `status` | `LocalVerbTests.test_status_local` | Reads a local folder + registry and prints a status report. | No |
| `collect` | `LocalVerbTests.test_collect_local` | Zips a local PGS folder into a `PGS-*.zip` containing the data + `_device-status.txt`. | No |
| `reset` (local) | `LocalVerbTests.test_reset_local` | `reset --local` deletes the targeted folder. | No |
| (smoke) | `SmokeTest.test_exe_present_and_help` | All verbs appear in `--help`. | No |
| (smoke) | `SmokeTest.test_missing_secret_is_reported` | A cloud verb reports a clear error when `--secret-key` is absent. | No |

The local verbs (`status`, `collect`, `reset --local`) and the smoke tests run
without credentials. If `PFSECRETKEY` (or a player id) is missing, only the cloud
tests are skipped — with a message explaining what to set.

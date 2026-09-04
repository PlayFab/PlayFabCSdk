# pfgamesaveutil

`pfgamesaveutil` is a command-line tool for working with PlayFab Game Save data. Use it to download cloud saves, inspect player save state, compare cloud and local data, collect local diagnostics, and reset save data during development.

`pfgamesaveutil` runs on a development PC and communicates with the PlayFab Game Save service APIs. It requires a PlayFab title ID and developer secret key for cloud operations, and can also inspect local game save folders without cloud access.

Use `pfgamesaveutil` to run the commands listed in the following table.

| Command | Description |
|---|---|
| [pfgamesaveutil download](#pfgamesaveutil-download) | Downloads a player's cloud saves to a local directory. |
| [pfgamesaveutil info](#pfgamesaveutil-info) | Displays information about a player's cloud save state. |
| [pfgamesaveutil compare](#pfgamesaveutil-compare) | Compares cloud save data with local files to verify sync status. |
| [pfgamesaveutil reset](#pfgamesaveutil-reset) | Deletes cloud save data, local save data, or both. |
| [pfgamesaveutil collect](#pfgamesaveutil-collect) | Packages local save data into a ZIP archive for debugging. |
| [pfgamesaveutil status](#pfgamesaveutil-status) | Displays local device status and GamingServices registry settings. |

## Prerequisites

- .NET 8.0 runtime (Windows x64)
- A PlayFab title ID
- A title developer secret key (from [PlayFab Game Manager](https://developer.playfab.com/)) — required for cloud operations
- A player entity ID (`title_player_account`) or PlayFab ID (`master_player_account`) — required for cloud operations

## Installation

Build and publish from source:

```
cd Tools\pfgamesaveutil
dotnet publish -c Release -o bin\publish
```

The output is a single-file executable at `bin\publish\pfgamesaveutil.exe`.

## Common options

The following options are available to all commands.

| Option | Description |
|---|---|
| `--title-id <id>` | **(Required)** The PlayFab title ID (for example, `ABCDE`). |
| `--secret-key <key>` | **(Required for cloud operations)** The title developer secret key from PlayFab Game Manager. |
| `--title-player-id <id>` | **(Required for cloud operations)** The player's `title_player_account` entity ID. Specify this or `--master-player-id`, not both. |
| `--master-player-id <id>` | The player's `master_player_account` PlayFab ID. If specified, the tool automatically resolves it to a `title_player_account` entity ID. |
| `--local-path <path>` | Path to the local PGS folder. Defaults to `auto`, which searches all fixed drives for `<drive>:\XboxGames\GameSave\pgs\*_<titleId>`. |

## Downloading player saves

### pfgamesaveutil download

Downloads a player's cloud saves to a local directory. The tool authenticates with PlayFab, retrieves the player's manifest list, downloads the extended manifest and all compressed chunk blobs, then extracts the original save files.

```
pfgamesaveutil download --title-id <id> --secret-key <key> --title-player-id <entityId> [--local-path <path>]
```

| Option | Description |
|---|---|
| `--title-id` | **(Required)** The PlayFab title ID. |
| `--secret-key` | **(Required)** The title developer secret key. |
| `--title-player-id` | **(Required)** The player's entity ID. |
| `--local-path` | The directory to write save files to. Defaults to `auto` (auto-detected PGS folder). |

By default, the latest finalized manifest version is downloaded. The extracted output directory contains the reconstructed save files in their original folder structure, plus a `cloudsync\` subfolder with metadata:

```
<output>\
├── <save-files>                           # Original game save files
└── cloudsync\
    ├── extended-<version>-manifest.json   # Extended manifest from cloud
    ├── service-manifest-<version>.json    # Download details response
    └── info.json                          # Player state summary
```

Example:

```
pfgamesaveutil download --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

PlayFab Game Save Downloader
Title: ABCDE
Player: 0123456789ABCDEF
Output: C:\Users\dev\saves

Authenticating with PlayFab...
Authenticated as: title (abc123)

Fetching manifests...
Found 3 manifest(s):
  - Version 143 (status: Finalized, finalized: 2026-01-15T10:30:00Z)
  - Version 142 (status: Finalized, finalized: 2026-01-14T08:15:00Z)
  - Version 141 (status: Initialized, finalized: )

Downloading manifest version 143...
Downloading extended manifest...
Downloading chunks...
  [1/2] chunk-001.zip: 15.2 KB / 15.2 KB
  [2/2] chunk-002.zip: 5.1 KB / 5.1 KB

Extracting to: C:\Users\dev\saves
  Created folder: SaveData
  Extracted: profile.dat
  Extracted: SaveData\slot1.sav
  Extracted: SaveData\slot2.sav

Done!
```

[Return to the top of this topic.](#pfgamesaveutil)

## Viewing save information

### pfgamesaveutil info

Displays information about a player's cloud save state, including manifest count and status, file and chunk counts, compressed and uncompressed storage sizes, and quota usage.

```
pfgamesaveutil info --title-id <id> --secret-key <key> --title-player-id <entityId> [--json] [--verbose]
```

| Option | Description |
|---|---|
| `--title-id` | **(Required)** The PlayFab title ID. |
| `--secret-key` | **(Required)** The title developer secret key. |
| `--title-player-id` | **(Required)** The player's entity ID. |
| `--json` | Output in JSON format for scripting. |
| `--verbose` | Include per-manifest breakdown. |

Example (default output):

```
pfgamesaveutil info --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

Player: 0123456789ABCDEF
Manifests: 4 (2 Initialized, 2 Finalized)
Total Files: 3
Total Chunks: 3
Storage: 20.3 KB compressed / 28.0 KB uncompressed (1.4x ratio)

Quota:
  Used: 20.3 KB / 256.0 MB (0.0%)
  Available: 256.0 MB
```

Example (`--json` output):

```json
{
  "playerId": "0123456789ABCDEF",
  "summary": {
    "manifestCount": 4,
    "statusBreakdown": { "Initialized": 2, "Finalized": 2 },
    "totalFiles": 3,
    "totalChunks": 3,
    "totalCompressedBytes": 20787,
    "totalUncompressedBytes": 28672
  },
  "quota": {
    "totalBytes": 268435456,
    "usedBytes": 20787,
    "availableBytes": 268414669,
    "usedPercent": 0.008
  },
  "manifests": [ ... ],
  "healthIssues": []
}
```

[Return to the top of this topic.](#pfgamesaveutil)

## Comparing cloud and local data

### pfgamesaveutil compare

Compares the cloud save manifest with files on the local device to determine whether they are in sync. The tool downloads the cloud extended manifest, reads the local PGS folder, and reports file-by-file differences.

```
pfgamesaveutil compare --title-id <id> --secret-key <key> --title-player-id <entityId> [--local-path <path>]
```

| Option | Description |
|---|---|
| `--title-id` | **(Required)** The PlayFab title ID. |
| `--secret-key` | **(Required)** The title developer secret key. |
| `--title-player-id` | **(Required)** The player's entity ID. |
| `--local-path` | Path to the local PGS folder, or `auto` (default) to auto-detect. |

The comparison reports:
- **Matching files** — present in both cloud and local with matching sizes.
- **Cloud-only files** — present in the cloud manifest but missing locally.
- **Local-only files** — present on disk but not in the cloud manifest.
- **Size mismatches** — present in both but with different sizes.

Example:

```
pfgamesaveutil compare --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

PlayFab Game Save - Cloud vs Local Comparison
Title: ABCDE
Player: 0123456789ABCDEF

Auto-detected local folder: C:\XboxGames\GameSave\pgs\u_123456789_ABCDE

Fetching cloud manifests...
Comparing cloud version: 143

======================================================================
                              COMPARISON RESULTS
======================================================================

Versions:
  Cloud:  143
  Local:  N/A (comparing files directly)

Files:
  Cloud manifest:  4 files
  Local manifest:  0 files
  Local disk:      4 files

Comparison:
  ✓ 4 files match

  ✓ Cloud and local are in sync!
```

[Return to the top of this topic.](#pfgamesaveutil)

## Deleting save data

### pfgamesaveutil reset

Deletes save data. Use scope options to target cloud data, local data, or both.

```
pfgamesaveutil reset --title-id <id> --secret-key <key> --title-player-id <entityId> [--cloud] [--local] [--force] [--local-path <path>]
```

| Option | Description |
|---|---|
| `--title-id` | **(Required)** The PlayFab title ID. |
| `--secret-key` | **(Required for cloud reset)** The title developer secret key. |
| `--title-player-id` | **(Required for cloud reset)** The player's entity ID. |
| `--cloud` | Delete all cloud manifests for the player. This is the default if no scope is specified. |
| `--local` | Delete the local PGS folder for the title. Does not require `--secret-key` or `--title-player-id` when used alone. |
| `--force` | Skip the confirmation prompt. |
| `--local-path` | Path to the local PGS folder, or `auto` (default) to auto-detect. |

> **Caution:** This command permanently deletes save data. The operation cannot be undone.

When deleting cloud data, the tool iterates through all manifests and deletes those in `Initialized`, `Uploading`, or `Finalized` status. Manifests in `Quarantined` or `PendingDeletion` status are skipped.

Examples:

```
:: Reset cloud data only (default)
pfgamesaveutil reset --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

:: Reset local data only (no cloud credentials needed)
pfgamesaveutil reset --title-id ABCDE --local

:: Reset both cloud and local, skip confirmation
pfgamesaveutil reset --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --cloud --local --force
```

[Return to the top of this topic.](#pfgamesaveutil)

## Collecting local diagnostics

### pfgamesaveutil collect

Packages the local PGS folder into a timestamped ZIP archive for sharing with support or other developers. This command does not access the cloud and does not require a secret key or player ID.

```
pfgamesaveutil collect --title-id <id> [--local-path <path>]
```

| Option | Description |
|---|---|
| `--title-id` | **(Required)** The PlayFab title ID. |
| `--local-path` | Path to the local PGS folder, or `auto` (default) to auto-detect. |

The output ZIP file is created in the current working directory with the naming pattern `PGS-<folderName>-<yyyyMMdd-HHmmss>.zip`. The archive includes all files from the PGS folder plus a `_device-status.txt` file containing:

- Machine name and user name
- PGS folder path, file count, and total size
- Local manifest versions
- Windows Registry status for GamingServices keys

Example:

```
pfgamesaveutil collect --title-id ABCDE

PlayFab Game Save - Collect Local Data
Title: ABCDE

Auto-detected local folder: C:\XboxGames\GameSave\pgs\u_123456789_ABCDE
Found 12 files (5.3 MB)

Creating archive: C:\Users\dev\PGS-u_123456789_ABCDE-20260215-143022.zip
Archive created: 2.1 MB

✓ Local PGS data collected to: C:\Users\dev\PGS-u_123456789_ABCDE-20260215-143022.zip
```

[Return to the top of this topic.](#pfgamesaveutil)

## Viewing local device status

### pfgamesaveutil status

Displays local device information relevant to PlayFab Game Save, including PGS folder status and GamingServices registry settings. This command does not access the cloud. It can also modify GamingServices registry values when toggle options are specified.

```
pfgamesaveutil status --title-id <id> [--local-path <path>] [--inproc <true|false>] [--local-services <true|false>] [--trace <true|false>]
```

| Option | Description |
|---|---|
| `--title-id` | **(Required)** The PlayFab title ID. |
| `--local-path` | Path to the local PGS folder, or `auto` (default) to auto-detect across all fixed drives. |
| `--inproc <true\|false>` | Set the `ForceUseInprocGameSaves` registry value. `true` enables the in-process game save provider; `false` removes the value. Requires Administrator. |
| `--local-services <true\|false>` | Set the `ForceUseLocalServices` registry value. `true` enables local services mode; `false` removes the value. Requires Administrator. |
| `--trace <true\|false>` | Set the `TraceToDebugger` registry value. `true` enables debug tracing for GamingServices auth; `false` removes the value. Requires Administrator. |

The status display includes three sections:

**PGS Folder Status** — Shows all PGS base paths found on fixed drives, subfolders matching the title ID, file counts, total sizes, manifest versions present, and contents of the `current\` subfolder.

**Registry Status** — Reports whether the `HKLM\SOFTWARE\Microsoft\XGameSaveStorage\PlayFab` key exists.

**GamingServices Settings** — Shows the current state of three registry values under `HKLM\SOFTWARE\Microsoft\GamingServices`:
- `ForceUseInprocGameSaves` — Controls whether the in-process game save provider is used instead of the out-of-process GRTS provider.
- `ForceUseLocalServices` — Controls whether local GamingServices are forced.
- `TraceToDebugger` — Controls debug trace output for GamingServices authentication (under `GamingServices\Auth`).

Example (read-only):

```
pfgamesaveutil status --title-id ABCDE

PlayFab Game Save - Local Device Status
Title: ABCDE

======================================================================

PGS Folder Status:
  Base path(s): C:\XboxGames\GameSave\pgs
  u_123456789_ABCDE
    Files: 5, Size: 20.0 MB
    Manifest versions: 106
    current/ subfolder: 1 files

Registry Status:
  XGameSaveStorage\PlayFab: EXISTS

GamingServices Settings:
  ForceUseInprocGameSaves: NOT SET
  ForceUseLocalServices: NOT SET
  TraceToDebugger: NOT SET

Tip: Modify registry settings with:
  --inproc <true|false>          Set ForceUseInprocGameSaves
  --local-services <true|false>  Set ForceUseLocalServices
  --trace <true|false>           Set TraceToDebugger

======================================================================
```

Example (modify registry):

```
:: Run as Administrator
pfgamesaveutil status --title-id ABCDE --inproc true --trace true

Modifying registry settings:
  Set ForceUseInprocGameSaves = 1
  Set TraceToDebugger = 1

PlayFab Game Save - Local Device Status
...
GamingServices Settings:
  ForceUseInprocGameSaves: ENABLED (1)
  ForceUseLocalServices: NOT SET
  TraceToDebugger: ENABLED (1)
```

[Return to the top of this topic.](#pfgamesaveutil)

## Local PGS folder layout

On Windows, PlayFab Game Save stores data locally at:

```
<drive>:\XboxGames\GameSave\pgs\
```

Within this directory, each user/title combination has a subfolder named `u_<userId>_<titleId>`. The subfolder contains:

| Path | Description |
|---|---|
| `extended-<version>-manifest.json` | Local copy of the extended manifest for a synced version. |
| `current\` | Subfolder containing the active (extracted) save files. |
| `<version>\` | Subfolder containing compressed chunk files for a specific version. |

The `--local-path auto` option searches all fixed drives for this directory structure and matches subfolders by title ID.

## Security notes

- The title developer secret key provides full administrative access to all player data within the title. Treat it as a credential — do not commit it to source control or include it in logs.
- For scripting, pass the secret key via environment variable:
  ```powershell
  pfgamesaveutil download --title-id ABCDE --secret-key $env:PLAYFAB_SECRET_KEY --title-player-id $env:PLAYER_ID
  ```
- Registry modifications require running the tool as Administrator. The tool reports an "Access denied" error if privileges are insufficient.

## Limitations

- Download only — save data cannot be uploaded to the cloud with this tool.
- Only finalized manifests contain complete downloadable data.
- Supports `zip` and `gzip` compression formats.
- Windows only.

## Incident tooling (not on `main`)

> Internal note. This spec is not packaged with the tool; `Tools/pfgamesaveutil/README.md` is
> (the csproj copies it next to the published exe and the build zips it for partner sharing),
> so incident detail belongs here rather than there.

Three commands and a set of hidden fault-injection flags were built during a 2026 save-corruption
investigation and are deliberately kept off `main`. They live on the branch
`amccalib/gamesave-unwedge-incident` (tip `dc07a146`).

| Command | What it does |
| --- | --- |
| `unwedge` | Rolls a player back to the newest version whose blobs all still exist. Dry run by default; writes a CSV ledger. |
| `diagnose` | Read-only per-file blob census across a set of players — which files are missing, not just whether any are. |
| `rebuild` | Reconstructs a healthy version from surviving payload archives and lands it at head, instead of wiping the player's cloud. |
| `upload --omit-extended-manifest`, `upload --abandon-after-initialize` | Fault injection that deliberately corrupts a save to reproduce the defect. Gated behind `PFGS_ALLOW_CORRUPT=1`. |

Why they are not on `main`:

- They repair a client defect that is now fixed, so they are point-in-time remediation rather than
  part of this tool's steady-state surface.
- The fault-injection flags destroy player save state by design and must never reach a partner or
  be pointed at a production title. Documenting them in a packaged file would defeat the gating.

Bug 63475452 and Deliverable 63588076 carry the investigation, the remediation campaign, and the
runbook. If the branch is ever pruned, the commit is still reachable by SHA.

## See also

- [PlayFab Game Save overview](https://learn.microsoft.com/en-us/gaming/playfab/features/data/gamesave/)
- [PlayFab Game Manager](https://developer.playfab.com/)

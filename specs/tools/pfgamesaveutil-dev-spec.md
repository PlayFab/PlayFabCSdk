# pfgamesaveutil — Development Specification

## Overview

`pfgamesaveutil` is an command-line tool for inspecting, downloading, comparing, and managing PlayFab Game Save (PGS) data. It is used by developers and support engineers to diagnose sync issues, retrieve player save data from the cloud, compare cloud state against local device state, and reset save data during development and testing.

The tool is a .NET 8.0 console application (Windows-only) built with `System.CommandLine` and the PlayFab C# SDK. It communicates with PlayFab Game Save REST APIs (`/GameSave/ListManifests`, `/GameSave/GetManifestDownloadDetails`, `/GameSave/DeleteManifest`, `/GameSave/GetQuotaForPlayer`) and also inspects local on-disk PGS folders and Windows Registry keys related to GamingServices.

## Problem Statement

When diagnosing PlayFab Game Save issues — whether during SDK development, title integration testing, or live support — engineers need a way to:

1. **See what's in the cloud** for a specific player (manifests, files, sizes, quota usage).
2. **Download cloud saves** to inspect file contents locally.
3. **Compare cloud vs. local** to verify whether a device is in sync.
4. **Collect local state** (PGS folder + device context) as a shareable diagnostic artifact.
5. **View and modify local device settings** (registry keys that control GamingServices behavior).
6. **Reset save data** (cloud, local, or both) to return to a clean state for testing.

Prior to this tool, each of these tasks required manual REST API calls, hand-written scripts, or navigating the file system and registry by hand.

## Architecture

```
pfgamesaveutil.exe
├── Program.cs              Command definitions & argument parsing (System.CommandLine)
├── CloudOperations.cs      Commands that talk to PlayFab: download, info, compare, reset
├── LocalOperations.cs      Commands that only touch local disk: status, collect, reset --local
├── GameSaveClient.cs       HTTP client wrapper for PlayFab Game Save REST APIs
├── SaveExtractor.cs        Decompresses chunks (zip/gzip) into save files using extended manifest
├── InfoFormatters.cs       Output formatters (summary, verbose, JSON) for the info command
├── PlayerResolver.cs       Resolves master_player_account ID → title_player_account entity ID
├── RegistryHelpers.cs      Read/write Windows Registry (GamingServices settings)
├── Utilities.cs            Shared helpers (size formatting, folder counting)
└── Models/
    ├── ExtendedManifest.cs     Extended manifest JSON model (folders, files, extract entries)
    └── GameSaveResponses.cs    PlayFab API response models (ListManifests, DownloadDetails, Quota)
```

### Dependencies

| Dependency | Purpose |
|---|---|
| `PlayFabAllSDK` (NuGet) | Authentication (`GetEntityToken`) and player resolution (`GetTitlePlayersFromMasterPlayerAccountIds`) |
| `System.CommandLine` (NuGet) | CLI argument parsing, help text generation, tab completion |
| .NET 8.0 (Windows TFM) | Runtime; Windows-specific due to `Microsoft.Win32.Registry` usage |

### PlayFab API Endpoints Used

| Endpoint | Used By | Purpose |
|---|---|---|
| `POST /Authentication/GetEntityToken` | All cloud commands | Authenticate with title secret key to obtain entity token |
| `POST /Profiles/GetTitlePlayersFromMasterPlayerAccountIds` | `--master-player-id` resolution | Resolve PlayFabId to title_player_account entity ID |
| `POST /GameSave/ListManifests` | `info`, `download`, `compare`, `reset` | List all manifest versions for a player |
| `POST /GameSave/GetManifestDownloadDetails` | `download`, `compare`, `info` | Get download URLs for files within a manifest version |
| `POST /GameSave/DeleteManifest` | `reset --cloud` | Delete a specific manifest version |
| `POST /GameSave/GetQuotaForPlayer` | `info` | Get storage quota usage for a player |
| Blob storage URLs (from download details) | `download`, `compare` | Download manifest files and compressed chunk blobs |

## Commands

### `download` — Download Cloud Saves

Downloads a player's cloud saves from PlayFab to a local directory.

**Flow:**
1. Authenticate with PlayFab using title ID + secret key.
2. List all manifests for the player.
3. Select the target manifest version (latest finalized by default, or a specific version via `--local-path`).
4. Fetch download details (file names + blob URLs) for the target version.
5. Download the extended manifest JSON from blob storage.
6. Download all chunk blobs (compressed save data).
7. Extract chunks using `SaveExtractor`, which reads the extended manifest to reconstruct folder structure and decompress files (supports `zip`, `gzip`, and uncompressed).
8. Save metadata to a `cloudsync/` subfolder: extended manifest, service manifest, and `info.json` with full player state summary.

**Output structure:**
```
<output>/
├── <game-save-files>         # Extracted save files in original folder structure
└── cloudsync/
    ├── extended-<ver>-manifest.json
    ├── service-manifest-<ver>.json
    └── info.json              # Player summary (manifests, quota, health issues)
```

**Key behaviors:**
- Only finalized manifests are downloadable by default.
- Warns (but attempts) if a non-finalized version is explicitly requested.
- Superseded files (`SkipFile: true` in extended manifest) are skipped during extraction.
- File timestamps are restored from the `LastModified` field in the extended manifest.
- Path traversal attacks are guarded against — extraction paths are validated to stay within the output directory.

### `info` — View Save Information

Displays a summary of a player's cloud save state: manifest count, file count, chunk count, compressed/uncompressed sizes, quota usage, and health issues (e.g., quarantined manifests).

**Output modes:**
- **Default (summary):** One-line stats (manifests, files, storage, quota).
- **`--verbose`:** Adds per-manifest breakdown with version, status, file count, sizes, and timestamps.
- **`--json`:** Machine-readable JSON output for scripting and automation.

**Flow:**
1. Authenticate and list manifests.
2. For each finalized manifest, download extended manifest to compute file/chunk/size stats.
3. Fetch quota information.
4. Render output in selected format.

### `compare` — Compare Cloud vs Local

Compares the cloud manifest with local on-disk files to determine sync status.

**Flow:**
1. Authenticate and resolve the local PGS folder (auto-detect by title ID or manual path).
2. Download the cloud extended manifest for the latest finalized version.
3. Look for a matching local extended manifest file.
4. Enumerate files on local disk (in the `current/` subfolder if present, otherwise root).
5. Build three file lists: cloud manifest files, local manifest files, and local disk files.
6. Compare by file name and size:
   - **Match:** Same file name and size in cloud and on disk.
   - **Cloud only:** File in cloud manifest but not on disk.
   - **Local only:** File on disk but not in cloud manifest.
   - **Size mismatch:** Same file name but different sizes.

**Output:** Color-coded comparison table with sync verdict (✓ in sync, or itemized differences).

### `reset` — Delete Save Data

Deletes save data with configurable scope:
- `--cloud` (default): Deletes all cloud manifests for the player (skips quarantined/pending-deletion states).
- `--local`: Deletes the local PGS folder for the title.
- `--cloud --local` (or implicit when both apply): Deletes both.

**Safety:**
- Prompts for confirmation by default (`Are you sure? [y/N]`).
- `--force` bypasses the confirmation prompt for scripting.
- Local-only reset does not require cloud credentials.

### `collect` — Collect Local Data

Creates a timestamped ZIP archive of the local PGS folder for sharing or debugging. Does not require cloud credentials.

**Contents of the ZIP:**
- All files from the PGS folder.
- `_device-status.txt` — auto-generated device context including machine name, user, manifest versions, folder sizes, and GamingServices registry status.

**Output file naming:** `PGS-<folderName>-<yyyyMMdd-HHmmss>.zip` in the current working directory.

### `status` — Local Device Status

Displays local device information relevant to PGS:
- **PGS Folder Status:** Base paths found across all fixed drives, matching subfolders for the title, file counts, sizes, manifest versions, and `current/` subfolder contents.
- **Registry Status:** Whether the `XGameSaveStorage\PlayFab` key exists.
- **GamingServices Settings:** Three registry values that control GamingServices behavior:
  - `ForceUseInprocGameSaves` — Forces in-process game save provider (vs. out-of-process GRTS).
  - `ForceUseLocalServices` — Forces local services mode.
  - `TraceToDebugger` — Enables debug tracing for GamingServices auth.

**Registry modification:** When `--inproc`, `--local-services`, or `--trace` flags are provided with `true` or `false`, the command writes/deletes the corresponding DWORD values in HKLM. Requires Administrator privileges.

## Global Options

| Option | Alias | Required For | Description |
|---|---|---|---|
| `--title-id` | `-t` | All commands | PlayFab title ID (e.g., `ABCDE`) |
| `--secret-key` | `-k` | Cloud commands | Title developer secret key from PlayFab Game Manager |
| `--title-player-id` | `-p` | Cloud commands | Player's `title_player_account` entity ID (16 hex chars) |
| `--master-player-id` | | Cloud commands (alternative) | Player's `master_player_account` ID — auto-resolved to entity ID |
| `--local-path` | `-l` | Local commands | Path to local PGS folder, or `auto` (default) to detect by title ID |

## Local PGS Folder Auto-Detection

When `--local-path` is `auto` (the default), the tool:
1. Scans all fixed drives for `<drive>:\XboxGames\GameSave\pgs\`.
2. Within each PGS base path, searches for subfolders matching `*_<titleId>`.
3. If exactly one match is found, uses it automatically.
4. If multiple matches are found, lists them and asks the user to specify with `--local-path`.
5. If no matches are found, reports an error.

## Build & Publish

```bash
cd Tools/pfgamesaveutil
dotnet build                    # Debug build
dotnet publish -c Release       # Single-file publish to bin\publish\
```

The `publish.cmd` script runs: `dotnet publish pfgamesaveutil.csproj -c Release -o bin\publish`

**Publish configuration** (from `.csproj`):
- Single-file executable (`PublishSingleFile`)
- Framework-dependent (requires .NET 8.0 runtime on target machine)
- Windows x64 only (`win-x64` RID)

## Security Considerations

- **Secret key handling:** The title secret key provides full title-level access to PlayFab APIs. It should never be committed to source control or shared in logs. The tool accepts it as a command-line argument; for scripting, use environment variables (`$env:PLAYFAB_SECRET_KEY`).
- **Entity token scope:** The tool authenticates as a title-level entity, not as a player. This grants access to any player's data within the title.
- **Registry writes:** The `status` command can modify HKLM registry keys. This requires Administrator privileges and is guarded by the `--inproc`, `--local-services`, and `--trace` flags (not done silently).
- **Path traversal:** The `SaveExtractor` validates that all extraction paths remain within the output directory to prevent zip-slip attacks.
- **Destructive operations:** The `reset` command permanently deletes data. It uses confirmation prompts by default and clearly labels the scope of deletion.

## Limitations

- **Download only** — no upload support (saves cannot be pushed to the cloud).
- **Only finalized manifests** can be fully downloaded (initialized/uploading manifests lack complete data).
- **Windows only** — depends on `Microsoft.Win32.Registry` for registry operations and targets `net8.0-windows`.
- **No streaming** — all chunk data is downloaded into memory before extraction.
- **No authentication caching** — each command run re-authenticates with PlayFab.

## Future Considerations

- Upload support for test data injection.
- Cross-platform support (Linux/macOS) by making registry operations optional.
- Authentication token caching to avoid repeated auth round-trips.
- Parallel chunk downloads for large save sets.
- Integration with PFGameSave test infrastructure for automated validation.

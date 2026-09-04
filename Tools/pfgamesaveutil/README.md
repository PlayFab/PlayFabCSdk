# pfgamesaveutil

A command-line tool for working with PlayFab Game Save data. Download cloud saves, upload a folder as a new save version, inspect save state, compare cloud vs local, and collect local data for debugging.

## Getting a Secret Key

Cloud operations require your title's **secret key** (developer secret key). To get it:

1. Open the PlayFab Game Manager: <https://developer.playfab.com/>
2. Select your title.
3. Go to **Settings** (the gear icon) → **Secret Keys** tab.
4. **Reveal** an existing key, or click to **create a new** secret key.

> **Keep your secret key secure.** It grants full title-level access to your game's data.
> Never commit it to source control or share it. Prefer passing it via an environment
> variable (see [Security Notes](#security-notes)).

## Quick Start

```bash
# Tip: set the secret key once via an environment variable so you don't repeat --secret-key
# (and so it stays out of your shell history). PowerShell:  $env:PLAYFAB_SECRET_KEY = "YOUR_KEY"
# All cloud commands below then work without --secret-key. See Security Notes for details.

# Download a single manifest version and extract to a folder (latest finalized by default)
pfgamesaveutil download --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\saves

# Download ALL manifest versions into per-version folders
pfgamesaveutil downloadall --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\saves

# Upload a folder as a new finalized save version
pfgamesaveutil upload --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\my-save-folder

# View save info
pfgamesaveutil info --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

# Compare cloud vs local (auto-detects local folder)
pfgamesaveutil compare --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

# Collect local PGS folder (no cloud access needed)
pfgamesaveutil collect --title-id ABCDE

# Show local device status
pfgamesaveutil status --title-id ABCDE
```

## Common Scenarios

Real-world workflows that combine multiple commands. (Replace `YOUR_KEY` and the
player id with your own; consider passing the secret key via an environment variable —
see [Security Notes](#security-notes).)

### Investigate a player's save state (debugging)

Start with `info` to see all manifest versions and health, then pull everything down
with `downloadall` to inspect the actual files and per-version metadata side by side.

```bash
# 1. Summarize what the player has in the cloud (versions, status, sizes, health)
pfgamesaveutil info --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

# 2. Download every version into per-version folders for inspection
pfgamesaveutil downloadall --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\debug\player

# 3. If a request misbehaves, re-run with --verbose to see the raw HTTP traffic
pfgamesaveutil info --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --verbose
```

### Roll a player back to a specific version

Use `info` to find the good version, `download --version` to extract exactly that
version, then `upload` it back as a new finalized version (the current "latest").

```bash
# 1. List versions and pick the known-good one (e.g. 1044)
pfgamesaveutil info --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

# 2. Extract that specific version to a folder
pfgamesaveutil download --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --version 1044 --path C:\rollback

# 3. Re-upload it as a brand-new latest version
pfgamesaveutil upload --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\rollback --description "Rollback to v1044"
```

### Reproduce / seed a save for testing

Download a player's save, tweak the files locally, then upload it back (to the same or
a different player) to set up a specific test state.

```bash
# 1. Grab the latest save
pfgamesaveutil download --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\seed

# 2. ...edit files under C:\seed as needed...

# 3. Upload the modified folder as a new version
pfgamesaveutil upload --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\seed --force
```

### Diagnose a sync problem on this device

Compare what the cloud has against the local PGS folder, and capture local state for a
bug report — neither requires touching the cloud beyond the compare.

```bash
# 1. Compare cloud manifest vs the local save folder (auto-detected by title)
pfgamesaveutil compare --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

# 2. Inspect local device + registry status
pfgamesaveutil status --title-id ABCDE

# 3. Zip up the local PGS folder (+ device status) to attach to a bug
pfgamesaveutil collect --title-id ABCDE
```

### Clean slate for a player

Wipe cloud and/or local data to start a test from scratch.

```bash
# Cloud + local, no prompts
pfgamesaveutil reset --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --cloud --local --force
```

## Commands

### `download` - Download a Single Manifest Version

Downloads one manifest version and extracts the save files directly into the output folder.
If `--version` is omitted, the latest finalized manifest is used. Only the files present in
the cloud save are written (no extra metadata folders), so `--path` stays clean and is safe
to feed straight back into `upload`.

Pass `--metadata <folder>` to also capture diagnostics separately (so they don't pollute the
uploadable save): the raw `list-manifests.json` service response, the target version's
`manifest.json` entry, the full `download-details.json` (including the per-file SAS download URLs),
the raw `extended-manifest.json`, and the raw chunk `zips/`.

```bash
pfgamesaveutil download --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\saves

# Clean save in C:\saves, all diagnostic material in C:\diag
pfgamesaveutil download --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\saves --metadata C:\diag
```

Options:
- `--path <path>` - (REQUIRED) Directory to extract the save into (clean, uploadable)
- `--version <version>` - Download a specific manifest version (default: latest finalized)
- `--metadata <path>` - Optional folder for diagnostics (raw service responses, extended manifest, chunk zips), keeping `--path` clean

### `downloadall` - Download All Manifest Versions

Downloads every manifest version for the player into a known, fixed layout — no extra flags
needed. Clean extracted saves and diagnostics are kept separate:

```
<path>/
  diag/
    list-manifests.json              # raw ListManifests service response
    summary.json                     # summary list + quota
  v{version}/
    extracted/                       # clean save files (uploadable)
    diag/
      manifest.json                  # this version's manifest entry
      download-details.json          # full GetManifestDownloadDetails response (includes SAS download URLs)
      extended-manifest.json         # raw extended manifest
      zips/                          # raw downloaded chunks
```

Non-downloadable versions get a `v{version}/diag/status.json` marker instead of `extracted/`.

```bash
pfgamesaveutil downloadall --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\saves
```

Options:
- `--path <path>` - (REQUIRED) Root directory to write the per-version layout into

### `upload` - Upload a Folder as a New Save Version

Compresses the contents of a local folder into one or more chunks, builds the extended manifest, and
runs the full upload sequence (`InitializeManifest` → `InitiateUpload` → PUT blobs →
`FinalizeManifest`) to create a new **finalized** save version for the player. The folder tree is
preserved (nested subfolders included). A `cloudsync` subfolder, if present, is skipped (it is local
device state, not game data).

Files are split across multiple zip chunks so no single zip exceeds the size limit (default 64MB) —
the console cannot extract zips larger than 64MB. Each chunk is a separate entry in the extended
manifest with its own extract list.

```bash
pfgamesaveutil upload --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\my-save-folder --description "Chapter 3"
```

Options:
- `--path <path>` - (REQUIRED) Local folder whose contents are uploaded
- `--description <text>` - Optional short save description attached to the manifest
- `--force` - Skip the confirmation prompt
- `--max-zip-mb <n>` - Maximum size of each zip chunk in MB (default 64)

> Note: A single file that compresses to more than the limit cannot be split (the manifest format
> maps each file to exactly one chunk). Such a file is placed in its own chunk and a warning is
> emitted — the console may be unable to extract it.

The new version number is derived from the player's existing manifests (the service's
`NextAvailableVersion`, falling back to highest version + 1), with the latest finalized version as
its base.

### `info` - View Save Information

Display information about a player's game save state.

```bash
pfgamesaveutil info --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF
```

Output:
```
Player: 0123456789ABCDEF
Manifests: 4 (2 Initialized, 2 Finalized)
Total Files: 3
Total Chunks: 3
Storage: 20.3 KB compressed / 28.0 KB uncompressed (1.4x ratio)
```

Options:
- `--json` - Output as JSON for scripting
- `--pending-delete` - Include manifests in `PendingDeletion` state. These are hidden by
  default (and by all other commands) because they cannot be downloaded — to recover one,
  restore it via PlayFab Game Manager first.

### `compare` - Compare Cloud vs Local

Compare cloud manifest with local files to check sync status.

```bash
# Auto-detect local PGS folder by title ID (default)
pfgamesaveutil compare --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF

# Compare with a specific folder
pfgamesaveutil compare --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\saves
```

Output:
```
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

### `reset` - Delete Save Data

Delete save data. Use scope options to target cloud, local, or both. With no scope option,
cloud data is deleted by default.

```bash
# Reset cloud only (default if no scope specified)
pfgamesaveutil reset --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF
pfgamesaveutil reset --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --cloud

# Reset local only (no cloud access needed)
pfgamesaveutil reset --title-id ABCDE --local

# Reset both cloud and local
pfgamesaveutil reset --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --cloud --local

# Skip confirmation (for scripting)
pfgamesaveutil reset --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --cloud --local --force
```

Scope options:
- `--cloud` - Delete all cloud manifests for the player (default)
- `--local` - Delete local PGS folder for the title **(PC/Windows only — see note below)**

> **Warning:** This permanently deletes save data. Use `--force` to skip confirmation prompts.

> **Platform note:** `--local` (and all other local-device features — `collect`, `status`,
> the local side of `compare`, and `reset --local`) only work on **PC/Windows**, where game
> saves are cached under `C:\XboxGames\GameSave\pgs\`. They do **not** work for **Xbox console**
> or **Steam Deck** scenarios — those platforms don't expose that local PGS folder layout to this
> tool. The **cloud** features (`download`, `downloadall`, `upload`, `info`, `reset --cloud`)
> work regardless of the player's platform, since they talk to the PlayFab service directly.

### `collect` - Collect Local Data

Zip local PGS folder for sharing or debugging. Does not require a secret key, player ID, or cloud access.

```bash
# Auto-detect by title ID (default)
pfgamesaveutil collect --title-id ABCDE

# Specify folder path
pfgamesaveutil collect --title-id ABCDE --path "C:\XboxGames\GameSave\pgs\u_123_ABCDE"
```

Creates a timestamped zip file in the current directory (e.g., `PGS-u_123_ABCDE-20260123-120000.zip`). The archive includes a `_device-status.txt` file with machine info, manifest versions, and registry status for debugging context.

### `status` - Local Device Status

Show local device status including PGS folder info and relevant registry keys. Does not require cloud access. Auto-detects PGS folders on all drives.

```bash
# Auto-detect by title ID (searches all drives)
pfgamesaveutil status --title-id ABCDE

# Specify folder path
pfgamesaveutil status --title-id ABCDE --path "D:\XboxGames\GameSave\pgs\u_123_ABCDE"
```

The `status` command can also modify the GamingServices registry settings:
- `--force-inproc <true|false>` - Set/clear `ForceUseInprocGameSaves`
- `--force-local-services <true|false>` - Set/clear `ForceUseLocalServices`
- `--trace-to-debugger <true|false>` - Set/clear `TraceToDebugger`

Output:
```
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
```

## Common Options

### Required for cloud operations

| Option | Description |
|--------|-------------|
| `--title-id` | PlayFab title ID (e.g., `ABCDE`) |
| `--secret-key` | Title secret key from PlayFab Game Manager (see [Getting a Secret Key](#getting-a-secret-key)). May also be supplied via the `PLAYFAB_SECRET_KEY` environment variable — see [Security Notes](#security-notes). |

### Player identification (required for cloud operations)

Specify one of:

| Option | Description |
|--------|-------------|
| `--title-player-id` | Player's `title_player_account` entity ID (16 hex chars) |
| `--master-player-id` | Player's `master_player_account` ID (PlayFabId) - auto-resolved to a title player |

### Other options

| Option | Alias | Description |
|--------|-------|-------------|
| `--path` | | Path to the local PGS/save folder. For `download`/`downloadall` it is the (required) destination; for `upload` it is the (required) source folder; for `compare`/`collect`/`status`/`reset --local` it defaults to `auto` (detect by title ID). |
| `--version` | | Specific manifest version (with `download`) |
| `--metadata` | | Diagnostics folder for `download` (raw service responses, extended manifest, chunk zips); keeps `--path` clean. (`downloadall` always writes diagnostics to `diag/` subfolders.) |
| `--description` | | Short save description (with `upload`) |
| `--max-zip-mb` | | Maximum size of each zip chunk in MB, default 64 (with `upload`) |
| `--cloud` | | Delete cloud data (with `reset`) |
| `--local` | | Delete local data (with `reset`) |
| `--force` | | Skip confirmation prompts |
| `--json` | | JSON output (with `info`) |
| `--pending-delete` | | Include `PendingDeletion` manifests (with `info`; hidden everywhere by default since they can't be downloaded) |
| `--verbose` | `-v` | Log all HTTP requests/responses to stderr for diagnostics (works with every command; secrets, entity tokens, and SAS tokens are redacted) |

## Local PGS Folder

On Windows, game saves are cached locally at:
```
C:\XboxGames\GameSave\pgs\
```

Subfolders are named `u_{id}_{titleId}` where `{id}` identifies the user account.

Requires .NET 8.0 or later (**Windows/PC only**). The local-device features (`collect`, `status`,
`reset --local`, and the local side of `compare`) read this folder and therefore only work on
PC/Windows — they do **not** apply to **Xbox console** or **Steam Deck** scenarios. Cloud features
work on any platform's player data because they talk to the PlayFab service.

## Related GDK Tools

For **Xbox console** Connected Storage scenarios (which this tool does not cover), two utilities
ship with the Microsoft GDK, typically under `C:\Program Files (x86)\Microsoft GDK\bin\`:

- **`xbstorage.exe`** — Manages Connected Storage **on a connected console / devkit**. Subcommands:
  `export` (dump a Connected Storage space to an XML file), `import` (load data from XML into a
  space), `delete` (remove data from a space), `reset` (factory-reset Connected Storage on the
  console), `generate` (create dummy data XML), and `simulate` (simulate out-of-storage conditions).
  Run `xbstorage help commands` or `xbstorage help <command>` for details.

  ```bash
  xbstorage export --output savedata.xml
  ```

- **`XblConnectedStorage.exe`** — Downloads a user's Connected Storage data from the **Xbox Live
  service** to a local XML file, identified by SCID + sandbox + legacy gamertag (no console
  required). Useful for inspecting service-side Connected Storage for a specific player.

  ```bash
  XblConnectedStorage --scid 00000000-0000-0000-0000-0000628cd0f2 --sandbox TEST.0 --gamertag CrazyGiraffe --output ./output/xbstorage.xml
  ```

These operate on **Xbox Connected Storage** (the classic XGameSave/Connected Storage system),
whereas `pfgamesaveutil` operates on **PlayFab Game Save** cloud manifests — different storage
systems for different scenarios.

## Security Notes

- The secret key provides full title-level access. Keep it secure and never commit it to source control.
- Get the secret key from PlayFab Game Manager (**Settings → Secret Keys**); see [Getting a Secret Key](#getting-a-secret-key).
- **Prefer the environment variable over `--secret-key`.** If `--secret-key` is omitted, the tool reads
  the key from the `PLAYFAB_SECRET_KEY` environment variable.
  This keeps the secret out of your shell history and out of the process's visible command-line arguments.
  The `--secret-key` flag, when provided, always takes precedence.
  ```powershell
  # Set once for the session (PowerShell)
  $env:PLAYFAB_SECRET_KEY = "YOUR_KEY"
  pfgamesaveutil download --title-id ABCDE --title-player-id 0123456789ABCDEF --path C:\saves

  # Or pass explicitly (less safe — visible in shell history / process list)
  pfgamesaveutil download --title-id ABCDE --secret-key YOUR_KEY --title-player-id 0123456789ABCDEF --path C:\saves
  ```
- The secret key is never written to disk and is automatically redacted from `--verbose` output
  (the `X-SecretKey` header and any `SecretKey`/`DeveloperSecretKey` body fields are shown as `<redacted>`).
- Use `--verbose` to inspect HTTP traffic when diagnosing issues; secret keys, entity tokens, and Azure Storage SAS tokens are automatically redacted from the logged output.

# Testing an older SDK build (e.g. the 2510 QFE) with the current harness

How to build a servicing branch of the PlayFab SDK and run **this repo's** test harness against it,
so a QFE can be validated with the newest scenarios, controller, and tooling.

This was written from a 2510 GDK QFE run, but nothing here is 2510-specific — it applies to any
older `PlayFab.C` clone.

---

## Why this is not just "copy the DLLs"

Three things make the naive approach fail, and two of them fail *silently*:

| Trap | What you see | Why |
|---|---|---|
| **Wrong build flavor** | Everything builds, runs, and goes green — against the wrong code | An older clone's `Out\x64\Debug` is its **Win32** build. Those DLLs are already named `PlayFabCore.dll` and export the same entry points, so they load cleanly. This repo's `Out\x64\Debug` test app is actually the **Gaming.Desktop.x64 (PC GDK)** build copied out under bare names. |
| **Wrong output names** | `0xC0000135` (`DLL_NOT_FOUND`) at launch | Older SDK projects predate main's `<TargetName>` overrides, so they emit `PlayFabCore.GDK.dll`. Renaming the files afterwards does **not** help: each dependent's *import table* also records the `.GDK` name. |
| **Split dependency instances** | Mock-based scenarios "pass through" — a test that injects an HTTP 404 sees the sync succeed | The older SDK links `libHttpClient.GDK.lib` and so imports `libHttpClient.GDK.dll`, while a main-built test app imports `libHttpClient.dll`. **Both files exist in the output, so both load** — no crash. But they are two module instances with separate global state, so an HTTP mock the app registers is invisible to SDK calls, which quietly take the real network path. |
| **In-proc not forced** | All 44 tests fail at `PFGameSaveFilesResetCloudAsync` with `0x80004001` (`E_NOTIMPL`) | The harness forces in-proc via `PFGameSaveFilesSetForceInprocForDebug`, which older SDKs don't export. The app logs *"not available in this GDK build — skipped"*, silently falls back to GRTS, and GRTS's `ResetCloudAsync` is a stub. |

`Utilities\Scripts\pfgs-tests-update-bins.ps1` handles the first three. The third is verified
automatically after every `/src` copy — see [Step 4](#step-4--verify-before-you-trust-the-run). The
fourth is a machine-wide registry key; see [In-proc forcing](#step-3--force-in-proc).

> **Never put both `PlayFabCore.dll` and `PlayFabCore.GDK.dll` in the test app folder.** Two module
> instances mean two copies of `PFCoreGlobalState`, and the GDK game-save context lives there. The
> same reasoning is why `libHttpClient` must resolve to one name across the app and the SDK.

---

## Prerequisites

- Two clones, e.g. this repo at `C:\git\PlayFab.C` and the servicing branch at `C:\git\PlayFab.C.2`.
- Visual Studio 2022 (or later) with the GDK workload.
- `AdminHelperTray` running (elevated) — lets `AdminHelper` start without a UAC prompt.

---

## Step 1 — Build the harness from *this* repo

```powershell
cd C:\git\PlayFab.C
.\Utilities\Scripts\tests-build.ps1            # add -Clean for a clean rebuild
```

This builds `GameTestAppWindows`, `GameTestAppXbox`, and the C# `GameTestController` into
`Out\x64\Debug\`. Build the harness **before** swapping in the older SDK — a harness rebuild
overwrites the swapped DLLs, so you would have to swap again.

## Step 2 — Build the older SDK and swap it in

One command does both. It builds the servicing clone with the bare-name override injected, then
copies the three SDK binaries into the test app folders:

```powershell
.\Utilities\Scripts\pfgs-tests-update-bins.ps1 /src C:\git\PlayFab.C.2 -Build
```

What it does, and why:

- Builds `PlayFabCore.GDK`, `PlayFabServices.GDK`, `PlayFabGameSave.GDK` from the source clone at
  `Gaming.Desktop.x64|Debug`, **one at a time** (clones commonly share a single `libHttpClient`
  checkout, and parallel builds race its `.tlog` files → `MSB3491`).
- Injects `Utilities\Scripts\ExternalSdkTargetNames.props` via
  `/p:ForceImportBeforeCppTargets=<abs path>`, which forces the bare `TargetName`. Because that is
  a *global* property it propagates through `ProjectReference`, so the dependents link against —
  and therefore import — `PlayFabCore.dll`. This is what makes the DLLs drop-in compatible.
- That same props file redirects the `libHttpClient` link to its `.nosuffix` build, so the swapped
  SDK shares the app's single `libHttpClient.dll` instead of loading a second instance under the
  `.GDK` name.
- **Requires no edits to the servicing clone.** `git status` there stays clean, which matters when
  the branch is headed for a PR.
- Saves whatever it overwrites into `_orig-bins\` and records the source branch + SHA in
  `_orig-bins\swapped-from.txt`.
- After copying, re-reads the import tables and **fails** if the SDK and the test app resolve any
  shared dependency under different names.

Useful switches:

| Switch | Effect |
|---|---|
| `-Build` | Rebuild the source clone with the bare-name override first |
| `-Platform <p>` | Source platform to read. Default `Gaming.Desktop.x64`. `x64` warns — that is the Win32 flavor |
| `-Configuration <c>` | Default `Debug` |
| `-All` | Copy every `.dll`/`.pdb` found, not just the three SDK modules |
| `-Restore` | Put the original binaries back, then exit |

If the clone is already built, drop `-Build`. If its output still has `.GDK` names the script
refuses and tells you to add `-Build` rather than copying something that cannot load.

## Step 3 — Force in-proc

Older SDKs pick the provider from a machine-wide registry key instead of the debug API:

```
HKLM\SOFTWARE\Microsoft\GamingServices!ForceUseInprocGameSaves = 1  (REG_DWORD)
```

Writing it needs elevation. Route it through the already-elevated `AdminHelper` so the test loop
stays unattended:

```powershell
.\Out\x64\Debug\AdminHelpCli\AdminHelpCli.exe GameSaveMaintenance options=--inprocgamesaveson
```

That action is a validated wrapper over `Utilities\Scripts\gamesave-maintenance.ps1`; it accepts
only that script's documented flags. Other useful ones: `--fullclean`, `--deletereg`,
`--deletefolder`, `--restart`, `--collectlogs`.

> ⚠️ **This key is machine-wide and persists across reboots.** While it is set, *every* GameSave
> run is forced in-proc, which silently breaks `pc-grts` suites. Clear it when you finish —
> see [Cleanup](#cleanup).

## Step 4 — Verify before you trust the run

Thirty seconds here turns a mystery failure into a named list. The script already runs the
module-binding check for you on `/src`; these confirm the rest by hand:

```powershell
$db = (Get-ChildItem "C:\Program Files\Microsoft Visual Studio" -Recurse -Filter dumpbin.exe |
       Where-Object FullName -match 'Hostx64\\x64' | Select-Object -First 1).FullName
& $db /imports Out\x64\Debug\GameTestAppWindows\PlayFabGameSave.dll | Select-String PlayFabCore
# expect: PlayFabCore.dll   (NOT PlayFabCore.GDK.dll)

# The app and every swapped module must name the SAME libHttpClient, or mocks silently do nothing.
foreach ($m in 'GameTestAppWindows.exe','PlayFabCore.dll','PlayFabServices.dll','PlayFabGameSave.dll') {
  "$m -> " + ((& $db /imports "Out\x64\Debug\GameTestAppWindows\$m" |
               Select-String -SimpleMatch 'libHttpClient') -join ', ')
}

Get-Content Out\x64\Debug\GameTestAppWindows\_orig-bins\swapped-from.txt   # what is installed
Get-ChildItem Out\x64\Debug\GameTestAppWindows -Filter "PlayFab*.GDK.dll"  # expect: nothing
```

## Step 5 — Run the tests

```powershell
py .\Utilities\Scripts\tests-run.py gamesave-inproc --stop-on-fail
```

`tests-run.py` only launches — it does **not** build — so the swapped DLLs survive the run.

The runner prints what it is actually about to exercise, so a wrong configuration is visible
without asking:

```
  Test Runner — 4 tests (all)
  SDK:     C:\git\PlayFab.C.2
           user/jasonsa/gs-2510-morefixes @ a25b6c91  [Gaming.Desktop.x64|Debug]
  LHC:     C:\git\libHttpClient.2510 @ fcbe2e6
  Swapped: 2026-08-19T14:03:48
  In-proc: forced via regkey
  Engines: pc-inproc-gamesaves
```

Use `--info` to print that block and exit without running anything (exit code 1 if it finds a
problem, so it can gate a script):

```powershell
py .\Utilities\Scripts\tests-run.py gamesave-inproc --info
```

The same block is written to `pass<N>\run-manifest.json` next to the results, including per-DLL
size/mtime/short-hash — so a past run can be attributed later instead of guessed at.

Two configurations **abort the run** rather than produce misleading results:

- **Split module instances** — the SDK and the app resolve a shared dependency under different
  names, so mocks silently do nothing.
- **`pc-inproc-gamesaves` without the in-proc regkey** — the SDK falls back to GRTS and every test
  fails with `E_NOTIMPL`.

`--skip-provenance-check` overrides both. `SDK: this repo (no swap recorded)` means no swap is
installed — you are testing this repo's own build.

Logs land in `Out\gamesave-inproc-tests\pass<N>\<testId>\` (`controller-stdout.txt`,
`device-*-fetched-log.txt`, `test-results.json`). In `test-results.json`, scenario `status` is
`0` = pass, `1` = fail, `2` = skipped.

**Always get a baseline.** Restore this repo's own binaries and run the same suite before
concluding a failure is a regression in the older SDK:

```powershell
.\Utilities\Scripts\pfgs-tests-update-bins.ps1 -Restore
py .\Utilities\Scripts\tests-run.py gamesave-inproc --stop-on-fail
```

---

## Pinning libHttpClient to the shipping version

Everything above swaps the PlayFab SDK but still runs it against **whatever `C:\git\libHttpClient`
is checked out to** — usually main. That is fine for most work, but a QFE ships with a specific LHC:
the 2510 GDK ships `fcbe2e63783ecf022fc345dd11450d8bb38899b3` (PR #929). Testing 2510 against a much
newer LHC can hide or invent behavior — the gap to current main includes
`improve multi mock behavior (#979)`, which is exactly the machinery mock-based scenarios rely on.

**Do not switch the shared checkout.** `C:\git\libHttpClient` is used by this repo's own build too,
and main's `Build\libHttpClient.import.props` depends on `GDK build fix (#954)` for its `.nosuffix`
output — which is *newer* than the 2510 commit, so main would stop building. Use a worktree:

```powershell
cd C:\git\libHttpClient
git worktree add C:\git\libHttpClient.2510 fcbe2e63783ecf022fc345dd11450d8bb38899b3
cd C:\git\libHttpClient.2510
git submodule update --init External/asio External/websocketpp External/zlib
msbuild Build\libHttpClient.GDK\libHttpClient.GDK.vcxproj /p:Configuration=Debug /p:Platform=Gaming.Desktop.x64
```

libHttpClient is a leaf module, so unlike the PlayFab DLLs it needs **no** rename: consumers bind it
by filename, so the older build's content is simply installed under the bare name. Let the swap
script do it, so the commit is recorded and `-Restore` can undo it:

```powershell
.\Utilities\Scripts\pfgs-tests-update-bins.ps1 /src C:\git\PlayFab.C.2 -Build `
    -LhcSource C:\git\libHttpClient.2510
```

`-LhcSource` takes either a folder containing `libHttpClient[.GDK].dll` or a libHttpClient
repo/worktree root. `tests-run.py` then reports the pinned commit in its header and manifest; a
hand-copied DLL is invisible to it and shows up only as an unattributed size/hash.

**Check the export surface first.** An older LHC that is missing even one imported symbol makes the
app fail to start with `0xC0000139` (`ENTRY_POINT_NOT_FOUND`):

```powershell
$exports = & $db /exports "$src\libHttpClient.GDK.dll" |
           ForEach-Object { if ($_ -match '^\s+\d+\s+[0-9A-F]+\s+[0-9A-F]{8}\s+(\S+)') { $matches[1] } }
# then compare against the libHttpClient imports of the exe and the three SDK DLLs
```

If a symbol is genuinely missing, prefer resolving it dynamically in the harness over pinning the
whole suite to a newer LHC. `HCGlobalHandlers.cpp` does this for
`HCSettingsSet/GetGlobalRequestLimit`, which exist only in very recent LHC and are used by two
`lhc`-suite commands: a static import there would have blocked *every* unrelated GameSave scenario
from running against a shipping LHC. Same pattern as
`PFGameSaveFilesSetForceInprocForDebug` in `PFGameSaveFilesHandlers.cpp`.



```powershell
.\Utilities\Scripts\pfgs-tests-update-bins.ps1 -Restore
.\Out\x64\Debug\AdminHelpCli\AdminHelpCli.exe GameSaveMaintenance options=--inprocgamesavesoff
```

Then confirm the servicing clone is still clean (`git -C C:\git\PlayFab.C.2 status`). With the
props-override flow it should be — if it is not, someone hand-edited `Build\*` and those edits
must not be committed to a servicing branch.

---

## Troubleshooting

| Symptom | Cause / fix |
|---|---|
| App exits immediately with `0xC0000135` | A dependent still imports `PlayFabCore.GDK.dll`. Renaming files cannot fix this — rebuild with `-Build`. |
| Every test fails at `PFGameSaveFilesResetCloudAsync` → `0x80004001` | In-proc was not forced; the SDK fell back to GRTS, whose `ResetCloudAsync` is a stub. Set the regkey (Step 3). Confirm in the device log: *"PFGameSaveFilesSetForceInprocForDebug not available in this GDK build — skipped"*. |
| Suite passes suspiciously cleanly | You may be running the Win32 flavor, or no swap at all. Run `tests-run.py <suite> --info` — it prints the exact SDK branch/commit and libHttpClient it would use. |
| A scenario that injects an HTTP error (mock 404/503) reports the call **succeeded** | The SDK and the test app loaded two `libHttpClient` instances, so the app's mock never applied and the real request went out. Run the swap script again — it now fails on this — and confirm every module imports the same `libHttpClient.dll`. |
| `MSB3491` / `.tlog` access denied | Two clones building in parallel against the same `libHttpClient` checkout. Build serially (the script already does). |
| *"The platform guessed by MSBuild does not agree with the platform selected by config.h"* | You passed `/p:Platform=x64` to a GDK `.vcxproj`. Use `Gaming.Desktop.x64`. |
| Solution built `Debug\|x64` but `Out\x64` has no new SDK DLLs | The solution's `Debug\|x64` maps to project `Debug\|Gaming.Desktop.x64`; output lands in `Out\Gaming.Desktop.x64`. |
| A build script piped to a file yields an empty log that greps as "0 errors" | `tests-build.ps1` writes via `Write-Host`, which bypasses the pipeline. Check the log size before trusting an error count, or use `Start-Transcript`. |
| `msbuild` not found | Use `vswhere`, or prepend `C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin` to `PATH`. |
| `PlayFabGameSaveUnitTests` won't build on 2510 | Pre-existing and unrelated: `Build\PlayFabGameSave.Win32\PlayFabGameSave.Win32.vcxitems` points at `Source\PlayFabGameSave\Source\Platform\Win32\Metadata_Win32.cpp`, but the file actually lives under `Platform\Windows\`. Not needed for this workflow. |
| App exits with `0xC0000139` (`ENTRY_POINT_NOT_FOUND`) after pinning libHttpClient | The older LHC doesn't export something the app or SDK imports. Diff exports vs imports (see [Pinning libHttpClient](#pinning-libhttpclient-to-the-shipping-version)) and resolve the missing API dynamically in the harness. |
| A source edit to the harness seems to have no effect on the exe's imports | Incremental linking keeps stale import descriptors. Delete the `.exe` and any `.ilk`, then rebuild — `dumpbin /imports` will still show the old entry otherwise, even though the `.obj` is clean. |
| Building the test app fails in `intrin0.inl.h` with `C2375: '__builtin_assume_aligned': redefinition` | You picked up the VS18 toolset (e.g. via `vswhere -latest`). Put VS2022's MSBuild first: `$env:PATH = "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin;$env:PATH"`. |
| Rebuilding `GameTestAppWindows.vcxproj` silently reverts a swapped SDK | That project builds and copies this repo's own PlayFab DLLs into the output folder. Re-run the swap script after any harness rebuild. |

---

## Related

- `Utilities\Scripts\pfgs-tests-update-bins.ps1` — the swap/build/restore script
- `Utilities\Scripts\ExternalSdkTargetNames.props` — the bare-name override
- `Utilities\Scripts\gamesave-maintenance.ps1` — registry/service/folder maintenance
- `Utilities\Scripts\tests-run.py` — test runner; `--info` reports build provenance
- `specs\BUILD_AND_TEST.md` — general build and test loop
- `specs\playfab-gamesave\pfgamesave-debugging-guide.md` — ETL/Kusto, GRTS state, failure triage
- `Test\AGENTS.md` — harness internals, tags, scenario reference

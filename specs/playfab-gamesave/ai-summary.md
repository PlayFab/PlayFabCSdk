Here's a distilled knowledge pack on PFGameSave based on the public docs, dev spec, headers, sample, and the Windows (GRTS/out‑of‑proc) provider source. Let me know what angle you want next (design help, code changes, troubleshooting, feature proposal, etc.).

## High-level Purpose
PlayFab Game Saves provides cross‑device, cross‑platform synchronized save data with:
- Single point of progression (active device model)
- Conflict management at a chosen granularity (root-level subfolder = atomic unit)
- Offline-first local continuity
- Incremental, compressed, manifest-based upload/download
- Out-of-process background upload on Xbox/Windows (via GRTS / xgameruntime) vs in‑process on other platforms (e.g., Steam Deck)

## Core Runtime Flow (Typical Session)
1. Initialize PlayFab Core (PFInitialize) if not already.
2. Create `PFServiceConfigHandle` (endpoint + Title ID).
3. Create / obtain platform identity -> `PFLocalUserHandle` (Xbox, Steam, custom).
4. Call `PFGameSaveFilesInitialize(&PFGameSaveInitArgs)` (set `saveFolder` on platforms that require explicit location—e.g., Steam Deck; ignored on Xbox/Windows).
5. (Optional / required on non-Xbox & for custom UI) Set UI callbacks via `PFGameSaveFilesSetUiCallbacks`.
6. Call `PFGameSaveFilesAddUserWithUiAsync` (the “download dance”):
   - Acquire/confirm active device
   - Resolve conflicts / contention
   - Download changed data
   - Provide root folder path via `PFGameSaveFilesGetFolder`
7. Game reads/writes normal files inside that root (organize by top-level subfolders to define atomic units).
8. Periodic or event-driven `PFGameSaveFilesUploadWithUiAsync` (the “upload dance”) with option:
   - `KeepDeviceActive` (continue playing & future uploads)
   - `ReleaseDeviceAsActive` (end-of-session / main menu transition)
9. (Optional) Set short save description (`PFGameSaveFilesSetSaveDescriptionAsync`) before upload to improve remote UI context.
10. On quit (Xbox/Windows): background system may finish upload automatically; on in-process platforms you must finish before exit.
11. Uninitialize if you need to tear down: `PFGameSaveFilesUninitializeAsync`.

## Key Concepts
- Active Device: Only one device “owns” ongoing progression; others must explicitly take over (device contention UI).
- Atomic Unit = each root-level subfolder under the save root. Conflicts operate at this level: if any file in that subfolder needs both upload & download, the entire unit is in conflict.
- Manifest + Compression:
  - Files grouped into ZIP bundles (≤ 64 MB) for efficient transfer.
  - Extended manifest tracks each compressed payload and internal extracted files, with “SkipFile” flags to reuse previously uploaded compressed blobs.
  - New saves may re-upload only changed bundles; old bundles reused or eventually compacted (future heuristic).
- Local State JSON (in-proc client) tracks last synced file size + timestamp to detect local modifications (not shared across devices).
- Offline Mode: User can elect “UseOffline” when initial sync fails. Cloud ops then return `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` until re-AddUser attempt succeeds.

## Platform Differences
| Aspect | Xbox / Windows (GRTS) | Steam Deck / Other In-Process |
|--------|-----------------------|-------------------------------|
| Upload execution | Out-of-process (continues after exit) | In-process only (must finish before exit) |
| Stock UI | Provided (optional to override) | Must supply via callbacks |
| `saveFolder` argument | Ignored (system managed) | Required (game chooses root) |
| Active device polling | Handled service-side | Title uses callbacks / reconnection |
| Background reliability | Higher (OS-managed) | Title must sync aggressively |

Steam Deck special:
- May integrate via Xbox ecosystem (XUser + remote connect + SPOP dialogs) or custom identity (OpenID / custom provider). The guide strongly recommends frequent uploads due to lack of post-exit syncing.

## Public API Snapshot (Order & Notes)
Initialization / Teardown:
- `PFGameSaveFilesInitialize(PFGameSaveInitArgs*)`
- `PFGameSaveFilesUninitializeAsync` / `PFGameSaveFilesUninitializeResult`

User Onboarding & Cloud Sync:
- `PFGameSaveFilesAddUserWithUiAsync` / `...Result`
- `PFGameSaveFilesGetFolder` (+ `GetFolderSize`)
- `PFGameSaveFilesIsConnectedToCloud`
- `PFGameSaveFilesGetRemainingQuota`
- `PFGameSaveFilesUploadWithUiAsync` / `...Result`
- `PFGameSaveFilesSetSaveDescriptionAsync` / `...Result`
- `PFGameSaveFilesResetCloudAsync` (dev/test) / `...Result` (Not implemented in GRTS provider: returns `E_NOTIMPL` there)

UI Callback Wiring:
- `PFGameSaveFilesSetUiCallbacks(&PFGameSaveUICallbacks)`
- `PFGameSaveFilesSetActiveDeviceChangedCallback(queue, callback, ctx)`

UI Response APIs (must be called after each respective callback unless user canceled progress):
- Progress: `PFGameSaveFilesSetUiProgressResponse`
- Sync Failed: `PFGameSaveFilesSetUiSyncFailedResponse`
- Active Device Contention: `PFGameSaveFilesSetUiActiveDeviceContentionResponse`
- Conflict: `PFGameSaveFilesSetUiConflictResponse`
- Out of Storage: `PFGameSaveFilesSetUiOutOfStorageResponse`
- Query progress in progress callback: `PFGameSaveFilesUiProgressGetProgress`

Upload Option Enum:
- `KeepDeviceActive`
- `ReleaseDeviceAsActive`

## UI Callback Triggers
| Callback | Trigger Operations | Purpose |
|----------|--------------------|---------|
| Progress | AddUser + Upload | Show bytes progress & allow cancel |
| Sync Failed | AddUser + Upload | Retry, Cancel, (UseOffline only during AddUser initial sync) |
| Active Device Contention | AddUser | Player chooses: Retry / Cancel / SyncLastSavedData |
| Conflict | AddUser | Choose Local vs Remote (atomic units collectively) |
| Out Of Storage | AddUser (download) | Retry after clearing space or Cancel |
| Active Device Changed (separate callback) | Async background event | Title may return to menu or let offline continue |

## Error Codes (Selected)
- `E_PF_GAMESAVE_NOT_INITIALIZED` – Forgot to initialize library.
- `E_PF_GAMESAVE_USER_NOT_ADDED` – Called folder/quota/upload before successful AddUser completion.
- `E_PF_GAMESAVE_USER_ALREADY_ADDED` – Attempted second AddUser without re-init.
- `E_PF_GAMESAVE_DOWNLOAD_IN_PROGRESS` – Tried upload before AddUser finished.
- `E_PF_GAMESAVE_USER_CANCELLED` – User canceled via UI response.
- `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` – Operating offline (expected in offline mode).
- `E_PF_GAMESAVE_NETWORK_FAILURE` – Upload attempt failed while nominally connected.
- `E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE` – Device lost active status (other device took over).
- `E_PF_GAMESAVE_DISK_FULL` – Local storage insufficient during download.

## Conflict Semantics
Conflict = (Local changed since last upload) AND (Cloud changed since last sync) within same root-level subfolder. Resolution applies to all conflicting atomic units in one user choice:
- Take Local → Next upload overwrites cloud with local versions.
- Take Remote → Local tree overwritten by cloud versions (local edits lost for those units).
No partial merges. Deletions also factor into conflict detection (deleted vs modified remote).

## Active Device Contention
Occurs when another device has a pending or active manifest newer than local, or device transitions:
- Player can retry (wait for remote to finish)
- Cancel (abort operation)
- Sync last cloud data (force takeover; remote device future uploads blocked until it re-syncs)
Releasing active device early is strongly encouraged when user returns to menus by using `ReleaseDeviceAsActive` during upload.

## Offline Mode Lifecycle
1. AddUser fails network → SyncFailed callback → user selects:
   - Retry (re-loop)
   - UseOffline → API result `S_OK`, but `IsConnectedToCloud = false`
   - Cancel → operation fails with `E_PF_GAMESAVE_USER_CANCELLED`
2. While offline:
   - `UploadWithUiAsync` immediately completes async with `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`.
   - `GetRemainingQuota` returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`.
   - `AddUserWithUiAsync` may be retried to restore connectivity (no full uninit required).
3. Disconnection can also occur if another device becomes active and local goes passive.

## Manifest & Compression Highlights
- Service Manifest states: Initialized → Uploading → Finalized → (PendingDeletion)
- Extended manifest (client side) enumerates compressed file aggregates and extracted entries (name, folder ID, size, times, skip flags).
- Zipping strategy: ≤ 64MB bundles, reuse previous bundles if no internal changes (files flagged as skipped when superseded).
- Potential later compaction to reduce manifest size vs upload cost tradeoff.

## Sample Code Patterns (From GameSaveIntegration.cpp)
- Async pattern wraps `XAsyncBlock` on user add & upload; ownership via `unique_ptr` to avoid leaks.
- Save file generation just writes binary blob; game logic decoupled from sync.
- UI callback implementations log + show ImGui dialogs and call response APIs.
- Steam Deck path sets a custom folder (`%LOCALAPPDATA%\GameSaveSample`) and must set UI callbacks.
- In sample, active device changed callback only logs (acceptable since system auto-offlines).

## Windows GRTS Provider (PFGameSaveFilesAPIProvider_GRTS.cpp) Insights
- Out-of-process pipeline converts PF callbacks into underlying PFX (platform) callbacks.
- `AddUser` builds a PFX config (apiUrl + titleId + requesting XUser) and defers to `PFXGameSaveFilesGetFolderWithUiAsync`.
- Maintains an internal context with up to 16 users (PF/GRTS user state array).
- `IsConnectedToCloud` always returns true in this provider (out-of-proc model doesn’t surface offline path here).
- Many debug / mock methods are no-ops or `E_NOTIMPL` (e.g., ResetCloudAsync not implemented yet in this provider).

## Limits (Current Public Preview)
- 256 MB per title per player (cloud quota).
- 100 service endpoint requests / rolling 2 minutes (≈ 10 uploads per minute headroom).
(Broader size/file/path constraints still “TBD” in dev spec; future doc likely to finalize.)

## Best Practice Summary
- Organize saves into root-level subfolders aligned with independently mergeable units (slots, profiles, etc.).
- Upload often (level end, checkpoints, menu transitions, before quit) — essential on in-process platforms.
- Use `ReleaseDeviceAsActive` at natural session boundaries (reduces contention friction).
- Provide clear, minimal UI when conflicts or contention occur; let players understand impact (“this device” vs “cloud data”).
- Always respond to UI callbacks (except progress if you don't cancel).
- Use offline mode gracefully: still allow play; surface reconnect options.
- Consider double-buffer local files (robustness against corruption).
- Populate short save description + optional thumbnail (pfthumbnail.png) for richer remote decisions.

## Common Pitfalls to Avoid
- Calling upload before AddUser completes (yields `E_PF_GAMESAVE_DOWNLOAD_IN_PROGRESS`).
- Neglecting to set `saveFolder` on platforms that need it (Steam Deck) → initialization issues.
- Forgetting to call response APIs, causing state machine stalls.
- Relying on background upload on platforms where none exists.
- Treating conflict resolution per file instead of designing root subfolder boundaries properly.

## Useful Decision Matrix
| Goal | API / Action |
|------|--------------|
| Detect cloud connectivity | `PFGameSaveFilesIsConnectedToCloud` |
| Show sync progress | Progress callback + `PFGameSaveFilesUiProgressGetProgress` |
| Cancel a long sync | `PFGameSaveFilesSetUiProgressResponse(Cancel)` |
| Retry network failure | SyncFailed callback → set `Retry` |
| Go offline gracefully | SyncFailed callback → set `UseOffline` |
| Take over active device | ActiveDeviceContention callback → `SyncLastSavedData` |
| Resolve conflict (keep local) | Conflict callback → `TakeLocal` |
| Release device after final upload | Upload with `ReleaseDeviceAsActive` |

## Open / Not Fully Finalized Areas (from dev spec)
- Additional limits (file count, path length, compaction thresholds) still listed as TBD.
- Heuristics for manifest compaction not yet codified.
- Rollback option via `PFGameSaveFilesAddUserOptions::Rollback` still “being designed”.
- Some debug & mock APIs stubbed out in GRTS provider.

## Mental Model Cheat Sheet
Download (AddUser):
 List manifests → Determine active/pending → Acquire active → Fetch extended manifest → Compare vs local state → Detect conflicts → Resolve → Download needed bundles → Expand & update local state.

Upload:
 Compare local vs last finalized extended manifest → Determine changed/deleted files → Bundle/compress ≤64MB sets → Reuse old bundles (skip reused entries) → Upload with progress + periodic manifest update → Finalize manifest → Optionally spawn new Initialized manifest (retain active) or release.


# PFGameSave AddUser (Download) & Upload State Machines

Purpose: Help platform-specific (e.g. PlayStation) integration know exactly where all filesystem access is finished so a mounted save container can be safely unmounted after `PFGameSaveFilesAddUserWithUiAsync` and `PFGameSaveFilesUploadWithUiAsync` complete.

---
## High-Level Entry Points

| Public API | Internal Owning Manager | Core Loop Method | Completion Signal |
|------------|-------------------------|------------------|-------------------|
| `PFGameSaveFilesAddUserWithUiAsync` | `FolderSyncManager` (download path) | `FolderSyncManager::DoWorkFolderDownload` | Returns `S_OK` from async when `SetSyncStateProgress(SyncComplete)` has been set |
| `PFGameSaveFilesUploadWithUiAsync` | `FolderSyncManager` (upload path)   | `FolderSyncManager::DoWorkFolderUpload`   | Returns `S_OK` from async when `SetSyncStateProgress(SyncComplete)` has been set |

In both flows, the async pattern repeatedly calls `DoWork...` until the internal state machines reach a terminal stage. All file IO to the save root takes place inside the subordinate step objects before the final `SyncComplete` is set.

---
## Download (AddUser) State Machine
Top-level sequencing in `FolderSyncManager::DoWorkFolderDownload`:

1. Lock Acquisition (`LockStep`)
   * Method: `LockStep::AcquireActiveDevice` via `m_lockStep.AcquireActiveDevice(...)`
   * Handles: active device contention UI; may set forced disconnect.
   * Exit to next stage once active device (pending manifest) obtained or offline path chosen.
2. Compare (`CompareStep`)
   * Method: `CompareStep::CompareWithCloud(..., downloading=true, ...)`
   * Internal stages (`CompareStep::CompareStage`):
     - `GetManifestDownloadDetails` (service call)
     - `GetExtendedManifest` (optional download of `extended-<ver>-manifest.json`)
     - `ReadLocalManifest` (scan local folder, mark operations, conflict detection)
     - `WaitForConflictUI` or `WaitForFailedUI_*` (UI gating) as needed
     - `CompareDone`
   * Produces: lists of files/folders to delete/create/download; conflict resolution choice.
3. Local Delete/Create Phase (Pre-Download) (`DownloadStep` local ops)
   * Runs once per call after compare done and not offline.
   * File ops: `DownloadStep::DeleteFiles`, `DownloadStep::DeleteFolders`, `DownloadStep::CreateEmptyFolders`.
   * Marks `DownloadStep::m_isLocalOperationsDone = true`.
4. Download Phase (`DownloadStep::Download` state machine)
   * Method: `DownloadStep::Download(...)`
   * Internal stages (`DownloadStage`):
     - `DownloadStart` (progress UI shown, sets sync state to Downloading)
     - `QueryStorage` (calc total size, out-of-storage UI path) 
     - `Download` (loop each compressed file → async cloud fetch)
     - `UncompressFiles` (expand zips/move single files; file timestamps set)
     - `UpdateLocalManifest` (update local state JSON via `LocalStateManifest::WriteLocalManifest`)
     - `DownloadDone`
     - Failure/Retry gates: `WaitForFailedUI_*`, `WaitForOutOfStorageUI`, `Failure`
5. Completion
   * After `DownloadStep::IsDownloadDone()` and any required downloads (or none needed), `FolderSyncManager` calls:
     - `SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, 0, 0)`
   * Active device polling worker started (not touching files) after this point.

### Safe Unmount Point (Download)
All filesystem writes (delete/create/unzip/local manifest write) are complete immediately before or at the line:
```
FolderSyncManager::DoWorkFolderDownload -> SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, ...)
```
No further writes to the save root occur after this call for the AddUser flow.

---
## Upload State Machine
Top-level sequencing in `FolderSyncManager::DoWorkFolderUpload`:

1. Compare (`CompareStep` again with `downloading=false`)
   * Same internal stages as download but upload branch: determines files/folders to upload/delete/create and compression reuse (`CompressedFilesToKeep`).
2. Upload Needed? (branch)
   * If there are any pending file/folder adds/deletes OR files to upload → proceed to UploadStep state machine.
   * Else (no changes): optionally delete pending manifest when option = `ReleaseDeviceAsActive`.
3. Upload Step State Machine (`UploadStep::Upload`)
   * Internal stages (`UploadStage`):
     - `UploadStart` (progress UI shown, sync state Uploading)
     - `CompressFiles` (create zips, extended manifest pre-pass; writes to temp `cloudsync` folder only)
     - `InitiateUpload` (service call → presigned URLs / session)
     - `UploadFile` (loops through each compressed file + final extended manifest; progress updates; deletes local zips after each successful file in `UploadFileFinally`)
     - `FinalizeManifest` (service call; may trigger full set retry or force disconnect) — on success updates local manifest: `LocalStateManifest::WriteLocalManifest`
     - `ListManifestsAfterUpload` (refresh manifest set)
     - `TakeLockAfterUpload` (re-create pending manifest if `KeepDeviceActive`; otherwise skip)
     - `UploadDone`
     - Failure/Retry gates: `WaitForFailedUI_*`, `ForceDisconnectFromCloud`, `Failure`
4. Delete Pending Manifest (Only when `ReleaseDeviceAsActive` and no file changes OR after an upload with that option but no new pending manifest wanted)
   * State machine: `UploadStep::DeletePendingManifest` (`DeleteManifestStage`) called via `UploadStep::DeletePendingManifest` from `FolderSyncManager`.
5. Completion
   * `FolderSyncManager` sets `SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, 0, 0)`.

### File IO Boundaries (Upload)
| Stage | Writes to Save Root | Notes |
|-------|---------------------|-------|
| CompressFiles | READ existing, WRITE temp zips under `<save>/cloudsync` | Pure staging
| UploadFile (per file) | DELETE temp zip after upload | Thumbnail (uncompressed) may persist
| FinalizeManifest (success path) | `LocalStateManifest::WriteLocalManifest`, possibly delete individual uploaded zips | Actual local metadata update
| ListManifestsAfterUpload | Cleanup residual zip files in `cloudsync` | No gameplay files changed
| TakeLockAfterUpload (KeepDeviceActive) | No save root writes except potential new pending manifest on service only | Local FS untouched
| DeletePendingManifest (Release path) | No local file writes | Service only
| SyncComplete | No further file access | Safe unmount

### Safe Unmount Point (Upload)
After `FolderSyncManager::DoWorkFolderUpload` sets:
```
SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, ...)
```
All local compression temp artifacts are cleaned (except thumbnail), manifest written, and (if requested) pending manifest deletion finished.

---
## Hook Suggestions for Platform (PlayStation) Unmount

Preferred minimal-intrusion hook locations (one per flow):

1. Download completion:
   - File: `FolderSyncManager.cpp`
   - Function: `FolderSyncManager::DoWorkFolderDownload`
   - After final call to `SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, ...)` and before returning `S_OK`.
   - Just below: triggering of `ActiveDevicePollWorker::MakeAndStart` (safe — no more save root writes).
2. Upload completion:
   - File: `FolderSyncManager.cpp`
   - Function: `FolderSyncManager::DoWorkFolderUpload`
   - After final `SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, ...)` and before `return S_OK;`.

These are single, centralized points guaranteeing all subordinate step machines (`CompareStep`, `DownloadStep`, `UploadStep`) have reached their terminal states (`CompareDone`, `DownloadDone`, `UploadDone` or no-op branches) and all filesystem mutation is complete.

### Optional Granular Hooks
If you need earlier, more granular mount lifetimes:

| Purpose | Earliest Safe Stage to Unmount | Risk |
|---------|--------------------------------|------|
| Read-only access needed after download only | After `DownloadStage::UpdateLocalManifest` transitions to `DownloadDone` | Must still allow FS for ActiveDevice polling? (Polling does not write) |
| Upload of unchanged data (no compression) | Skip mount early if compare yields no changes AND option != Release? | Edge: later `DeletePendingManifest` still runs (service only) |

For simplicity and safety, prefer the unified completion points.

---
## Quick Visual Summary

Download Flow:
```
LockStep -> CompareStep -> (Deletes/Creates) -> DownloadStep:
  DownloadStart -> QueryStorage -> Download* -> Uncompress -> UpdateLocalManifest -> DownloadDone
-> SyncComplete (safe unmount)
```

Upload Flow:
```
CompareStep -> UploadStep:
  UploadStart -> CompressFiles -> InitiateUpload -> UploadFile* -> FinalizeManifest -> ListManifestsAfterUpload -> TakeLockAfterUpload -> UploadDone
[Optionally DeletePendingManifest]
-> SyncComplete (safe unmount)
```

*Loops over files; each iteration may schedule async cloud operations and progress UI.

---
## Search Anchors (grep helpers)
Useful strings to quickly navigate:
- "FolderSyncManager::DoWorkFolderDownload"
- "SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete" (two occurrences: download & upload)
- "DownloadStep - " (stage traces)
- "UploadStep - " (stage traces)
- "FinalizeManifest" (end of core upload writes)
- "UpdateLocalManifest" (end of download file writes)

---
_End of document_

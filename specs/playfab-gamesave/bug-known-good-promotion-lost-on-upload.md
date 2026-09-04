# Bug: IsKnownGood promotion never fires — RollbackToLastKnownGood silently falls back to latest manifest

## Title
PFGameSaveFiles: `RollbackToLastKnownGood` has no effect because `IsKnownGood` promotion is lost during upload initialization

## Repro Steps
1. Initialize PFGameSaveFiles, call `PFGameSaveFilesAddUserWithUiAsync` to hydrate baseline manifest **N**.
2. Modify local files and call `PFGameSaveFilesUploadWithUiAsync` to finalize manifest **N+1**.
3. In a new session, call `PFGameSaveFilesAddUserWithUiAsync` to rehydrate **N+1** (making it the active baseline).
4. Modify local files again and call `PFGameSaveFilesUploadWithUiAsync` to finalize manifest **N+2**.
5. On a different device (or after deleting local data), call `PFGameSaveFilesAddUserWithUiAsync` with `rollbackOption = RollbackToLastKnownGood`.
6. **Expected:** Device receives the files from manifest **N+1** (the last known-good version).
7. **Actual:** Device receives the files from manifest **N+2** (the latest version). The rollback option is silently ignored because no manifest has `IsKnownGood = true`.

## Root Cause
Two bugs in the upload pipeline prevented `IsKnownGood` promotion from ever succeeding:

### Bug 1: `InitForUpload()` wiped the baseline version needed for promotion

When the title calls `PFGameSaveFilesUploadWithUiAsync`, the SDK calls `FolderSyncManager::InitForUpload()` which calls `m_uploadStep.Reset()`. This reset clears all upload-step state including `m_originalActivationBaselineVersion` — the version recorded during `AddUserWithUiAsync` that identifies which prior manifest should be promoted.

After the reset, `m_originalActivationBaselineVersion` is always `0`. When the post-upload `PromoteIfNeeded` stage runs, `EvaluateKnownGoodPromotionEligibility()` sees the zero value and returns `NotApplicable` without making any service call. Promotion never happens.

The conflict-upload path inside `ActivateAndSync()` already had the correct pattern — it preserved the baseline version across the reset:

```cpp
// ConflictUpload path (already correct)
uint64_t origBaseline = m_uploadStep.GetOriginalActivationBaselineVersion();
m_uploadStep.Reset();
if (origBaseline != 0)
{
    m_uploadStep.SetOriginalActivationBaselineVersion(origBaseline);
}
```

But the normal upload path in `InitForUpload()` did not.

### Bug 2: Promotion aborted when baseline manifest was pruned from service enumeration

After uploading, the SDK calls `ListManifests` and then scans the returned manifest list to locate the baseline manifest and check its `IsKnownGood` status. If the baseline manifest is not in the list, promotion was skipped with `NotApplicable`.

In practice, the PlayFab service returns a limited manifest window (typically the latest finalized plus one prior). When three or more versions exist (N, N+1, N+2), the intermediate version (N+1) is often pruned from the `ListManifests` response. Since the SDK already knows the exact version to promote (`m_originalActivationBaselineVersion`), the list-presence check is unnecessary — the `UpdateManifest` API accepts the version directly.

## Fix Description

### 1. FolderSyncManager.cpp — Preserve baseline version across upload reset

Modified `InitForUpload()` to save and restore `m_originalActivationBaselineVersion` across the `m_uploadStep.Reset()` call, matching the existing pattern in the conflict-upload path:

```cpp
HRESULT FolderSyncManager::InitForUpload()
{
    m_compareStep.Reset();
    uint64_t origBaseline = m_uploadStep.GetOriginalActivationBaselineVersion();
    m_uploadStep.Reset();
    if (origBaseline != 0)
    {
        m_uploadStep.SetOriginalActivationBaselineVersion(origBaseline);
    }
    m_relockInProgress = false;
    m_relockVersionOffset = 0;
    return S_OK;
}
```

### 2. UploadStep.cpp — Attempt promotion even when baseline is absent from enumeration

Changed the "baseline missing from enumeration" path from returning `NotApplicable` to returning `NeedsUpdateCall`. The `UpdateManifest` request already uses `m_originalActivationBaselineVersion` to specify the target version directly, so the manifest does not need to be present in the local list:

```cpp
// Before (broken):
TRACE_WARNING("[GAME SAVE] KnownGoodPromotion skipped: baseline v:%llu missing from enumeration", ...);
return KnownGoodPromotionResult::NotApplicable;

// After (fixed):
TRACE_WARNING("[GAME SAVE] KnownGoodPromotion: baseline v:%llu not in enumeration, "
              "attempting promotion anyway (service may have pruned it)", ...);
return KnownGoodPromotionResult::NeedsUpdateCall;
```

## Files Changed
| File | Change |
|------|--------|
| `Source/PlayFabGameSave/Source/SyncManager/FolderSyncManager.cpp` | Preserve `originalActivationBaselineVersion` across `InitForUpload()` reset |
| `Source/PlayFabGameSave/Source/SyncManager/UploadStep.cpp` | Attempt `UpdateManifest(MarkAsKnownGood)` even when baseline is not in post-upload manifest list |

## Verification
Scenario 09 (`scenario-09-rollback-last-known-good.yml`) exercises the full promotion and rollback flow:
- Device A uploads three successive versions (baseline N, good patch N+1, corrupted N+2)
- Both N and N+1 are promoted to `IsKnownGood = true` during their respective successor uploads
- Device B calls `AddUserWithUiAsync` with `RollbackToLastKnownGood` and receives N+1 (the good patch)
- Snapshot comparison confirms the rolled-back files match the good-patch state exactly

Scenario 10 (`scenario-10-rollback-last-conflict.yml`) confirms the fixes do not regress the `RollbackToLastConflict` path.

---

# Bug: `assert(false)` in `GetFolderDetailIndexFromFolderId` hangs headless Debug builds

## Title
PFGameSaveFiles: `assert(false)` in `FileFolderSet::GetFolderDetailIndexFromFolderId` pops a modal dialog that hangs headless processes indefinitely

## Repro Steps
1. Build PFGameSaveFiles in **Debug** configuration.
2. Upload a save container with a large file count (e.g., 200 files in a subfolder).
3. In a new session, call `PFGameSaveFilesAddUserWithUiAsync` to sync the container.
4. If any file's `FolderId` in the extended manifest does not match a previously-parsed folder entry, the code hits `assert(false)`.
5. **Expected:** The SDK logs an error and returns index 0 (root folder) as a fallback.
6. **Actual:** In Debug builds, `assert(false)` pops a Windows dialog box. If the process is running headless (e.g., `/notinteractive` test device), the dialog blocks the thread forever.

## Root Cause
`FileFolderSet::GetFolderDetailIndexFromFolderId()` contained `assert(false)` in the "folderId not found" branch. In Debug builds, this assertion triggers a modal dialog rather than a crash, causing the process to hang when no interactive desktop is available.

## Fix Description
Replaced `assert(false)` with `TRACE_ERROR(...)` so the condition is logged but does not block execution:

```cpp
// Before (broken):
if (it == m_folderFolderIdMap.end())
{
    assert(false);
}

// After (fixed):
if (it == m_folderFolderIdMap.end())
{
    TRACE_ERROR("[GAME SAVE] GetFolderDetailIndexFromFolderId: folderId '%s' not found in map (size=%zu)",
                folderId.c_str(), m_folderFolderIdMap.size());
}
```

## Files Changed
| File | Change |
|------|--------|
| `Source/PlayFabGameSave/Source/Types/FileFolderSet.cpp` | Replace `assert(false)` with `TRACE_ERROR` in `GetFolderDetailIndexFromFolderId()` |

## Verification
Scenario 16 (`scenario-16-large-payload-incremental-sync.yml`) exercises the large-payload sync path with 200 files. With this fix, the extended manifest parsing completes without hanging in Debug builds.

## Design Reference
See `specs/playfab-gamesave/design/rollback-add-user-flags.md`:
- Section 6.1: Manifest selection algorithm for `RollbackToLastKnownGood`
- Section 6.4: `IsKnownGood` promotion rules — a manifest becomes known-good after it has been activated as the baseline and a successor has been finalized from it

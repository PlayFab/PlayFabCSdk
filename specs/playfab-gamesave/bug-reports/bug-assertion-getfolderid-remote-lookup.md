# Bug: Assertion Failure in GetFolderId When Keeping Remote Compressed Files

## Status
**Fixed** — SDK fixes applied to ExtendedManifest.cpp, ExtendedManifest.h, and FileFolderSet.cpp

## Summary
When resolving a conflict by choosing "UseCloud", an assertion failure occurs in
`GetFolderId()` if the remote save data contains folders that don't exist locally.
The bug is in both `WriteCompressedFileIndexJson()` and `WriteCompressedFileJson()`
which incorrectly look up folder paths in the local folder structure instead of
using the already-available folder IDs.

Additionally, the manifest's Folders section was missing entries for remote-only
folders, and stale local folder IDs could override correct remote IDs.

## Reproduction
- **Test scenario**: `gamesave-xbox-02-two-device-golden-path.yml`
- **Trigger condition**: Run scenario when local save folder has stale data from
  previous runs, causing a conflict. Choose "UseCloud" to take remote data.
- **Error**: Assertion failure dialog appears:
  ```
  Assertion failed!
  Program: ...\PlayFabGameSave.dll
  File: ...\ExtendedManifest.cpp
  Line: 274
  Expression: false
  ```

## Key Log Lines

```
[05:45:35] PFGameSaveFilesUiConflictCallback (local) time=1774007959 deviceId=DeviceB friendly=JASONSA2019-3 bytes=537801
[05:45:35] PFGameSaveFilesUiConflictCallback (remote) time=1774010712 deviceId=c5bb6e7d... friendly=JASONSA-XBOX bytes=535753
[05:45:35] Auto responder: PFGameSaveFilesSetUiConflictResponse action=UseCloud (hr=0x00000000)
[05:45:38] PFGameSaveFilesUiProgressCallback (state=PreparingForUpload, current=0, total=0)
# ... assertion failure occurs during upload preparation
```

## Root Cause

Three related problems in `ExtendedManifest::WriteExtendedManifest()`:

### Problem 1: Incorrect folder ID lookup via nested tree
When processing files for the extended manifest, the code called
`GetFolderId(nested, folderPath)` to look up folder IDs. This function searches
a nested folder structure built from the LOCAL file folder set only.

**WriteCompressedFileIndexJson (line 407):**
```cpp
const FolderDetail& folderDetail = remoteFileFolderSet->GetFileFolder(&file);
String folderId = GetFolderId(nested, folderDetail.relFolderPath); // BUG
```

**WriteCompressedFileJson (line 435):**
```cpp
String folderId = GetFolderId(nested, extractedFileDetail.relFolderPath); // BUG
```

Both `FolderDetail` and `ExtendedManifestExtractedFileDetail` already have
`folderId` fields populated with valid values. There's no need to look them up.

### Problem 2: Missing remote folders in manifest Folders section
When keeping remote files during upload, their folder references (FolderIds in
the Files section) pointed to folders that were never added to the Folders section.
`CreateNestedStructure()` only adds folders where `existsLocally == true`, so
remote-only folders were absent from the manifest.

On the next sync/download, `InitWithExtendedManifest()` would parse the manifest
and call `GetFolderDetailIndexFromFolderId()` for each file's FolderId — but
the folder wouldn't exist in the map, triggering an assertion failure in
`FileFolderSet.cpp`.

### Problem 3: Stale local folder IDs overriding remote IDs
`CreateNestedFolderJson()` resolved folder IDs by matching `relFolderPath` against
local folders first. But local folders deleted from disk (`existsLocally == false`)
could still match by path and provide an outdated folder ID that differed from
the remote manifest's ID. This caused a mismatch between the Files section
(referencing the remote folder ID) and the Folders section (emitting the stale
local folder ID).

## Fixes Applied

### Fix 1: Use existing folder IDs directly
Changed `WriteCompressedFileIndexJson()` and `WriteCompressedFileJson()` to use
the folder ID already stored on the file/folder detail instead of re-looking it
up via the nested tree:

```cpp
// WriteCompressedFileIndexJson — before:
String folderId = GetFolderId(nested, folderDetail.relFolderPath);
// After:
String folderId = folderDetail.folderId; // use the remote folder's existing ID

// WriteCompressedFileJson — before:
String folderId = GetFolderId(nested, extractedFileDetail.relFolderPath);
// After:
String folderId = extractedFileDetail.folderId; // use existing folder ID
```

Removed unused `nested` parameter from both functions, and deleted the now-unused
`GetFolderId()` and `FindNestedFolder()` helper functions.

### Fix 2: Merge remote folders into nested structure
After collecting all folder IDs referenced by files (`folderIdsInFiles`), the code
now checks which IDs are missing from the local folder structure and adds them
from the remote manifest using `AddPath()`:

```cpp
// Build set of local folder IDs that exist on disk
Set<String> localFolderIds;
for (const FolderDetail& localFolder : localFolders)
{
    if (localFolder.existsLocally)
        localFolderIds.insert(localFolder.folderId);
}

// For each folder ID in files not covered by local folders, add remote path
auto remoteIt = remoteFolderById.find(folderIdInFile);
if (remoteIt != remoteFolderById.end())
    AddPath(nested, remoteFolder->relFolderPath);
```

`AddPath()` properly splits paths like `"saves/profiles"` into nested segments,
preserving the correct folder hierarchy. The folder JSON is then regenerated
after all missing paths have been added.

### Fix 3: Filter stale local folders and fall back to remote IDs
`CreateNestedFolderJson()` now:
1. Only matches local folders where `existsLocally == true`
2. Falls back to `remoteFileFolderSet` for folder ID lookup if not found locally
3. Generates a new GUID only if neither local nor remote has a match

```cpp
// Only consider folders that actually exist on disk
for (const FolderDetail& localFolder : localFolders)
{
    if (localFolder.existsLocally && localFolder.relFolderPath == nestedFolder.relFolderPath)
    {
        nestedFolder.folderId = localFolder.folderId;
        break;
    }
}

// If not found in local, check remote manifest
if (nestedFolder.folderId.empty() && remoteFileFolderSet)
{
    for (const FolderDetail& remoteFolder : remoteFolders)
    {
        if (remoteFolder.relFolderPath == nestedFolder.relFolderPath)
        {
            nestedFolder.folderId = remoteFolder.folderId;
            break;
        }
    }
}
```

### Fix 4: Normalize kept remote file folder IDs to local IDs
When the same folder path exists locally (`existsLocally == true`) and remotely
with different GUIDs, `CreateNestedFolderJson` prefers the local ID for the
Folders section. But `WriteCompressedFileIndexJson` was using the remote ID in
the Files section, causing a mismatch → files silently dropped on re-parse.

Now `WriteCompressedFileIndexJson` checks whether a local folder with the same
path exists, and if so, uses the local folder's ID to match what `CreateNestedFolderJson`
will emit:

```cpp
String folderId = folderDetail.folderId; // start with remote ID
const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();
for (const FolderDetail& localFolder : localFolders)
{
    if (localFolder.existsLocally && localFolder.relFolderPath == folderDetail.relFolderPath)
    {
        folderId = localFolder.folderId; // normalize to local ID
        break;
    }
}
```

### Fix 5: Replace assertion with diagnostic logging
In `FileFolderSet::GetFolderDetailIndexFromFolderId()`, replaced `assert(false)`
with detailed diagnostic logging that dumps the folder map state (up to 20
entries) and returns `SIZE_MAX` so callers can skip the file gracefully.

### Fix 6: Added diagnostic trace logging
Added `TRACE_VERBOSE` and `TRACE_INFORMATION` logging throughout folder parsing
and folder detail insertion to aid future debugging of manifest issues.

## Files Changed

- `Source/PlayFabGameSave/Source/Types/ExtendedManifest.cpp` — Core fixes 1–4, plus logging
- `Source/PlayFabGameSave/Source/Types/ExtendedManifest.h` — Updated function signatures
- `Source/PlayFabGameSave/Source/Types/FileFolderSet.cpp` — Fix 5 (assertion removal) and logging

## Testing

After fix, run `gamesave-xbox-02-two-device-golden-path.yml` with stale local
data to verify UseCloud conflict resolution no longer causes assertion failure.

Verified: test passes with `[PASS] GameSave Xbox 02 - Two-Device Golden Path Sync`.

## Code Review

Changes were reviewed by both Claude Opus 4.6 and GPT-5.4 across multiple rounds:

| Round | Finding | Resolution |
|-------|---------|------------|
| 1 | Remote folders appended flat at root, corrupting nested paths on re-parse | Fixed: use `AddPath()` to properly nest, then regenerate JSON |
| 1 | Deleted local folders included in `localFolderIds`, masking missing remote folders | Fixed: filter on `existsLocally == true` |
| 2 | Stale local folder IDs override remote IDs in `CreateNestedFolderJson` path matching | Fixed: skip local entries where `existsLocally == false` |
| 3 | FolderId mismatch when same path exists locally and remotely with different GUIDs — kept remote files use remote ID but Folders section uses local ID, causing silent data loss on re-parse | Fixed: normalize remote file folder IDs to local IDs in `WriteCompressedFileIndexJson` |

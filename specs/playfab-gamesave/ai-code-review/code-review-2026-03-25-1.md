# PlayFabGameSave Code Review — 2026-03-25

**Reviewer:** GPT-5.4
**Scope:** `Source\PlayFabGameSave\` (excluding Generated/)

## Summary

| Severity | Count |
|----------|-------|
| Critical | 2 |
| Important | 1 |
| Minor | 1 |

## Critical Issues

### CR-1: Optional GRTS UI callbacks are dereferenced unconditionally

**File:** `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_GRTS.cpp:1101-1106, 1122-1127, 1152-1160, 1207-1215, 1259-1263, 1286-1306`

`PFGameSaveUICallbacks` explicitly marks every callback as optional, but the GRTS bridge always registers wrapper callbacks with PFX and those wrappers unconditionally call the stored title callback pointer. A title can legally set only `progressCallback` or only `syncFailedCallback`; when PFX later raises any other UI event, the wrapper dereferences a null function pointer and crashes the title process.

```cpp
// SetUiCallbacks always registers every bridge callback.
gsContext->progressCallback = callbacks->progressCallback;
gsContext->syncFailedCallback = callbacks->syncFailedCallback;
gsContext->activeDeviceContentionCallback = callbacks->activeDeviceContentionCallback;
gsContext->conflictCallback = callbacks->conflictCallback;
gsContext->outOfStorageCallback = callbacks->outOfStorageCallback;

pfxcallbacks.progressCallback = MyPFXPALGameSaveProgressUiCallback;
pfxcallbacks.syncFailedCallback = MyPFXPALGameSaveSyncFailedUiCallback;
pfxcallbacks.activeDeviceContentionCallback = MyPFXGameSaveActiveDeviceContentionUiCallback;
pfxcallbacks.conflictCallback = MyPFXGameSaveConflictUiCallback;
pfxcallbacks.outOfStorageCallback = MyPFXPALGameSaveOutOfStorageUiCallback;

// The bridge later dereferences them without checking.
gsContext->syncFailedCallback(localUser, pfSyncState, error, context);
gsContext->activeDeviceContentionCallback(localUser, &pfLocalGameSave, &pfRemoteGameSave, context);
gsContext->conflictCallback(localUser, &pfLocalGameSave, &pfRemoteGameSave, context);
gsContext->outOfStorageCallback(localUser, requiredBytes, context);
```

**Fix:** Only register each PFX callback when the corresponding title callback is non-null, or guard every bridge invocation with a null check and fall back to stock UI when the title did not supply that callback.

---

### CR-2: Local scan failures are swallowed, so uploads can delete healthy cloud data

**File:** `Source\PlayFabGameSave\Source\Types\LocalStateManifest.cpp:186-225`

`MergeLocalFolders` ignores the `HRESULT` from both `MergeLocalFiles` and recursive `MergeLocalFolders` calls. If a nested folder cannot be enumerated during upload preparation (for example because the save tree contains a too-long path, a sharing violation, or an access-denied subfolder), `InitWithLocalFilesAndFolders` still returns `S_OK` with a partial local inventory. The compare phase then treats previously-synced files that were omitted from this partial scan as local deletions and can queue them for deletion from the cloud on the next upload.

```cpp
if (curFullFolderPath == fullFolderPath)
{
    folder.existsLocally = FilePAL::DoesDirectoryExist(curFullFolderPath);
    foundMatchingFolder = true;
    MergeLocalFiles(rootPath, fullFolderPath, folderIndex);
    break;
}
...
size_t folderIndex = AddFolderDetail(std::move(folder));
MergeLocalFiles(rootPath, fullFolderPath, folderIndex);
...
for (const String& subfolder : subfolders)
{
    String fullSubfolderPath;
    RETURN_IF_FAILED(JoinPathHelper(fullFolderPath, subfolder, fullSubfolderPath));
    MergeLocalFolders(rootPath, subfolder, fullSubfolderPath); // recursively search
}
```

**Fix:** Propagate failures from `MergeLocalFiles` and recursive `MergeLocalFolders` with `RETURN_IF_FAILED`, so any unreadable part of the save tree aborts the sync instead of producing a truncated inventory.

---

## Important Issues

### II-1: Invalid remote folder entries are rejected but the manifest parser ignores the failure

**File:** `Source\PlayFabGameSave\Source\Types\ExtendedManifest.cpp:62-68, 124-130, 552-562`

`ExtendedManifestParseFolderJson` correctly returns `E_INVALIDARG` for unsafe folder names, but both the top-level caller and the recursive descent ignore that return value. A downloaded manifest with one invalid nested folder therefore still parses as success; later, files under the rejected folder are silently skipped because their `FolderId` was never added. The AddUser/download path can complete with an incomplete restore instead of surfacing manifest corruption to the title.

```cpp
if (foldersJson.is_array())
{
    String curPath = "";
    ExtendedManifestParseFolderJson(json, curPath, true, saveFolder);
}
...
for (const auto& subFolderJson : subFoldersJson.get<Vector<JsonValue>>())
{
    ExtendedManifestParseFolderJson(subFolderJson, f.relFolderPath, false, saveFolder);
}
...
e.folderIndex = GetFolderDetailIndexFromFolderId(folderId);
if (e.folderIndex == SIZE_MAX)
{
    TRACE_ERROR("[GAME SAVE] ExtendedManifest: Unknown folderId '%s' for file '%s', skipping", folderId.c_str(), e.fileName.c_str());
    continue;
}
```

**Fix:** Check and propagate the `HRESULT` from every `ExtendedManifestParseFolderJson` call. If any folder entry is invalid, fail manifest parsing and restart the sync through the normal error UI path instead of silently dropping part of the remote save.

---

## Minor Issues

### MI-1: `PFGameSaveFilesGetFolderSize` accepts a null required output buffer on GRTS

**File:** `Source\PlayFabGameSave\Source\Api\PFGameSaveFilesAPI.cpp:114-121` (with the GRTS success path in `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_GRTS.cpp:745-760`)

The public API declares `saveRootFolderSize` as a required `_Out_` parameter, but `PFGameSaveFilesGetFolderSize` never validates it before dispatching to the provider. On Win32 this eventually returns `E_INVALIDARG`, but on the GRTS provider a call such as `PFGameSaveFilesGetFolderSize(user, nullptr)` succeeds and returns `S_OK` without producing any size. That platform-dependent contract break makes it easy for callers to miss an invalid argument bug during development and ship code that behaves differently on GDK.

```cpp
PF_API PFGameSaveFilesGetFolderSize(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ size_t* saveRootFolderSize
) noexcept
{
    return GSApiImpl("PFGameSaveFilesGetFolderSize", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().GetFolderSize(localUserHandle, saveRootFolderSize);
    });
}
```

**Fix:** Add `RETURN_HR_INVALIDARG_IF_NULL(saveRootFolderSize);` in the API entry point (or at minimum in the GRTS provider) so all platforms reject the same invalid call.

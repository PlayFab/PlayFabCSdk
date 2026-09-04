# PlayFabGameSave Code Review — 2026-03-23

Dual-model review of `Source\PlayFabGameSave` (public headers + all implementation files).

**Reviewers:** GPT-5.4, Claude Opus 4.6  
**Scope:** Full codebase under `Source\PlayFabGameSave\`  
**Date:** 2026-03-23

---

## Summary

| Severity | Count |
|----------|-------|
| Critical | 3 |
| Important | 6 |
| Total | 9 |

---

## Critical Issues

### CR-1: Wrong context pointer passed to progress callback

**File:** `Source\PlayFabGameSave\Source\Common\UICallbackManager.cpp:48`  
**Found by:** Both GPT-5.4 and Opus 4.6

`ShowProgressUI` passes `uiInfo.syncFailedContext` instead of `uiInfo.progressContext`:

```cpp
uiInfo.progressCallback(localUser.Handle(), syncState, uiInfo.syncFailedContext);
//                                                      ^^^^^^^^^^^^^^^^^^^^^^^^ WRONG
```

Titles register separate context pointers for each callback type via `PFGameSaveUICallbacks`. The progress callback receives a pointer of the wrong type, leading to crashes or memory corruption when the title dereferences it. Every other `Show*UI` method correctly uses its matching context field.

**Fix:** Change to `uiInfo.progressContext`.

---

### CR-2: Remote manifest paths are trusted — path traversal vulnerability

**File:** `Source\PlayFabGameSave\Source\Types\ExtendedManifest.cpp:509-523`  
**Related:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp:532-571`  
**Found by:** GPT-5.4

Folder/file names from the downloaded manifest are accepted verbatim and combined into `relFolderPath`/`relFilePath`, then used to build extraction targets under `saveFolder`. There is no rejection of `..`, rooted paths, or embedded separators.

Game-save data is effectively untrusted cross-device input. A crafted manifest uploaded by another client for the same user can make this client write downloaded content outside the save root (e.g., `..\..\SomeOtherFolder\file`).

**Fix:** Validate every manifest-sourced name: reject absolute/rooted paths, `.`/`..` segments, and names containing path separators. After joining, canonicalize and enforce the final path resides under `saveFolder`.

---

### CR-3: GetCompressedFileFromFileId returns index-0 element on lookup failure

**File:** `Source\PlayFabGameSave\Source\Types\FileFolderSet.cpp:365-379`  
**Found by:** Opus 4.6

When the fileId is not found in the map, the function hits `assert(false)` (no-op in Release), then falls through with `index = 0` and returns `m_compressedFiles[0]` — an unrelated entry. If `m_compressedFiles` is empty, this is undefined behavior.

```cpp
const CompressedFile& FileFolderSet::GetCompressedFileFromFileId(const String& fileId) const
{
    auto it = m_compressedFilesMap.find(fileId);
    size_t index = 0;
    if (it == m_compressedFilesMap.end())
    {
        assert(false);  // no-op in Release
    }
    else
    {
        index = it->second;
    }
    return m_compressedFiles[index]; // wrong element or UB
}
```

In Release builds with corrupted or stale manifests, this silently returns the wrong compressed file, leading to downloading/decompressing wrong data or data corruption.

**Fix:** Return a `const CompressedFile*` (nullable), or propagate an error. Do not silently return index 0.

---

## Important Issues

### IMP-1: GUID generation uses non-cryptographic, non-thread-safe PRNG

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:19-64`  
**Found by:** Opus 4.6

Two distinct problems:

1. **Thread safety:** `internal_seed` is a non-atomic `static uint64_t` modified without synchronization. `CreateGUID()` is called from multiple threads (download/upload finalizers, folder merge, device ID generation). Concurrent calls produce data races (UB per C++ standard).

2. **Collision risk:** The LCG seeded with `std::time(nullptr)` (second-resolution) produces only 15 bits of randomness per call (`% 32768`). Two processes starting in the same second produce identical GUID sequences. GUIDs are used as `fileId` and `folderId` values — collisions would corrupt save data.

**Fix:** Use `BCryptGenRandom` (Windows) or `std::random_device` with thread-local or mutex-protected state.

---

### IMP-2: Dangling pointer vectors after FileFolderSet vector reallocation

**File:** `Source\PlayFabGameSave\Source\Types\FileFolderSet.h:111-118`  
**Found by:** Opus 4.6

`FileFolderSet` stores raw pointers into `m_files` and `m_folders` vectors:

```cpp
Vector<const FileDetail*> m_filesToUpload;        // raw pointers into m_files
Vector<const FileDetail*> m_filesToDownload;      // raw pointers into m_files
Vector<const FolderDetail*> m_foldersToCreateUponUpload;  // raw pointers into m_folders
```

If `m_files` or `m_folders` are resized after these pointer vectors are populated, all raw pointers become dangling. Currently safe because init completes before marking, but the design is fragile — a single future code change adding an element during sync introduces use-after-free.

**Fix:** Use indices instead of raw pointers, or `reserve()` and document the invariant.

---

### IMP-3: Active-device-changed callback races with re-registration

**File:** `Source\PlayFabGameSave\Source\Common\UICallbackManager.cpp:188-198`  
**Related:** `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_Win32.cpp:380-389`  
**Found by:** GPT-5.4

`TriggerActiveDeviceChangedCallback` checks `activeDeviceChangedCallback` is non-null before queueing work, but the queued lambda re-reads the global callback pointer later without another null check or a stable snapshot. If a title clears or replaces the callback between queue submission and execution, the lambda can call a null function pointer or dispatch to a wrong callback/context pair.

**Fix:** Capture a stable snapshot of the callback function and context before queue submission; invoke the snapshot from the lambda.

---

### IMP-4: DoWorkFolderDownload missing folderSyncMutex lock

**File:** `Source\PlayFabGameSave\Source\SyncManager\FolderSyncManager.cpp:83-397`  
**Found by:** Opus 4.6

`DoWorkFolderUpload` (line 504) acquires `folderSyncMutex` at the top, but `DoWorkFolderDownload` (line 83) does **not**. The download path modifies shared state (`m_latestFinalizedManifest`, `m_latestPendingManifest`, `m_localFileFolderSet`, `m_remoteFileFolderSet`) without the lock. If a `.Finally()` callback fires on another thread while `DoWorkFolderDownload` is running, they race on these shared pointers.

**Fix:** Add `std::lock_guard<std::recursive_mutex> lock(folderSyncMutex)` at the top of `DoWorkFolderDownload`, matching `DoWorkFolderUpload`.

---

### IMP-5: UploadStep Reset() clears conflict metadata during full-set retry

**File:** `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:790-795`  
**Found by:** Opus 4.6

In the `E_PF_INVALID_PARAMS` handler for the 100-file finalize limit:

```cpp
SetToUploadFullSet(localFileFolderSet, remoteFileFolderSet);
Reset(); // clears m_conflictMetadata at line 38
this->m_uploadFullSetRetryCount++;
this->m_stage = UploadStage::CompressFiles; // skips UploadStart where metadata is set
```

`Reset()` clears `m_conflictMetadata` (set during `UploadStart` at line 445). The retry re-enters at `CompressFiles`, skipping `UploadStart`. If this retry path occurs during a conflict upload in the AddUser flow, conflict metadata is lost and `FinalizeManifest` won't record the conflict relationship — breaking rollback.

**Fix:** Save and restore `m_conflictMetadata` across the `Reset()` call.

---

### IMP-6: Local file enumeration can crash if a file disappears mid-scan

**File:** `Source\PlayFabGameSave\Source\Types\LocalStateManifest.cpp:229-266`  
**Found by:** GPT-5.4

After enumerating files, `MergeLocalFiles` calls `FilePAL::GetFileSize(...).Payload()` without checking `hr`, and ignores the return from `GetFileTimes(...)`. If a file is deleted between `EnumFiles` and `GetFileSize`, `Payload()` is invoked on a failed `Result`. In this codebase, `Payload()` asserts a payload exists; in Release builds this becomes undefined behavior.

**Fix:** Check `GetFileSize`/`GetFileTimes` results explicitly. On failure, skip the file or retry.

---

## Appendix: GameSaveGlobalState string getters lack synchronization

**File:** `Source\PlayFabGameSave\Source\Common\GameSaveGlobalState.h:24-46`  
**Found by:** Opus 4.6  
**Severity:** Low-Medium (depends on actual call patterns)

Multiple getter/setter pairs for `String` members (`m_debugRootSaveFolderOverride`, `m_localDeviceID`) are unsynchronized. `GetLocalDeviceID` returns `const String&` — the reference can be invalidated by a concurrent `SetLocalDeviceID`. These are called from API threads (Set via debug APIs) and sync worker threads (Get in `FolderSyncManager`).

**Fix:** Protect with a mutex, or document that these are only set during initialization before concurrent access begins.

---

## Cross-Reviewer Agreement

| Issue | GPT-5.4 | Opus 4.6 |
|-------|---------|----------|
| CR-1 Progress callback wrong context | ✅ | ✅ |
| CR-2 Path traversal | ✅ | — |
| CR-3 GetCompressedFile returns wrong element | — | ✅ |
| IMP-1 GUID PRNG issues | — | ✅ |
| IMP-2 Dangling pointers in FileFolderSet | — | ✅ |
| IMP-3 Active device callback race | ✅ | — |
| IMP-4 Missing mutex in DoWorkFolderDownload | — | ✅ |
| IMP-5 Conflict metadata lost on retry | — | ✅ |
| IMP-6 Crash on file disappearing mid-scan | ✅ | — |

---

## Fix Status

All 9 issues have been fixed. Build passes (0 errors, 0 warnings). Regression tests pending.

| Issue | Status | Files Changed |
|-------|--------|---------------|
| CR-1 | ✅ Fixed | `UICallbackManager.cpp:48` — changed `syncFailedContext` → `progressContext` |
| CR-2 | ✅ Fixed | `ExtendedManifest.cpp` — added path traversal validation for folder and file names (rejects `..`, `.`, `/`, `\`, drive letters) |
| CR-3 | ✅ Fixed | `FileFolderSet.cpp` + `.h` — returns `const CompressedFile*` (nullptr on miss) |
| IMP-1 | ✅ Fixed | `Utils.cpp` — replaced LCG PRNG with `BCryptGenRandom`; thread-safe and cryptographic |
| IMP-2 | ✅ Documented | `FileFolderSet.h` — added invariant comment documenting that m_files/m_folders must not grow after pointer vectors are populated |
| IMP-3 | ✅ Fixed | `UICallbackManager.cpp:188-202` — snapshot callback+context before queue submission |
| IMP-4 | ✅ Fixed | `FolderSyncManager.cpp:83` — added `std::lock_guard` matching upload path pattern |
| IMP-5 | ✅ Fixed | `UploadStep.cpp:790-795` — save/restore `m_conflictMetadata` across `Reset()` |
| IMP-6 | ✅ Fixed | `LocalStateManifest.cpp:253,265` — check `GetFileSize`/`GetFileTimes` results, skip file on failure |

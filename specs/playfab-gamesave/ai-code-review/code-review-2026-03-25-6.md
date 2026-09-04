# PlayFabGameSave Code Review — 2026-03-25

**Reviewer:** Claude Opus 4.6
**Scope:** `Source\PlayFabGameSave\Source\` (excluding Generated/)

## Summary

| Severity | Count |
|----------|-------|
| Critical | 2 |
| Important | 4 |
| Minor | 3 |

## Critical Issues

### CR-1: GUID generator uses non-cryptographic PRNG seeded with seconds-resolution time — collision risk for device IDs

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:19-28`

The `CreateGUID()` function is used to generate device IDs (`GetLocalDeviceID` line 305) and file/folder IDs throughout the SDK. It uses a Linear Congruential Generator seeded with `std::time(nullptr)` (seconds resolution). The LCG has only 15 bits of output per call (`% 32768`), and the seed is predictable. Two devices or processes initialized in the same second will produce identical GUIDs. Since device IDs are used for active-device contention detection (ActiveDevicePollWorker compares `latestPendingDeviceId != localDeviceId`), colliding IDs could cause the SDK to fail to detect that another device has taken the active lock, leading to silent data loss when both devices upload concurrently.

```cpp
static uint64_t internal_seed = 0;
uint32_t internal_rand(void)
{
    if (internal_seed == 0)
    {
        internal_seed = static_cast<uint64_t>(std::time(nullptr)); // seconds resolution — predictable, collides
    }
    internal_seed = internal_seed * LCG_MULTIPLIER + LCG_INCREMENT;
    return (uint32_t)(internal_seed / LCG_MODULUS_DIVISOR) % LCG_MODULUS_RANGE; // only 15 bits of entropy
}
```

**Fix:** Replace the LCG with a proper random source. On Windows, use `BCryptGenRandom` or `std::random_device` to fill the GUID bytes. At minimum, mix in high-resolution time (`std::chrono::high_resolution_clock`), process ID, and thread ID as seed material. Consider using `CoCreateGuid` / `UuidCreate` on Windows platforms.

---

### CR-2: `internal_seed` is a non-atomic global written from multiple threads without synchronization — data race (UB)

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:19-28`

`internal_seed` is a plain `static uint64_t` that is read and written by `internal_rand()`, which is called from `CreateGUID()`. `CreateGUID()` is called from multiple code paths that can execute concurrently: `MergeLocalFolders` (provider work thread), `GetLocalDeviceID` (called from CompareStep which runs on async callbacks), and `ExtendedManifest::CreateNestedFolderJson`. Concurrent read-modify-write of a non-atomic variable is undefined behavior per C++14/17. On x86-64 this is likely to produce duplicate random sequences (two threads read same seed, compute same next value) rather than a crash, compounding the collision problem in CR-1.

```cpp
static uint64_t internal_seed = 0; // Not atomic, not mutex-protected
uint32_t internal_rand(void)
{
    if (internal_seed == 0) // TOCTOU race
    {
        internal_seed = static_cast<uint64_t>(std::time(nullptr));
    }
    internal_seed = internal_seed * LCG_MULTIPLIER + LCG_INCREMENT; // Non-atomic RMW
    return (uint32_t)(internal_seed / LCG_MODULUS_DIVISOR) % LCG_MODULUS_RANGE;
}
```

**Fix:** Either make `internal_seed` a `std::atomic<uint64_t>` and use `compare_exchange` for the seeding and `fetch_add`-style updates, or protect calls with a mutex. Better yet, replace the entire custom PRNG as recommended in CR-1.

---

## Important Issues

### II-1: `WaitForOutOfStorageUI` skips storage re-check when user reports space cleared — download proceeds into full disk

**File:** `Source\PlayFabGameSave\Source\SyncManager\DownloadStep.cpp:450-454`

When the user responds with `UIOutOfStorageSpaceCleared`, the state transitions directly to `DownloadStage::Download` (line 453), bypassing the storage size check in `QueryStorage`. If the user did not actually free enough space (or freed none at all), the download will proceed and file writes will fail mid-operation, potentially leaving save data in a partially-overwritten corrupt state.

```cpp
if (uiAction == UIAction::UIOutOfStorageSpaceCleared)
{
    m_stage = DownloadStage::Download; // Should go to QueryStorage to re-check
    task.ScheduleNow();
}
```

**Fix:** Transition to `DownloadStage::QueryStorage` instead of `DownloadStage::Download` so the available storage is re-validated before proceeding.

---

### II-2: `UIAction` is not reset between UI interactions — stale action from one UI flow leaks into the next

**File:** `Source\PlayFabGameSave\Source\Common\UICallbackManager.h:50` / `UICallbackManager.cpp:9-31`

`m_action` is an `atomic<UIAction>` that is set by `SetAction()` and read by `GetAction()`. It is never explicitly cleared between UI interactions. For example, if the user responds to a SyncFailed callback with `UISyncFailedRetry`, that action value persists. If the SDK subsequently shows a Conflict UI and the game takes time to respond, any DoWork call that happens to check `GetAction()` will see the stale `UISyncFailedRetry` value. The `HandleFailedUI` method reads `GetAction()` without an exchange, and `WaitForConflictUI` reads it directly — a stale value from a previous interaction could cause incorrect branching.

```cpp
HRESULT UICallbackManager::SetAction(UIAction action)
{
    // ...
    m_action.store(action);
    activeTask->ScheduleNow();
    return S_OK;
}
// m_action is never reset to UINone after being consumed
```

**Fix:** Reset `m_action` to `UIAction::UINone` at the start of each Show*UI method before storing `m_activeTask`, so a fresh UI interaction always starts with a clean state.

---

### II-3: `GameSaveGlobalState` debug flag getters/setters have inconsistent locking — torn reads possible on non-x86

**File:** `Source\PlayFabGameSave\Source\Common\GameSaveGlobalState.h:27-31,36-40`

Several debug flag accessors use no synchronization at all:
```cpp
bool GetForceOutOfStorageError() { return m_forceOutOfStorageError; }
void SetForceOutOfStorageError(_In_ bool forceError) { m_forceOutOfStorageError = forceError; }
// Same pattern for m_forceSyncFailedError, m_forceNullPendingManifest, m_writeManifests, m_debugManifestOffset
```

These are plain `bool` / `int64_t` members read and written from multiple threads (the debug APIs are called from the game thread, while the values are read from provider work threads). `m_debugManifestOffset` is `int64_t` — on 32-bit ARM platforms, a 64-bit non-atomic read can tear. Even on x64 where `bool` is naturally atomic, the lack of memory ordering means changes may not be visible across threads in a timely manner.

**Fix:** Make these members `std::atomic<bool>` and `std::atomic<int64_t>`, or protect them with `m_managersMutex` like the string members.

---

### II-4: `DownloadStep::UncompressFile` for `CompressionType::None` moves source file to first matching extracted file — second file would fail with missing source

**File:** `Source\PlayFabGameSave\Source\SyncManager\DownloadStep.cpp:61-82`

For `CompressionType::None` and `CompressionType::GZip`, the code iterates all `files` looking for any with matching `compressedFileIndex`. For each match it calls `FilePAL::MoveLocalFile(fullCompressedFilePath, fullExtractedFilePath)`. The assert at line 81 checks `numExtractedFiles == 1`, but this is only enforced in debug builds. If a malformed manifest (from a compromised server or corrupted download) maps multiple extracted files to a single `None`-compressed file, the first `MoveLocalFile` succeeds, but subsequent ones fail because the source file no longer exists. The function would then `RETURN_IF_FAILED` and propagate the error, but only after the first file was already moved, leaving an inconsistent state.

```cpp
case CompressionType::None:
case CompressionType::GZip:
{
    // ...
    for (const FileDetail& extractedFile : files)
    {
        if (extractedFile.compressedFileIndex == remoteFile.compressedFileIndex)
        {
            // ... 
            RETURN_IF_FAILED(FilePAL::MoveLocalFile(fullCompressedFilePath, fullExtractedFilePath));
            numExtractedFiles++;
        }
    }
    assert(numExtractedFiles == 1); // Debug-only — no Release guard
    break;
}
```

**Fix:** Add a Release-build guard: after the first successful move, `break` out of the loop. Or validate `numExtractedFiles` before attempting the move and return `E_UNEXPECTED` if more than one file maps to a `None`-compressed entry.

---

## Minor Issues

### MI-1: `GetCompressedFilesToUpload()` returns by value instead of by const reference — unnecessary copy of potentially large vector

**File:** `Source\PlayFabGameSave\Source\Types\FileFolderSet.h:25` / `FileFolderSet.cpp:163-165`

All other getter methods on `FileFolderSet` return `const Vector<T>&`. This one returns `const Vector<ExtendedManifestCompressedFileDetail>` by value, causing a deep copy of the vector (including all nested `Vector<ExtendedManifestExtractedFileDetail>` and `SharedPtr<ArchiveContext>` members) every time it's called.

```cpp
const Vector<ExtendedManifestCompressedFileDetail> FileFolderSet::GetCompressedFilesToUpload() const
{
    return m_compressedFilesToUpload;
}
```

**Fix:** Change the return type to `const Vector<ExtendedManifestCompressedFileDetail>&` to match the pattern of all other getters.

---

### MI-2: `CompareStep::CompareWithCloud` switch statement has no `default` case — silent fall-through on unexpected enum value

**File:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp:48-360`

The `switch (m_stage)` in `CompareWithCloud` handles all declared `CompareStage` enum values but has no `default` case. If `m_stage` somehow holds an invalid value (e.g., due to memory corruption or an enum addition without a handler), the function silently returns `S_OK` at line 359, which would cause the caller to believe the compare completed without error.

```cpp
switch (m_stage)
{
    case CompareStage::GetManifestDownloadDetails: { ... }
    case CompareStage::GetExtendedManifest: { ... }
    // ... all cases ...
    case CompareStage::CompareDone: { ... }
}
return S_OK; // Falls through here with no diagnostic
```

**Fix:** Add a `default` case that returns `E_UNEXPECTED` and logs an error. Apply the same fix to the similar switch in `DownloadStep::Download` (line 125-483) which also falls through to `return S_OK`.

---

### MI-3: `UploadStep::SplitUploadsIntoZipBatches` can produce duplicate file in same batch via `PopFileDetailBelowSize` swap-and-pop

**File:** `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:301-347`

`PopFileDetailBelowSize` uses swap-with-last-then-pop to remove an element. Back in `SplitUploadsIntoZipBatches`, after a batch overflows (line 322-328), if no file fits the remaining budget, the code pushes `filesToUpload.back()` to `fileZipBatchSet` and pops it. However, between the `PopFileDetailBelowSize` call (which returned nullptr) and this fallback, the vector order was potentially changed by a previous `iter_swap`. While this doesn't cause a bug per se (the pointers are still valid and no duplicates occur because the pop removes the element), the non-obvious iteration order makes the batching non-deterministic and harder to reason about for correctness. If `fileSizeBytes` is 0 for a file (e.g., an empty file), `PopFileDetailBelowSize` would still match it (0 <= limit), so the infinite-loop concern doesn't apply, but the algorithm deserves a comment clarifying this invariant.

**Fix:** Add a comment documenting that `fileSizeBytes == 0` files will always be matched by `PopFileDetailBelowSize` (since `0 <= any limit`), preventing infinite loops. Consider using a stable removal strategy (erase-from-vector) if deterministic ordering matters for reproducible manifests.

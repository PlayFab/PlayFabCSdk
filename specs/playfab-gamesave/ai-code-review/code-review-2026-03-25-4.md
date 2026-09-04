# PlayFabGameSave Code Review — 2026-03-25

**Reviewer:** Claude Opus 4.6
**Scope:** `Source\PlayFabGameSave\` (excluding Generated/)

## Summary

| Severity | Count |
|----------|-------|
| Critical | 2 |
| Important | 5 |
| Minor | 3 |

## Critical Issues

### CR-1: GUID generation uses non-thread-safe global state — concurrent calls produce duplicate GUIDs

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:19-28`

`CreateGUID()` relies on `internal_rand()` which reads and writes a static `internal_seed` variable without any synchronization. `CreateGUID()` is called from `FolderSyncManager` constructors, `MergeLocalFolders`, `AddFileDetail`, and `ExtendedManifest::CreateNestedFolderJson` — all of which can execute on different async work threads. Two threads calling `internal_rand()` simultaneously will read the same seed value and produce identical "random" numbers, resulting in duplicate GUIDs for file IDs or folder IDs. Duplicate IDs corrupt the `FileFolderSet` maps (`m_folderFolderIdMap`, `m_compressedFilesMap`) causing later lookups to return wrong data, which leads to files being associated with wrong folders or silently dropped during sync.

```cpp
static uint64_t internal_seed = 0;
uint32_t internal_rand(void)
{
    if (internal_seed == 0)
    {
        internal_seed = static_cast<uint64_t>(std::time(nullptr));
    }
    internal_seed = internal_seed * LCG_MULTIPLIER + LCG_INCREMENT;
    return (uint32_t)(internal_seed / LCG_MODULUS_DIVISOR) % LCG_MODULUS_RANGE;
}
```

**Fix:** Protect `internal_seed` with a mutex, or replace the LCG with a thread-local RNG (e.g., `thread_local std::mt19937_64`). Alternatively, use a platform GUID API (e.g., `CoCreateGuid` on Windows) which is inherently thread-safe.

---

### CR-2: `WriteExtendedManifest` casts `vector::size()` to `int32_t` — vectors with >2B elements wrap negative and skip all files; vectors between INT32_MAX and UINT32_MAX iterate out of bounds

**File:** `Source\PlayFabGameSave\Source\Types\ExtendedManifest.cpp:293-298`

`compressedFilesToUpload.size()` returns `size_t` (64-bit unsigned). The code casts it to `int32_t`. If the vector has more than `INT32_MAX` elements (unlikely but theoretically possible for a manipulated input) or, more practically, if `compressedIncludesExtendedManifest` is `true` and the vector is empty, `numCompressedFiles` becomes `-1` (since `(int32_t)0 - 1 == -1`), and the loop `for (int32_t i = 0; i < -1; ++i)` never executes — silently producing an extended manifest with no file entries, which causes the cloud to have an empty manifest after upload.

```cpp
int32_t numCompressedFiles = (int32_t)compressedFilesToUpload.size();
if (compressedIncludesExtendedManifest)
{
    numCompressedFiles--;
}
for (int32_t i = 0; i < (int32_t)numCompressedFiles; ++i)
```

**Trigger:** Call `WriteExtendedManifest` with `compressedIncludesExtendedManifest = true` and an empty `compressedFilesToUpload` vector. This happens if `CompressFiles` returns an empty vector but the caller adds only the extended manifest entry.

**Fix:** Use `size_t` for `numCompressedFiles`. Guard the decrement: `size_t numCompressedFiles = compressedFilesToUpload.size(); if (compressedIncludesExtendedManifest && numCompressedFiles > 0) { numCompressedFiles--; }`.

---

## Important Issues

### II-1: `GameSaveGlobalState` getters/setters are not thread-safe — concurrent access to string members is a data race

**File:** `Source\PlayFabGameSave\Source\Common\GameSaveGlobalState.h:24-46`

All getter/setter pairs for string members (e.g., `GetDebugRootFolderOverride`/`SetDebugRootFolderOverride`, `GetLocalDeviceID`/`SetLocalDeviceID`, `GetInitArgsSaveRootFolder`/`SetInitArgsSaveRootFolder`) access `String` members without holding `m_managersMutex` or any other lock. These methods can be called from different threads — the API layer calls setters from the caller's thread while `GetLocalDeviceID` in `Utils.cpp` is called from async provider work threads. Concurrent read and write of a `std::basic_string` is undefined behavior per C++ standard, which can cause crashes or corrupted strings.

```cpp
String GetDebugRootFolderOverride() { return m_debugRootSaveFolderOverride; }
void SetDebugRootFolderOverride(const String& s) { m_debugRootSaveFolderOverride = s; }
// ... same pattern for all string getters/setters
const String& GetLocalDeviceID() { return m_localDeviceID; }
void SetLocalDeviceID(const String& s) { m_localDeviceID = s; }
```

**Trigger:** Game calls `PFGameSaveFilesSetMockDeviceIdForDebug` on the UI thread while an async provider is calling `GetLocalDeviceID()` on a work thread. Also applies to `GetInitArgsSaveRootFolder` / `SetInitArgsSaveRootFolder`.

**Fix:** Either protect all getter/setter pairs with `m_managersMutex`, or use a separate mutex for the configuration fields. The `bool` and `uint64_t` fields also lack atomicity guarantees per the C++ memory model but are less likely to cause crashes.

---

### II-2: `GetDebugManifestOffset` return type mismatch — `uint64_t` silently converted to `int64_t`, then added to `uint64_t`

**File:** `Source\PlayFabGameSave\Source\Common\GameSaveGlobalState.h:31` and `Source\PlayFabGameSave\Source\Common\Utils.h:23` and `Source\PlayFabGameSave\Source\SyncManager\LockStep.cpp:474-476`

`GameSaveGlobalState::GetDebugManifestOffset()` returns `uint64_t`, but the free function `GetDebugManifestOffset()` in `Utils.h` returns `int64_t`. The implicit narrowing conversion from `uint64_t` to `int64_t` is implementation-defined if the value exceeds `INT64_MAX`. More importantly, in `LockStep.cpp:476`, the result (`int64_t`) is added to `m_manifestVersionOffset` (`uint64_t`) in an expression passed to `CreateInitManifestRequest` which takes a `uint64_t` parameter. If the debug offset is negative (which the `int64_t` signature suggests is intended), the unsigned addition wraps around, producing a very large version number.

```cpp
// GameSaveGlobalState.h:31
uint64_t GetDebugManifestOffset() { return m_debugManifestOffset; }

// Utils.h:23
int64_t GetDebugManifestOffset();

// LockStep.cpp:474-476
int64_t debugManifestOffset = GetDebugManifestOffset();
CreateInitManifestRequest(m_entity.value(), initManifestRequest, baseManifestVersion,
    newManifestVersion, m_manifestVersionOffset + debugManifestOffset, saveFolder);
```

**Fix:** Align the return types — either both should be `uint64_t` or both `int64_t`. If negative offsets are intentional for debugging, use `int64_t` throughout and add overflow checks before the addition.

---

### II-3: `GameSaveUiCallbackInfo` is a process-global static with no synchronization — race between callback registration and invocation

**File:** `Source\PlayFabGameSave\Source\Common\GameSaveUICallbackInfo.cpp:11-15` and `Source\PlayFabGameSave\Source\Common\UICallbackManager.cpp:45-48`

`GetGameSaveUiCallbackInfo()` returns a reference to a `static` local `GameSaveUiCallbackInfo` instance. The callback function pointers and context pointers are read by `UICallbackManager` methods (e.g., `ShowProgressUI`, `ShowSyncFailedUI`) on async work threads, while they are written by the public API `PFGameSaveFilesSetUiCallbacks` on the caller's thread. Reading a non-atomic pointer while another thread writes it is a data race (undefined behavior). In practice, this could cause a crash if the work thread reads a partially-written function pointer and calls through it.

```cpp
GameSaveUiCallbackInfo& GetGameSaveUiCallbackInfo() noexcept
{
    static GameSaveUiCallbackInfo info{};
    return info;
}
```

**Trigger:** Call `PFGameSaveFilesSetUiCallbacks` while an `AddUserWithUiAsync` operation is in progress on a work thread.

**Fix:** Either protect access to `GameSaveUiCallbackInfo` with a mutex, or use `std::atomic<>` for the function pointer and context fields. Alternatively, require callbacks to be set before any async operation starts and document this as a precondition.

---

### II-4: `TriggerActiveDeviceChangedCallback` snapshots callback/context without synchronization — stale or torn reads

**File:** `Source\PlayFabGameSave\Source\Common\UICallbackManager.cpp:184-199`

`TriggerActiveDeviceChangedCallback` reads `uiInfo.activeDeviceChangedCallback` and `uiInfo.activeDeviceChangedContext` without any lock or atomic operation, then captures them in a lambda for deferred execution. If `PFGameSaveFilesSetActiveDeviceChangedCallback` is called concurrently (e.g., during cleanup), the snapshotted callback pointer may be stale (already freed context) or torn (mismatched callback+context pair), leading to a crash when the lambda executes.

```cpp
// Snapshot callback and context before queuing to avoid racing with re-registration
auto callbackSnapshot = uiInfo.activeDeviceChangedCallback;
auto contextSnapshot = uiInfo.activeDeviceChangedContext;

RunContext rc = runContext.DeriveOnQueue(uiInfo.activeDeviceChangedCallbackQueue);
auto handle = localUser.Handle();
rc.TaskQueueSubmitCompletion([activeDevice, handle, callbackSnapshot, contextSnapshot]() mutable
{
    callbackSnapshot(handle, &activeDevice, contextSnapshot);
});
```

**Trigger:** Call `PFGameSaveFilesSetActiveDeviceChangedCallback(nullptr, nullptr, nullptr)` to unregister while `ActiveDevicePollWorker::CheckActiveDevice` detects a device change and calls `TriggerActiveDeviceChangedCallback`. The snapshot may capture the old callback pointer but a zeroed context, or vice versa.

**Fix:** This is the same root cause as II-3. Protect `GameSaveUiCallbackInfo` with a mutex and hold the lock while snapshotting all three fields (callback, context, queue).

---

### II-5: `LocalStateManifest::WriteLocalManifest` has no write-rename atomicity — power loss during write corrupts `localstate.json`

**File:** `Source\PlayFabGameSave\Source\Types\LocalStateManifest.cpp:393-396`

`WriteLocalManifest` calls `WriteEntireFile(filePath, vData)` which opens the file for writing, truncating it, then writes new content. If the process is killed or power is lost after truncation but before the write completes, `localstate.json` will be empty or partially written. On next launch, the JSON parse will fail, and all sync tracking data is lost — the SDK will re-download everything (data loss of local change tracking) and potentially trigger a false conflict.

```cpp
if (SUCCEEDED(pathHr))
{
    writeHr = WriteEntireFile(filePath, vData);
}
```

**Fix:** Use a write-to-temp-then-rename pattern: write to `localstate.json.tmp`, then atomically rename to `localstate.json`. On POSIX, `rename()` is atomic. On Windows, use `MoveFileEx` with `MOVEFILE_REPLACE_EXISTING`. This ensures the file is always in a consistent state.

---

## Minor Issues

### MI-1: `internal_rand` LCG has only 15 bits of effective output — GUIDs have extremely low entropy

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:20-28`

`internal_rand()` returns `(seed / 65536) % 32768`, which produces values in [0, 32767] — only 15 bits of entropy per call. Each hex digit in `CreateGUID` calls `internal_rand() % 16`, consuming only 4 bits of the 15-bit output. A 32-hex-digit GUID should have 128 bits of entropy, but this implementation has at most 32 × 15 = 480 bits of LCG state evolution, all derived from a single `time(nullptr)` seed (roughly 32 bits). This makes GUIDs predictable and increases collision probability in multi-process scenarios.

```cpp
return (uint32_t)(internal_seed / LCG_MODULUS_DIVISOR) % LCG_MODULUS_RANGE;
```

**Fix:** Use a platform-native UUID generator (`CoCreateGuid` on Windows, `uuid_generate` on Linux) or at minimum a cryptographic PRNG. The LCG is insufficient for identifiers that must be globally unique.

---

### MI-2: `AddFileDetail` returns `SIZE_MAX` on failure but callers in `InitWithExtendedManifest` and `MergeLocalFiles` ignore the return value

**File:** `Source\PlayFabGameSave\Source\Types\FileFolderSet.cpp:50-65` and `Source\PlayFabGameSave\Source\Types\ExtendedManifest.cpp:142`

`AddFileDetail` can return `SIZE_MAX` if `JoinPathHelper` fails (e.g., invalid path characters). However, callers discard the return value. If `AddFileDetail` fails, the file is not added to `m_filePathMap`, but `m_files` still gets the push_back on line 63 — the file is in the vector but not in the map, creating an inconsistent state where lookups by path fail but iteration includes the entry.

```cpp
// In AddFileDetail (FileFolderSet.cpp:50-65):
size_t FileFolderSet::AddFileDetail(FileDetail&& fileDetail)
{
    size_t newIndex = m_files.size();
    // ...
    if (FAILED(hr))
    {
        TRACE_ERROR("[GAME SAVE] ...");
        return SIZE_MAX;  // error return — file silently dropped
    }
    m_filePathMap[relFilePath] = newIndex;
    m_files.push_back(std::move(fileDetail));
    return newIndex;
}

// In ExtendedManifest.cpp:142 (caller):
fileFolderSet->AddFileDetail(std::move(fileDetail)); // return value ignored
```

The `return SIZE_MAX` exits before the file is added to the vector, so internal state stays consistent. However, callers (e.g., `InitWithExtendedManifest`) discard the return value, so a file from the cloud manifest is silently dropped without the user or any error path knowing. This means a save that contains a file with unusual path characters will quietly lose that file on download.

**Fix:** Callers should check the return value of `AddFileDetail` and propagate the error rather than silently dropping the file.

---

### MI-3: `GetTopLevelFolder` assumes relative paths start with a path separator, but root-level files have empty `relFolderPath`

**File:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp:620-637`

`GetTopLevelFolder` skips the first character (`relFolderPath.c_str() + 1`) assuming it's a leading path separator. For files in the root folder (where `relFolderPath` is `""`), `relFolderPath.length()` is 0, so the `if (relFolderPath.length() > 1)` guard protects against the skip. However, for a single-segment path like `\saves` (length 6, leading backslash), the function correctly returns `\saves`. But for a path like `saves` (no leading separator, length 5), `strchr` searches from `"aves"` — the `s` is skipped, and if there's no further separator, it returns the full string anyway. This only matters if `JoinPathHelper` ever produces paths without a leading separator, which appears platform-dependent.

```cpp
String GetTopLevelFolder(const String& relFolderPath)
{
    if (relFolderPath.length() > 1)
    {
        const char* firstSep = strchr(
            relFolderPath.c_str() + 1, // +1 to skip the leading path separator
            FilePAL::GetPathSeparatorChar());
```

**Fix:** Instead of assuming a leading separator, find the first separator from position 0, then extract the first path segment. Or normalize all `relFolderPath` values to never have a leading separator.

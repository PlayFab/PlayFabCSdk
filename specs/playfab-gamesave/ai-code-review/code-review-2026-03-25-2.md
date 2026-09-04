# PlayFabGameSave Code Review — 2026-03-25

**Reviewer:** Claude Opus 4.6
**Scope:** `Source\PlayFabGameSave\` (excluding Generated/)

## Summary

| Severity | Count |
|----------|-------|
| Critical | 2 |
| Important | 4 |
| Minor | 3 |

## Critical Issues

### CR-1: UploadFileFinally accumulates progress counters and deletes zip on failed uploads

**File:** `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:360-374`

`UploadFileFinally` unconditionally accumulates `m_currentUncompressedSizeBytes` and `m_currentCompressedSizeBytes` and deletes the local zip file **before** checking whether `hr` indicates success or failure. When a file upload fails and the user chooses "Retry", the retry re-enters `UploadStage::UploadFile` with the same `m_compressedFilesToUploadCurIndex`, but `m_currentCompressedSizeBytes` has already been incremented and the zip file has been deleted from disk. The retry will attempt to upload a deleted file and will report inflated progress to the UI.

```cpp
void UploadStep::UploadFileFinally(...)
{
    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex);
    
    auto& detail = m_compressedFilesToUpload[m_compressedFilesToUploadCurIndex];
    // BUG: These lines execute unconditionally, even on failure
    m_currentUncompressedSizeBytes += detail.uncompressedSizeBytes;
    m_currentCompressedSizeBytes += detail.compressedSizeBytes;
    if (detail.archiveContext)
    {
        // BUG: Deletes the zip file even when upload failed — retry cannot re-upload
        HRESULT deleteResult = FilePAL::DeleteLocalFile(detail.fullFilePath);
        ...
    }

    if (FAILED(hr))
    {
        // User may choose Retry → goes back to UploadFile stage
        // but the file is already deleted and progress is already counted
```

**Fix:** Move the progress accumulation and zip file deletion into the `SUCCEEDED(hr)` branch. On failure, progress should not be accumulated and the zip file must be preserved so it can be re-uploaded on retry.

---

### CR-2: GUID generator uses a weak non-thread-safe PRNG with global mutable state

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:19-63`

`CreateGUID()` is used to generate file IDs, folder IDs, and device IDs that are stored in manifests and used for identity/deduplication across devices. The implementation uses a bare `static uint64_t internal_seed` with no synchronization, seeded from `time(nullptr)` (1-second resolution). The LCG `(seed * 1103515245 + 12345) / 65536 % 32768` produces only 15 bits of entropy per call, and a full GUID uses 32 hex digits = 128 bits, but only gets ~480 bits of LCG output with at most ~15 bits of initial seed entropy. Two processes starting within the same second will generate identical GUIDs. The static seed also has a data race if `CreateGUID()` is called from multiple threads (e.g., async callbacks).

```cpp
static uint64_t internal_seed = 0;   // unsynchronized global
uint32_t internal_rand(void)
{
    if (internal_seed == 0)
    {
        internal_seed = static_cast<uint64_t>(std::time(nullptr)); // 1-second resolution seed
    }
    internal_seed = internal_seed * LCG_MULTIPLIER + LCG_INCREMENT;
    return (uint32_t)(internal_seed / LCG_MODULUS_DIVISOR) % LCG_MODULUS_RANGE; // only 15 bits
}
```

**Fix:** Replace the custom PRNG with a platform GUID API (e.g., `UuidCreate` on Windows, `CoCreateGuid`) or at minimum use `<random>` with `std::random_device` seeding and a thread-local or mutex-protected generator. Device IDs in particular must be unique across devices to avoid active-device contention misidentification.

---

## Important Issues

### II-1: Signed/unsigned comparison in GetTotalUncompressedSize loop variable

**File:** `Source\PlayFabGameSave\Source\Types\FileFolderSet.cpp:425`

The loop variable `iFile` is `int` but `m_files.size()` returns `size_t`. When the vector has more than `INT_MAX` elements (theoretically possible with a crafted manifest), the comparison `iFile < m_files.size()` will always be true after `iFile` wraps to negative, causing an infinite loop. More practically, the signed/unsigned comparison generates a compiler warning and the `int` index is used to access the vector, which with a large enough vector could produce undefined behavior via signed integer overflow.

```cpp
uint64_t FileFolderSet::GetTotalUncompressedSize() const
{
    uint64_t totalSize = 0;
    for (int iFile = 0; iFile < m_files.size(); iFile++)  // int vs size_t
    {
        const FileDetail& file = m_files[iFile];
```

**Fix:** Change `int iFile` to `size_t iFile`.

---

### II-2: GetRemainingQuota casts uint64_t to int64_t without overflow check

**File:** `Source\PlayFabGameSave\Source\SyncManager\FolderSyncManager.cpp:862`

`GetTotalUncompressedSize()` returns `uint64_t`. If the total local file size exceeds `INT64_MAX` (~9.2 EB), `static_cast<int64_t>(localSize)` produces implementation-defined behavior (typically wraps to negative), making `remaining` appear as a huge positive number — suggesting the user has unlimited quota when they are actually over quota. While unlikely with typical save sizes, a corrupted or malicious `localstate.json` could report inflated `fileSizeBytes` values that sum past `INT64_MAX`.

```cpp
uint64_t localSize = 0;
if (m_localFileFolderSet)
{
    localSize = m_localFileFolderSet->GetTotalUncompressedSize();
}
int64_t remaining = perPlayerQuota - static_cast<int64_t>(localSize);  // UB if localSize > INT64_MAX
```

**Fix:** Clamp `localSize` to `INT64_MAX` before the cast, or compare as `uint64_t` (treating `perPlayerQuota` as unsigned when it's not `INT64_MAX`).

---

### II-3: UICallbackManager stores raw task pointer that may dangle after ShowSyncFailedUI returns false

**File:** `Source\PlayFabGameSave\Source\Common\UICallbackManager.cpp:96-108`

In `ShowSyncFailedUI`, `m_activeTask` is set to `&task` **before** checking whether the callback is registered. If no callback is registered, it's set back to `nullptr` on line 105. However, between line 97 (`m_activeTask.store(&task)`) and line 100 (`if (uiInfo.syncFailedCallback)`), a concurrent call to `SetAction()` from another thread could read the stale `m_activeTask` pointer and call `ScheduleNow()` on it. This is a narrow race window, but `SetAction` does `m_activeTask.exchange(nullptr)` and then calls `activeTask->ScheduleNow()`.

```cpp
bool UICallbackManager::ShowSyncFailedUI(ISchedulableTask& task, ...)
{
    m_activeTask.store(&task);       // Line 97: sets task pointer

    auto& uiInfo = GetGameSaveUiCallbackInfo();
    if (uiInfo.syncFailedCallback)   // Line 100: check callback exists
    {
        uiInfo.syncFailedCallback(...);
        return true;
    }
    m_activeTask.store(nullptr);     // Line 105: cleanup on no-callback path
    return false;
}
```

**Fix:** Only store `m_activeTask` after confirming the callback is registered (move the `m_activeTask.store(&task)` inside the `if` block), or use a lock to make the check-and-store atomic.

---

### II-4: FolderSyncManager::InitForDownload does not reset conflict upload state

**File:** `Source\PlayFabGameSave\Source\SyncManager\FolderSyncManager.cpp:817-828`

`InitForDownload()` resets the lock, compare, and download steps, but does not reset `m_conflictUploadStarted`, `m_conflictUploadCompleted`, or the upload step. If a previous `AddUser` call completed the conflict upload path (setting `m_conflictUploadCompleted = true`) and then the user calls `AddUser` again, the second call will skip the conflict upload entirely because `m_conflictUploadCompleted` is still `true` from the previous run. This could cause a conflict resolution choice to be silently skipped.

```cpp
HRESULT FolderSyncManager::InitForDownload()
{
    m_lockStep.Reset();
    m_compareStep.Reset();
    m_downloadStep.Reset();
    m_isForcedDisconnectFromCloud = false;
    m_latestFinalizedManifest = nullptr;
    m_latestPendingManifest = nullptr;
    m_localFileFolderSet = nullptr;
    m_remoteFileFolderSet = nullptr;
    // Missing: m_conflictUploadStarted = false;
    // Missing: m_conflictUploadCompleted = false;
    // Missing: m_uploadStep.Reset();
    return S_OK;
}
```

**Fix:** Add `m_conflictUploadStarted = false; m_conflictUploadCompleted = false; m_uploadStep.Reset();` to `InitForDownload()`.

---

## Minor Issues

### MI-1: FolderSyncManager::m_syncProgress is read and used outside m_progressMutex in DoWorkFolderUpload

**File:** `Source\PlayFabGameSave\Source\SyncManager\FolderSyncManager.cpp:241`

`m_syncProgress` is passed by reference to `UploadStep::Upload()`, which stores it and may read its fields. However, the `m_progressMutex` that protects `m_syncProgress` (see `SetSyncStateProgress` at line 881-887) is not held during the `Upload()` call. This means there's a potential data race between the Upload step reading progress fields and the `FolderSyncManagerProgressCallback` writing them from a different context.

```cpp
HRESULT hrUp = m_uploadStep.Upload(
    runContext, task,
    m_latestPendingManifest, m_localFileFolderSet, m_remoteFileFolderSet,
    m_saveFolder, PFGameSaveFilesUploadOption::KeepDeviceActive,
    m_uiManager, m_syncProgress, folderSyncMutex,  // raw reference to mutex-protected struct
    FolderSyncManagerProgressCallback, this, ...);
```

**Fix:** The `syncProgress` parameter passed to `Upload()` appears to only be used for the `UNREFERENCED_PARAMETER` macro, but this reference propagation pattern is fragile. Either remove the parameter or ensure accesses are guarded by the progress mutex.

---

### MI-2: ExtendedManifest.cpp line 170 has a Result payload extracted but return value has already been consumed

**File:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp:170`

In the `GetExtendedManifest` finally callback, `result.ExtractPayload()` is called but the result is bound to a `const Vector<char>&` reference. `ExtractPayload()` returns by value (move), so the reference binds to a temporary that would be destroyed at the end of the expression. However, because the reference extends the lifetime of the temporary, this is actually safe in C++. This is fragile though — if anyone changes `ExtractPayload()` semantics or tries to use `result` again after extraction, they'll get empty data without any indication.

```cpp
const Vector<char>& manifestBytes = result.ExtractPayload();
```

**Fix:** Use `Vector<char> manifestBytes = result.ExtractPayload();` (or `auto manifestBytes = ...`) to make ownership explicit and avoid reliance on temporary lifetime extension.

---

### MI-3: GameSaveUiCallbackInfo struct members are not initialized

**File:** `Source\PlayFabGameSave\Source\Common\GameSaveUICallbackInfo.h:9-29`

`GameSaveUiCallbackInfo` is an aggregate with pointer and void* members that have no default initializers. The `static` instance returned by `GetGameSaveUiCallbackInfo()` is zero-initialized because it's function-local static with `{}`, which is fine. However, in `OnTerminated()` (GameSaveGlobalState.cpp:215), the info is reset via `GetGameSaveUiCallbackInfo() = GameSaveUiCallbackInfo{}` — this value-initializes all members to zero/nullptr, which is correct. The concern is that if someone ever creates a local `GameSaveUiCallbackInfo` without `{}`, all pointer members would be indeterminate, leading to potential crashes when checked against `nullptr`.

```cpp
struct GameSaveUiCallbackInfo
{
    PFGameSaveFilesUiProgressCallback* progressCallback;  // no initializer
    void* progressContext;                                  // no initializer
    // ... more uninitialized pointer members
};
```

**Fix:** Add default member initializers: `PFGameSaveFilesUiProgressCallback* progressCallback{nullptr};` etc., so the struct is safe regardless of initialization syntax.

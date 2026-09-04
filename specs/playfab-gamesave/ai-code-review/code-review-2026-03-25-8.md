# PlayFabGameSave Code Review — 2026-03-25

**Reviewer:** Claude Opus 4.6
**Scope:** `Source\PlayFabGameSave\` (excluding Generated/)

## Summary

| Severity | Count |
|----------|-------|
| Critical | 2 |
| Important | 3 |
| Minor | 2 |

## Critical Issues

### CR-1: Race condition in UICallbackManager — stale `ISchedulableTask*` can be invoked after provider destruction

**File:** `Source\PlayFabGameSave\Source\Common\UICallbackManager.cpp:17-24` / `UICallbackManager.h:51`

`UICallbackManager::SetAction` atomically exchanges `m_activeTask` to nullptr and then calls `ScheduleNow()` on the retrieved pointer. However, the `ISchedulableTask` (the async provider) can be destroyed on another thread between when `ShowSyncFailedUI`/`ShowConflictUI` stored the pointer and when the game calls `SetAction`. The `m_activeTask` is a raw `std::atomic<ISchedulableTask*>` — there is no prevent-destruction mechanism. If the `XAsyncBlock` is cancelled and the provider is destroyed while the UI callback is pending, `SetAction` invokes a dangling pointer, causing a use-after-free crash.

```cpp
HRESULT UICallbackManager::SetAction(UIAction action)
{
    if (m_shutdown.load()) { ... }

    ISchedulableTask* activeTask = m_activeTask.exchange(nullptr);
    if (activeTask)
    {
        m_action.store(action);
        activeTask->ScheduleNow(); // <-- may be dangling if provider was destroyed
        return S_OK;
    }
    ...
}
```

**Fix:** Store a `std::weak_ptr` to the provider (or a shared cancellation token) instead of a raw pointer. Before calling `ScheduleNow()`, verify the provider is still alive by locking the weak_ptr. Alternatively, the provider destructors should clear `m_activeTask` atomically before destroying themselves (the destructors currently don't do this — only `CancelPendingUIWait` does, but that's only called during global shutdown, not individual cancellation).

---

### CR-2: `internal_rand()` and `internal_seed` are not thread-safe — concurrent GUID generation produces duplicate GUIDs

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:19-28`

`internal_seed` is a non-atomic `static uint64_t` modified by `internal_rand()`, which is called by `CreateGUID()`. `CreateGUID()` is called from `MergeLocalFolders` (during download/upload), `LockStep::CreateInitManifestRequest` (during lock acquisition), and `UploadStep::CompressFiles` (during upload). These can run on different XAsync work threads for different users. Two threads calling `internal_rand()` simultaneously constitutes a data race on `internal_seed` — undefined behavior per C++14/17. Beyond UB, the shared-state LCG means two concurrent calls can produce identical sequences, generating duplicate GUIDs that would cause fileId collisions in the extended manifest.

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

**Fix:** Make `internal_seed` a `thread_local` variable (or `std::atomic<uint64_t>` with fetch-add semantics), and seed it with a per-thread value (e.g., `std::time(nullptr) ^ std::hash<std::thread::id>{}(std::this_thread::get_id())`). Using `thread_local` is the simplest fix that eliminates both the data race and the duplicate-sequence problem.

---

## Important Issues

### II-1: `DownloadAsyncProvider::DoWork` takes the `m_folderSyncMutex` recursively — the inner `DoWorkFolderDownload` also locks `folderSyncMutex`, which is the *same* mutex passed by reference

**File:** `Source\PlayFabGameSave\Source\Providers\DownloadAsyncProvider.cpp:16` and `Source\PlayFabGameSave\Source\SyncManager\FolderSyncManager.cpp:85`

`DownloadAsyncProvider::DoWork` acquires `m_folderSyncMutex` (line 16), then calls `m_folderSync->DoWorkFolderDownload(runContext, *this, m_folderSyncMutex)` passing the same mutex. `DoWorkFolderDownload` (line 85) then immediately acquires the same mutex again. This is safe only because it's a `std::recursive_mutex`, but the pattern masks a design issue: the `.Finally()` callbacks in step classes also acquire this mutex. If an async callback fires on the DoWork thread *before* DoWork returns, the recursive lock succeeds, but the callback modifies step state while DoWork is still examining it (the lock-guard scope in DoWork spans the entire function body including the return value check). This double-lock pattern obscures the actual concurrency contract and makes it easy to introduce bugs when adding new code paths.

```cpp
// DownloadAsyncProvider::DoWork
HRESULT DownloadAsyncProvider::DoWork(RunContext runContext)
{
    std::lock_guard<std::recursive_mutex> lock(m_folderSyncMutex); // 1st lock
    ...
    HRESULT hr = m_folderSync->DoWorkFolderDownload(runContext, *this, m_folderSyncMutex);
}

// FolderSyncManager::DoWorkFolderDownload
HRESULT FolderSyncManager::DoWorkFolderDownload(...)
{
    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // 2nd lock, same mutex
    ...
}
```

**Fix:** Remove the redundant lock acquisition in `DoWorkFolderDownload` / `DoWorkFolderUpload` / `DoWorkResetCloud` / `DoWorkSetSaveDescription`, since the caller already holds the lock. Document the mutex ownership contract: "caller must hold `folderSyncMutex`". This applies to all four `DoWork*` methods in `FolderSyncManager`.

---

### II-2: `UploadStep::Upload` at `UploadFile` stage captures `innerProgressContext` by `SharedPtr` in the `.Finally()` lambda but passes `innerProgressContext.get()` as a raw pointer to `InnerProgressCallback` — if the upload completes synchronously, the raw pointer can be used after the `SharedPtr` scope ends

**File:** `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:254-258` (download has the same pattern at `DownloadStep.cpp:254-258`)

In both `DownloadStep::Download` and `UploadStep::Upload`, an `InnerProgressContext` is allocated via `MakeShared`, and `innerProgressContext.get()` is passed as the `void* context` argument for the HTTP progress callback. The `SharedPtr` is captured in the `.Finally()` lambda to extend its lifetime. However, the HTTP progress callback is invoked *during* the HTTP call (potentially on yet another thread), while the `SharedPtr` is only guaranteed alive until the `.Finally()` lambda runs. If `GameSaveServiceSelector::DownloadFileFromCloud` dispatches progress callbacks to a different queue or thread and the `.Finally()` runs and destroys the SharedPtr before a late progress callback fires, the raw pointer is dangling.

```cpp
auto innerProgressContext = MakeShared<InnerProgressContext>(...);

GameSaveServiceSelector::DownloadFileFromCloud(runContext, downloadDetail, 
    remoteCompressedFile.downloadUrl, 
    InnerProgressCallback, innerProgressContext.get(),  // raw pointer
    m_totalCompressedSizeBytes, m_currentCompressedSizeBytes)
.Finally([..., innerProgressContext](Result<void> result)  // SharedPtr captured here
{
    // innerProgressContext kept alive by lambda capture
    ...
});
```

**Fix:** Ensure the progress callback mechanism holds a copy of the `SharedPtr<InnerProgressContext>` (not just a raw pointer) so that the context is guaranteed to outlive all progress callbacks. Alternatively, document and verify that progress callbacks are always dispatched *before* the completion callback (`.Finally()`).

---

### II-3: `GetLocalDeviceID` returns a value from a `const String&` reference to a temporary

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:281`

```cpp
const String& deviceId = globalState->GetLocalDeviceID();
if (!deviceId.empty())
{
    return deviceId;
}
```

`GameSaveGlobalState::GetLocalDeviceID()` (declared in `GameSaveGlobalState.h:45`) returns `String` by value (it locks a mutex and returns a copy of `m_localDeviceID`). The local `const String&` on line 281 binds to this temporary, which extends its lifetime to the end of the reference's scope. Then `return deviceId` on line 284 returns by value (the function returns `String`), copying the still-alive temporary. This is technically correct due to C++ lifetime extension rules. However, if the return type of `GetLocalDeviceID()` is ever changed to return a `const String&` (which would be natural given it reads from a member), the `const String&` on line 281 would hold a reference to the string inside the mutex-protected member, and returning it would return a dangling reference to `m_localDeviceID` after the mutex is released. This is a latent defect — fragile against reasonable refactoring.

```cpp
String GetLocalDeviceID() { 
    std::lock_guard<std::recursive_mutex> lock(m_managersMutex); 
    return m_localDeviceID; // returns by VALUE currently
}

// In Utils.cpp:
const String& deviceId = globalState->GetLocalDeviceID(); // binds to temporary
```

**Fix:** Change the local variable from `const String&` to `String` to make the copy explicit: `String deviceId = globalState->GetLocalDeviceID();`. This removes the fragile reliance on lifetime extension.

---

## Minor Issues

### MI-1: `UploadStep::SplitUploadsIntoZipBatches` can create batches containing a single file larger than `maxUncompressedSize`, silently producing zip files much larger than the 64 MB target

**File:** `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:301-347`

When all files in the batch are larger than `fileSizeLimit` (which starts at 64 MB), `PopFileDetailBelowSize` returns nullptr. The fallback on line 326 pushes `filesToUpload.back()` into its own batch regardless of size. This is the intended "oversized file" behavior, but there is no trace/warning when this happens, and no guard preventing a single file from being several hundred MB (the quota allows up to 256 MB total). A 256 MB uncompressed zip in memory during compression could cause memory pressure on constrained platforms.

```cpp
if (fileZipBatchSet.size() == 0 && filesToUpload.size() > 0)
{
    // remaining file(s) are bigger than size limit so just push it
    fileZipBatchSet.push_back(filesToUpload.back());
    filesToUpload.pop_back();
}
```

**Fix:** Add a `TRACE_WARNING` when a single file exceeds `maxUncompressedSize` to aid debugging. Consider using streaming compression for oversized files rather than in-memory buffering.

---

### MI-2: `UploadStep::Upload` in `WaitForFailedUI_InitiateUpload` (and similar) retry handler transitions to `UploadDone` on "UseOffline" path with an `assert(false)` but no error code

**File:** `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:1146-1150` (pattern repeated at lines 1161, 1176, 1192, 1207, 1222)

When the user chooses "UseOffline" during an upload failure, the handler fires `assert(false)` (debug-only) and transitions directly to `UploadDone`. In Release builds, this silently succeeds without uploading any data. The caller (`FolderSyncManager::DoWorkFolderUpload`) treats `UploadDone` as success and updates `m_latestFinalizedManifest` from the upload step's post-upload manifest — which may be empty or stale since the upload never completed. This can cause the SDK to believe the upload succeeded when it didn't.

```cpp
[this]() { 
    TRACE_WARNING("... user chose OFFLINE (unexpected during upload)");
    assert(false); 
    m_stage = UploadStage::UploadDone;  // Release builds: silent "success"
}
```

**Fix:** Instead of transitioning to `UploadDone`, transition to `UploadStepFailure` with an appropriate error code (e.g., `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`), or set a flag that the caller can inspect to distinguish successful uploads from abandoned ones.

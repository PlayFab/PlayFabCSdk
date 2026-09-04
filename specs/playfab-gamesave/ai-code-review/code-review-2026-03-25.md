# PlayFabGameSave Code Review — 2026-03-25

**Reviewers:** GPT-5.4, Claude Opus 4.6  
**Scope:** `Source\PlayFabGameSave\` (excluding Generated/)  
**Method:** Independent parallel reviews merged and deduplicated. Findings spot-checked against source.

## Summary

| Severity | Count |
|----------|-------|
| Critical | 2 |
| Important | 11 |
| Minor | 3 |
| **Total** | **16** |

### Finding index

| ID | Severity | Found by | File | Title |
|----|----------|----------|------|-------|
| CR-1 | Critical | Both | CompareStep.cpp:667-684 | ScanForConflictHelper overwrites conflictFound — missed conflicts |
| CR-2 | Critical | Opus 4.6 | CompareStep.cpp:118 | Extended manifest filename `%u` truncates 64-bit version |
| II-1 | Important | Opus 4.6 | DownloadStep.cpp:435-438 | UseOffline path missing `m_forceDisconnectFromCloud` — false SyncComplete |
| II-2 | Important | Opus 4.6 | ActiveDevicePollWorker.cpp:121 | Dereferences `GetMetadata()` optional without `has_value()` check |
| II-3 | Important | Opus 4.6 | UploadStep.cpp:791-794 | Full-set retry `Reset()` loses `m_originalActivationBaselineVersion` |
| II-4 | Important | GPT-5.4 | UICallbackManager.cpp:69-79 | Stale `m_activeTask` pointer when callback is not registered |
| II-5 | Important | GPT-5.4 | UploadStep.cpp:1344-1353 | DeletePendingManifest ignores failure — strands active-device lock |
| II-6 | Important | GPT-5.4 | DownloadStep.cpp:399-417 | Marker file write labeled "non-fatal" still aborts completed download |
| II-7 | Important | GPT-5.4 | SetSaveDescriptionProvider.cpp:42 | Timed-out description cached with dirty=false — lost on next sync |
| II-8 | Important | GPT-5.4 | LocalStateManifest.cpp:30-54 | Cloud-sync storage leaked on early-return after acquisition |
| II-9 | Important | GPT-5.4 | PFGameSaveFilesAPIProvider_GRTS.cpp:323-350 | Failed GRTS AddUser leaves stale user slot and leaked handles |
| II-10 | Important | GPT-5.4 | PFGameSaveFilesAPIProvider_GRTS.cpp:869-878 | IsConnectedToCloud always returns true in GRTS provider |
| II-11 | Important | GPT-5.4 | CompareStep.cpp:496-504 | Folder-only changes never participate in conflict detection |
| MI-1 | Minor | Opus 4.6 | CompareStep.h:90 vs .cpp:836 | `HasLocalFileChanged` parameter names swapped header vs implementation |
| MI-2 | Minor | Opus 4.6 | GameSaveGlobalState.h:31,40,75 | `m_debugManifestOffset` type mismatch (`size_t` stored, `int64_t` returned) |
| MI-3 | Minor | rest-review | PFGameSaveFilesAPI.cpp:83 vs Provider:382 | SAL annotation mismatch `_In_` vs `_In_opt_` on callback parameter |

---

## Critical Issues

### CR-1: ScanForConflictHelper overwrites conflictFound — missed conflicts

**File:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp:667-684`  
**Found by:** Both GPT-5.4 and Opus 4.6

`ScanForConflictHelper` unconditionally assigns `conflictFound` rather than OR-ing with the previous value. The caller iterates over all local files needing upload and all files needing deletion. If any file *after* a conflicting file belongs to a non-conflicting folder, `conflictFound` is overwritten to `false`, silently discarding the earlier conflict. The sync proceeds without showing conflict UI, and one side's changes are overwritten without user consent.

```cpp
// CompareStep.cpp:654-664 — caller loops
const PlayFab::Vector<const FileDetail*>& localFilesToUpload = localFileFolderSet->GetFilesToUpload();
for (const FileDetail* localFile : localFilesToUpload)
{
    ScanForConflictHelper(localFileFolderSet, localFile, topLevelFoldersNeedingDownload, conflictFound);
}
const PlayFab::Vector<const FileDetail*>& localFilesToDeleteUponUpload = localFileFolderSet->GetFilesToDeleteUponUpload();
for (const FileDetail* localFile : localFilesToDeleteUponUpload)
{
    ScanForConflictHelper(localFileFolderSet, localFile, topLevelFoldersNeedingDownload, conflictFound);
}

// CompareStep.cpp:667-684 — helper unconditionally assigns
void CompareStep::ScanForConflictHelper(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const FileDetail* localFileToUpload,
    _In_ const Set<String>& topLevelFoldersNeedingDownload, 
    _Out_ bool& conflictFound       // <-- BUG: _Out_ not _Inout_
    )
{
    const FolderDetail& folder = localFileFolderSet->GetFileFolder(localFileToUpload);
    String topLevelFolder = GetTopLevelFolder(folder.relFolderPath);

    auto iter = std::find_if(
        topLevelFoldersNeedingDownload.begin(),
        topLevelFoldersNeedingDownload.end(),
        [&topLevelFolder](const String& str)
        {
            return (str.compare(topLevelFolder.c_str()) == 0);
        });
    conflictFound = (iter != topLevelFoldersNeedingDownload.end()); // BUG: overwrites previous true
}
```

**Fix:** Use OR-assignment so that once a conflict is found it is never lost. Change the parameter annotation to `_Inout_` and the assignment to:
```cpp
conflictFound = conflictFound || (iter != topLevelFoldersNeedingDownload.end());
```
Alternatively, early-out in the caller loops once `conflictFound` is true.

---

### CR-2: Extended manifest filename `%u` truncates 64-bit version

**File:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp:118`  
**Found by:** Opus 4.6

CompareStep builds the extended manifest filename using `%u` with an explicit cast to `uint32_t`, while UploadStep (which creates the file) uses `%llu` with `uint64_t`. If the manifest version exceeds 2^32, CompareStep will look for a truncated filename (e.g., `extended-0-manifest.json` for version 4294967296) while the actual file on the server is `extended-4294967296-manifest.json`. The download details lookup fails silently, producing an empty remote file set and losing all cloud data awareness.

```cpp
// CompareStep.cpp:118 — downloads the manifest (WRONG format)
String extendedManifestName = FormatString("extended-%u-manifest.json",
    static_cast<uint32_t>(latestFinalizedManifest->Version()));

// UploadStep.cpp:490 — creates the manifest (CORRECT format)
String extendedManifestName = FormatString("extended-%llu-manifest.json",
    static_cast<uint64_t>(latestPendingManifest->Version()));
```

**Fix:** Match the upload format in CompareStep:
```cpp
String extendedManifestName = FormatString("extended-%llu-manifest.json",
    static_cast<uint64_t>(latestFinalizedManifest->Version()));
```

---

## Important Issues

### II-1: DownloadStep UseOffline path missing `m_forceDisconnectFromCloud` — false SyncComplete

**File:** `Source\PlayFabGameSave\Source\SyncManager\DownloadStep.cpp:435-438`  
**Found by:** Opus 4.6

When the user chooses "UseOffline" during a download failure, the handler transitions to `DownloadDone` without setting `m_forceDisconnectFromCloud = true`. Every other step's UseOffline handler (LockStep, CompareStep at line 325) correctly sets this flag. As a result, `FolderSyncManager::DoWorkFolderDownload` reads `IsForceDisconnectFromCloud()` as `false`, proceeds past the offline guard, reports `SyncComplete` to the game, and starts `ActiveDevicePollWorker` — while the download was never performed. The game believes sync succeeded but local files are stale/missing.

```cpp
// DownloadStep.cpp:427-439 — UseOffline lambda is MISSING the flag
case DownloadStage::WaitForFailedUI_Download:
{
    return uiCallbackManager.HandleFailedUI(task,
        [this]() { 
            m_stage = DownloadStage::Download; 
        },
        [this]() { 
            // BUG: m_forceDisconnectFromCloud is never set to true
            m_stage = DownloadStage::DownloadDone; 
        }
    );
}

// CompareStep.cpp:323-327 — correct pattern for comparison:
// [this]() { 
//     m_forceDisconnectFromCloud = true; 
//     m_stage = CompareStage::CompareDone; 
// }
```

**Fix:** Add the missing flag in the UseOffline lambda:
```cpp
[this]() { 
    m_forceDisconnectFromCloud = true;
    m_stage = DownloadStage::DownloadDone; 
}
```

---

### II-2: ActiveDevicePollWorker dereferences optional `GetMetadata()` without `has_value()` check

**File:** `Source\PlayFabGameSave\Source\SyncManager\ActiveDevicePollWorker.cpp:121`  
**Found by:** Opus 4.6

The code checks that `latestPendingManifest` is non-null but then dereferences `GetMetadata()` (which returns `std::optional`) without checking `has_value()`. If a pending manifest exists but has no metadata (e.g., created by an older client), this is undefined behavior — typically a crash via `std::bad_optional_access`. Compare with `LockStep.cpp:345` which correctly checks `GetMetadata().has_value()` first.

```cpp
// ActiveDevicePollWorker.cpp:118-122
if (latestPendingManifest != nullptr)
{
    // compare with the device id in the manifests
    const String& latestPendingDeviceId = latestPendingManifest->GetMetadata()->GetDeviceId();
    //                                                          ^^^^^^^^^^^^^^^^
    //                    BUG: GetMetadata() may return nullopt — no has_value() check
```

**Fix:** Add a `has_value()` guard:
```cpp
if (latestPendingManifest != nullptr && latestPendingManifest->GetMetadata().has_value())
{
    const String& latestPendingDeviceId = latestPendingManifest->GetMetadata()->GetDeviceId();
```

---

### II-3: UploadStep full-set retry `Reset()` loses `m_originalActivationBaselineVersion`

**File:** `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:791-794`  
**Found by:** Opus 4.6

When FinalizeManifest returns `E_PF_INVALID_PARAMS` (too many files), the code calls `Reset()` to retry with a re-compressed full file set. `Reset()` zeros `m_originalActivationBaselineVersion` (line 48). The `conflictMetadata` is saved/restored, but `originalActivationBaselineVersion` is not. The external `FolderSyncManager` code (lines 827-832) explicitly preserves this value across its own `Reset()` calls with a comment explaining why — but the internal retry path doesn't, causing `PromoteIfNeeded` to silently skip KnownGood promotion.

```cpp
// UploadStep.cpp:791-794 — internal retry path (BUG)
SetToUploadFullSet(localFileFolderSet, remoteFileFolderSet);
ConflictMetadata savedConflictMetadata = m_conflictMetadata;
Reset(); // clears m_originalActivationBaselineVersion to 0
m_conflictMetadata = savedConflictMetadata;
// BUG: m_originalActivationBaselineVersion is NOT preserved

// FolderSyncManager.cpp:827-832 — external path does it correctly
uint64_t origBaseline = m_uploadStep.GetOriginalActivationBaselineVersion();
m_uploadStep.Reset();
if (origBaseline != 0)
{
    m_uploadStep.SetOriginalActivationBaselineVersion(origBaseline);
}
```

**Fix:** Save and restore `m_originalActivationBaselineVersion` matching the FolderSyncManager pattern:
```cpp
ConflictMetadata savedConflictMetadata = m_conflictMetadata;
uint64_t savedBaseline = m_originalActivationBaselineVersion;
Reset();
m_conflictMetadata = savedConflictMetadata;
if (savedBaseline != 0)
{
    m_originalActivationBaselineVersion = savedBaseline;
}
```

---

### II-4: Stale `m_activeTask` pointer when callback is not registered

**File:** `Source\PlayFabGameSave\Source\Common\UICallbackManager.cpp:69-79`  
**Found by:** GPT-5.4

Each `Show*UI` method stores `m_activeTask = &task` before checking whether the callback is registered. If the callback pointer is null, the method returns `false` but leaves `m_activeTask` pointing at a task that may complete and be destroyed. A later `SetAction()` call can then schedule through that stale pointer. This affects all Show methods: `ShowOutOfStorageUI`, `ShowSyncFailedUI`, `ShowConflictUI`, and `ShowProgressUI`.

```cpp
bool UICallbackManager::ShowOutOfStorageUI(ISchedulableTask& task, const LocalUser& localUser, uint64_t requiredBytes)
{
    m_activeTask.store(&task);  // stored unconditionally

    auto& uiInfo = GetGameSaveUiCallbackInfo();
    if (uiInfo.outOfStorageCallback)
    {
        uiInfo.outOfStorageCallback(localUser.Handle(), requiredBytes, uiInfo.outOfStorageContext);
        return true;
    }
    return false;  // BUG: m_activeTask still points at &task
}
```

**Fix:** Only store `m_activeTask` after confirming the callback exists, or clear it on the `false` path:
```cpp
if (uiInfo.outOfStorageCallback)
{
    m_activeTask.store(&task);
    uiInfo.outOfStorageCallback(localUser.Handle(), requiredBytes, uiInfo.outOfStorageContext);
    return true;
}
return false;
```

---

### II-5: DeletePendingManifest ignores failure — strands active-device lock

**File:** `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:1344-1353`  
**Found by:** GPT-5.4

`DeletePendingManifest` always transitions to `DeleteDone` without checking `result.hr`. If the delete request fails, `DoWorkFolderUpload` treats the release-active path as complete, so the title thinks the lock was released while the pending manifest may still exist on the service. This can strand the active-device lock and break subsequent devices.

```cpp
GameSaveServiceSelector::DeleteManifest(m_entity.value(), deleteRequest, runContext)
.Finally([this, &task, &folderSyncMutex](Result<void> result)
{
    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex);
    TRACE_TASK(FormatString("DeleteManifestFinally HR:0x%0.8x", result.hr));
    m_telemetryManager->SetContextDeleteHttpInfo(result.httpResult);

    m_deleteManifestStage = DeleteManifestStage::DeleteDone;  // BUG: unconditional
    task.ScheduleNow();
});
```

**Fix:** Check `result.hr`. On failure, set `m_failureHR` and propagate the error instead of unconditionally completing.

---

### II-6: Marker file write labeled "non-fatal" still aborts completed download

**File:** `Source\PlayFabGameSave\Source\SyncManager\DownloadStep.cpp:399-417`  
**Found by:** GPT-5.4

After the download and local manifest update have already succeeded, the code writes a sentinel marker for wipe detection. When that write fails, the function returns the error even though the log says the failure is non-fatal. This turns a completed download into an API failure for a best-effort diagnostic artifact.

```cpp
if (!FilePAL::DoesFileExist(markerPath))
{
    Vector<char> markerData = { '1' };
    HRESULT markerHr = WriteEntireFile(markerPath, markerData);
    if (FAILED(markerHr))
    {
        TRACE_ERROR("[GAME SAVE] DownloadStep: Failed to write marker file hr=0x%08X (non-fatal)", markerHr);
        return markerHr;  // BUG: returns failure for "non-fatal" operation
    }
}
```

**Fix:** Log the warning but continue to `DownloadDone` instead of returning the error.

---

### II-7: Timed-out description cached with dirty=false — lost on next sync

**File:** `Source\PlayFabGameSave\Source\Providers\SetSaveDescriptionProvider.cpp:42`  
**Found by:** GPT-5.4

When `SetSaveDescriptionAsync` times out waiting for an in-flight `FinalizeManifest`, the comment says the description should be deferred to the next upload. But it calls `SetLastShortSaveDescription` with the default `dirty=false`, so the SDK later treats the cloud description as authoritative and can overwrite the unsynced local description during the next download/add-user flow.

```cpp
// Timed out; treat as deferred for next upload - just cache description and complete.
m_folderSync->SetLastShortSaveDescription(m_shortSaveDescription);  // BUG: dirty defaults to false
```

**Fix:** Pass `true` for the dirty flag:
```cpp
m_folderSync->SetLastShortSaveDescription(m_shortSaveDescription, true);
```

---

### II-8: Cloud-sync storage leaked on early-return paths after acquisition

**File:** `Source\PlayFabGameSave\Source\Types\LocalStateManifest.cpp:30-54`  
**Found by:** GPT-5.4

Both localstate read and write acquire metadata storage via `AcquireCloudSyncStorage`, but several `RETURN_IF_FAILED` calls execute before `ReleaseCloudSyncStorage`. On platforms where the provider must pair those calls, a path-join or directory-creation failure leaks the storage acquisition and can leave the metadata container locked.

```cpp
if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
{
    RETURN_IF_FAILED(globalState->ApiProvider().AcquireCloudSyncStorage(cloudSyncRoot));
    acquiredCloudSyncStorage = true;
}
String metadataRoot = cloudSyncRoot.empty() ? saveFolder : cloudSyncRoot;

String folderPath, filePath;
RETURN_IF_FAILED(JoinPathHelper(metadataRoot, "cloudsync", folderPath));  // leaks if fails
RETURN_IF_FAILED(JoinPathHelper(folderPath, "localstate.json", filePath)); // leaks if fails
...
if (globalState && acquiredCloudSyncStorage)
{
    globalState->ApiProvider().ReleaseCloudSyncStorage();  // only reached on success
}
```

**Fix:** Wrap the acquisition in an RAII scope guard so `ReleaseCloudSyncStorage()` always runs after a successful acquire, including on early-return error paths.

---

### II-9: Failed GRTS AddUser leaves stale user slot and leaked handles

**File:** `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_GRTS.cpp:323-350`  
**Found by:** GPT-5.4

The GRTS path publishes the duplicated local-user handle and config handle into `gsContext->users[]` before `PFXGameSaveFilesGetFolderWithUiAsync` has succeeded. If starting the async fails, or the completion returns cancel/failure, the code completes the outer async but never clears the partially initialized slot. This leaks handles/config and can make later calls treat a failed AddUser as if the user were registered.

```cpp
RETURN_IF_FAILED(PFLocalUserDuplicateHandle(context->localUserHandle, &duplicatedHandle));
gsContext->users[indexFound].localUser = duplicatedHandle;
gsContext->users[indexFound].xUser = context->xuser;
gsContext->users[indexFound].configHandle = configHandle;
...
hr = PFXGameSaveFilesGetFolderWithUiAsync(configHandle, &context->getFolderAsyncBlock);
...
if (context->cancelRequested || hr == E_ABORT || hr == E_GS_USER_CANCELED)
{
    XAsyncComplete(asyncBlock, E_ABORT, 0);  // slot not cleaned up
    return;
}
```

**Fix:** Do not publish the slot until folder retrieval succeeds, or add cleanup on every failure/cancel path that closes the duplicated handle, frees the config, and clears the slot.

---

### II-10: GRTS IsConnectedToCloud always returns true

**File:** `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_GRTS.cpp:869-878`  
**Found by:** GPT-5.4

`IsConnectedToCloud` ignores the user and hard-codes `true`. Callers will be told cloud sync is available even when the underlying GRTS layer is disconnected. This breaks the public API contract for titles that gate UX or retry behavior on connectivity.

```cpp
HRESULT GameSaveAPIProviderGRTS::IsConnectedToCloud(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ bool* isConnectedToCloud
) noexcept
{
    UNREFERENCED_PARAMETER(localUserHandle);
    UNREFERENCED_PARAMETER(isConnectedToCloud);
    *isConnectedToCloud = true;
    return S_OK;
}
```

**Fix:** Query the actual GRTS/PFX connectivity state for the specific user, or return `false`/error when no reliable signal is available. *(Note: May be an intentional stub — confirm with team whether GRTS platform exposes connectivity state.)*

---

### II-11: Folder-only changes never participate in conflict detection

**File:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp:496-504`  
**Found by:** GPT-5.4

The conflict scan at line 654 only iterates file uploads/deletions. Folder create/delete vectors are built after the scan. A local empty-folder create/delete in atomic unit `foo` can be merged automatically even when the cloud also changed `foo`, violating the all-or-nothing conflict model. *(Note: Low practical impact if games don't use empty folders as meaningful state, but violates the documented conflict model.)*

```cpp
MarkFilesToTransferUponUpload(localFileFolderSet); 
MarkFilesToTransferUponDownload(localFileFolderSet, remoteFileFolderSet, saveFolder);
MarkFilesToDeleteUponUpload(localFileFolderSet);
MarkFilesToDeleteUponDownload(localFileFolderSet, remoteFileFolderSet);
ScanForConflicts(localFileFolderSet, remoteFileFolderSet, conflictFound);
MarkFoldersToCreateUponUpload(localFileFolderSet, remoteFileFolderSet);   // after scan
MarkFoldersToCreateUponDownload(localFileFolderSet, remoteFileFolderSet); // after scan
MarkFoldersToDeleteUponUpload(localFileFolderSet, remoteFileFolderSet);   // after scan
MarkFoldersToDeleteUponDownload(localFileFolderSet, remoteFileFolderSet); // after scan
```

**Fix:** Include folder operations in the conflict scan, or move the scan after folder operation lists are built.

---

## Minor Issues

### MI-1: `HasLocalFileChanged` parameter names swapped between header and implementation

**File:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.h:90` vs `CompareStep.cpp:836-839`  
**Found by:** Opus 4.6

The header declares parameters as `(localFileDeleted, localFileChanged)` but the implementation names them `(localFileChanged, localFileDeleted)`. Since both are `bool&`, the compiler does not catch this. Current callers pass arguments matching the implementation order, so this is not a runtime bug today — but any future caller relying on the header's names would silently swap semantics.

```cpp
// CompareStep.h:90 — header
static void HasLocalFileChanged(_In_ const FileDetail& localFile,
    _Out_ bool& localFileDeleted,   // position 2
    _Out_ bool& localFileChanged);  // position 3

// CompareStep.cpp:836-839 — implementation (names swapped)
void CompareStep::HasLocalFileChanged(
    _In_ const FileDetail& localFile, 
    _Out_ bool& localFileChanged,   // position 2 — different name!
    _Out_ bool& localFileDeleted)   // position 3 — different name!
```

**Fix:** Update the header to match the implementation's parameter order.

---

### MI-2: `m_debugManifestOffset` type mismatch (`size_t` stored, `int64_t` returned)

**File:** `Source\PlayFabGameSave\Source\Common\GameSaveGlobalState.h:31,40,75`  
**Found by:** Opus 4.6

The member is `size_t` (unsigned), the setter takes `size_t`, but the getter returns `int64_t` (signed). The value is used in unsigned `uint64_t` arithmetic in `LockStep::CreateInitManifestRequest`, where a negative `int64_t` interpretation could corrupt the manifest version.

```cpp
int64_t GetDebugManifestOffset() { return m_debugManifestOffset; } // returns int64_t
void SetDebugManifestOffset(size_t offset) { m_debugManifestOffset = offset; } // takes size_t
size_t m_debugManifestOffset{ 0 }; // stored as size_t
```

**Fix:** Use a consistent type — `uint64_t` throughout since it's used in unsigned arithmetic.

---

### MI-3: SAL annotation mismatch `_In_` vs `_In_opt_` on callback parameter

**File:** `Source\PlayFabGameSave\Source\Api\PFGameSaveFilesAPI.cpp:83` vs `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_Win32.cpp:382`  
**Found by:** rest-review agent

The public API declares `callback` as `_In_` (required), but the Win32 provider implementation marks it `_In_opt_` (optional) and stores a null callback without validation. Any caller passing nullptr bypasses API contract enforcement.

```cpp
// API Layer — callback is required
PF_API PFGameSaveFilesSetActiveDeviceChangedCallback(
    _In_opt_ XTaskQueueHandle callbackQueue,
    _In_ PFGameSaveFilesActiveDeviceChangedCallback* callback,  // _In_ = required
    _In_opt_ void* context
) noexcept

// Implementation — callback is optional
HRESULT GameSaveAPIProviderWin32::SetActiveDeviceChangedCallback(
    _In_opt_ XTaskQueueHandle callbackQueue,
    _In_opt_ PFGameSaveFilesActiveDeviceChangedCallback* callback,  // _In_opt_ = optional
    _In_opt_ void* context
) noexcept
```

**Fix:** Either add a null check in the implementation returning `E_INVALIDARG`, or change the API annotation to `_In_opt_` if passing nullptr is the intended way to clear the callback.

---

## Raw review data

Individual model review outputs are preserved at:
- `specs\playfab-gamesave\ai-code-review\data\gpt54-review-2026-03-25.md`
- `specs\playfab-gamesave\ai-code-review\data\opus46-review-2026-03-25.md`

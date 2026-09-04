# PlayFabGameSave Code Review — 2026-03-25

**Reviewer:** GPT-5.4
**Scope:** `Source\PlayFabGameSave\Source\` (excluding Generated/)

## Summary

| Severity | Count |
|----------|-------|
| Critical | 1 |
| Important | 4 |
| Minor | 0 |

## Critical Issues

### CR-1: Save-root junctions are followed during scan, upload, download, and delete

**File:** `Source\PlayFabGameSave\Source\Types\LocalStateManifest.cpp:220-247`; `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:231-238`; `Source\PlayFabGameSave\Source\SyncManager\DownloadStep.cpp:71-76,502-543`

The SDK never checks whether a directory under the save root is a reparse point / symlink / junction before recursing into it or writing back through it. If a title's save folder contains a junction like `slot1 -> C:\Users\Public\Documents` (or a loop back to an ancestor), `MergeLocalFolders()` will enumerate and ingest files outside the save root, and later upload/download/delete code will read from or write to the junction target instead of staying inside the save container. That can exfiltrate arbitrary local files to the cloud, overwrite/delete files outside the save root, or recurse forever on a cycle until the process runs out of stack/work.

```cpp
Result<Vector<String>> subfoldersResult = FilePAL::EnumDirectories(fullFolderPath);
RETURN_IF_FAILED(subfoldersResult.hr);
Vector<String> subfolders = subfoldersResult.ExtractPayload();
for (const String& subfolder : subfolders)
{
    String fullSubfolderPath;
    RETURN_IF_FAILED(JoinPathHelper(fullFolderPath, subfolder, fullSubfolderPath));
    RETURN_IF_FAILED(MergeLocalFolders(rootPath, subfolder, fullSubfolderPath));
}
```

```cpp
String relFilePath = localFileFolderSet->GetRelFilePath(fileToUpload);
RETURN_IF_FAILED(JoinPathHelper(saveFolder, relFilePath, afd.fullPath));
...
RETURN_IF_FAILED(JoinPathHelper(saveFolder, relPath, fullFilePath));
HRESULT deleteHr = FilePAL::DeleteLocalFile(fullFilePath);
```

**Fix:** Reject or skip reparse points while enumerating the save tree, and canonicalize every on-disk path before file I/O to verify it still stays under the configured save root. Treat cycles and escapes as hard errors rather than following them.

---

## Important Issues

### II-1: GUID generation has a process-wide data race and can emit duplicate IDs

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:19-27,48-63`; `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:150-157`; `Source\PlayFabGameSave\Source\Types\LocalStateManifest.cpp:200-214`

`CreateGUID()` is backed by a global mutable `internal_seed` with no locking or atomic operations. The SDK supports multiple users/managers and can run uploads on different work threads; two concurrent operations that both call `CreateGUID()` race on `internal_seed`, which is undefined behavior in C++. In practice this can produce duplicate or corrupted folder/file IDs, and those IDs are then used for compressed bundle names, manifest entries, and local folder/file identity.

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

**Fix:** Replace this with a thread-safe GUID source (`CoCreateGuid`, `UuidCreate`, `std::random_device` + properly synchronized generator, etc.). Do not share mutable RNG state across threads without synchronization.

---

### II-2: GRTS null-user lookup aliases the first empty slot and makes `IsConnectedToCloud` succeed on `nullptr`

**File:** `Source\PlayFabGameSave\Source\Api\PFGameSaveFilesAPI.cpp:179-186`; `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_GRTS.cpp:89-100,890-910`

The public API forwards `localUserHandle` straight to the provider, and the GRTS provider never validates it. `PFXPALGetStateFromLocalUser(nullptr)` compares the null handle against each slot's `localUser`; because empty slots also contain `nullptr`, `PFLocalUserHandleCompare(nullptr, nullptr)` returns 0 and the helper returns the first unused slot as if it were a real user. On a fresh GRTS session, `PFGameSaveFilesIsConnectedToCloud(nullptr, &connected)` therefore returns `S_OK` and sets `connected=true` instead of failing with `E_INVALIDARG`.

```cpp
for (ULONG i = 0; i < PF_GDK_MAX_USERS; ++i)
{
    if (PFLocalUserHandleCompare(localUserHandle, gsContext->users[i].localUser) == 0)
    {
        return &gsContext->users[i];
    }
}
...
PFXPALGameSaveUserState* state = PFXPALGetStateFromLocalUser(localUserHandle);
...
*isConnectedToCloud = true;
```

**Fix:** Reject null `PFLocalUserHandle` at the API/provider boundary, and make the slot lookup ignore empty slots unless the caller is looking up a real tracked handle.

---

### II-3: Re-adding the same GRTS user leaks handles/config objects and overwrites live slot state

**File:** `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_GRTS.cpp:291-335,631-720`

Unlike the Win32 provider, the GRTS `AddUserWithUiAsync` path never rejects a second add for an already-added user. When the same `PFLocalUserHandle` is added again, slot selection hits the existing entry, logs "reusing existing slot", then overwrites `localUser`, `xUser`, and `configHandle` without closing the old ones first. If the later startup path fails, `cleanupSlot()` only closes `localUser`; it still leaks the duplicated `XUserHandle` and `PFXGameSaveConfigHandle` from the failed attempt.

```cpp
if (PFLocalUserHandleCompare(context->localUserHandle, gsContext->users[i].localUser) == 0)
{
    indexFound = i;
    break;
}
...
PFLocalUserDuplicateHandle(context->localUserHandle, &duplicatedHandle);
gsContext->users[indexFound].localUser = duplicatedHandle;
gsContext->users[indexFound].xUser = context->xuser;
gsContext->users[indexFound].configHandle = configHandle;

auto cleanupSlot = [&]()
{
    PFLocalUserCloseHandle(gsContext->users[indexFound].localUser);
    gsContext->users[indexFound].localUser = nullptr;
    gsContext->users[indexFound].xUser = nullptr;
    gsContext->users[indexFound].configHandle = nullptr;
};
```

**Fix:** Reject duplicate add-user calls with `E_PF_GAMESAVE_USER_ALREADY_ADDED`, or explicitly close/free the existing slot contents before replacing them. `cleanupSlot()` must also release the duplicated `XUserHandle` and `PFXGameSaveConfigHandle` on every failure path.

---

### II-4: Win32 validates a canonicalized save path but stores the raw unexpanded string for actual I/O

**File:** `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_Win32.cpp:258-304,343-355`; `Source\PlayFabGameSave\Source\SyncManager\FolderSyncManager.cpp:39-47`

`IsDisallowedSaveRoot()` expands environment variables and canonicalizes the caller's `saveFolder`, but `Initialize()` saves the original `args->saveFolder` string into global state instead of the canonicalized result. That means validation and actual file I/O can target different locations. A caller can pass `%USERPROFILE%\MyGameSave` or `C:MyGameSave`; validation treats it as an absolute canonical path, but `FolderSyncManager` later uses the raw string for every `JoinPathHelper`/file operation, so the SDK writes into a literal `%USERPROFILE%...` path or a drive-relative `C:...` path instead of the vetted directory.

```cpp
wchar_t expandedW[MAX_PATH];
DWORD ret = ExpandEnvironmentStringsW(inputW.c_str(), expandedW, MAX_PATH);
...
if (FAILED(MyPathCchCanonicalize(canonW, ARRAYSIZE(canonW), expandedW)))
{
    return true;
}
...
globalState->SetInitArgsSaveRootFolder(args->saveFolder);
```

**Fix:** Return the canonicalized/expanded path from validation and persist that normalized path in global state. The string you validate must be the exact string later used for file-system operations.

---

## Minor Issues

No issues found.


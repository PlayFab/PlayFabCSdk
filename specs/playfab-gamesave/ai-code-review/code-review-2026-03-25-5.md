# PlayFabGameSave Code Review — 2026-03-25

**Reviewer:** GPT-5.4
**Scope:** `Source\PlayFabGameSave\` (excluding Generated/)

## Summary

| Severity | Count |
|----------|-------|
| Critical | 1 |
| Important | 3 |
| Minor | 0 |

## Critical Issues
### CR-1: Async download callback captures a loop-local reference and writes through it after the stack frame is gone
**File:** `Source\PlayFabGameSave\Source\SyncManager\DownloadStep.cpp:229-290`
Each download schedules an async `.Finally(...)` callback that captures `&remoteCompressedFile`, but `remoteCompressedFile` is only a loop-local reference variable. The method returns `S_OK` immediately after starting the transfer, so the callback often runs after that stack frame has unwound or after a later `Download()` invocation has reused the same stack slot for a different file. When that happens, the callback updates progress counters from the wrong `CompressedFile` and can set `hasDownloadedLocally` on the wrong entry, causing skipped downloads or undefined behavior.

```cpp
for (size_t iRemoteFile = 0; iRemoteFile < remoteFileIndexToDownload.size(); iRemoteFile++)
{
    size_t index = remoteFileIndexToDownload[iRemoteFile];
    const CompressedFile& remoteCompressedFile = compressedFiles[index];
    ...
    GameSaveServiceSelector::DownloadFileFromCloud(...)
    .Finally([this, &task, &remoteCompressedFile, filePath, saveFolder, remoteFileFolderSet, &uiCallbackManager, &folderSyncMutex, innerProgressContext](Result<void> result)
    {
        ...
        m_currentUncompressedSizeBytes += remoteCompressedFile.uncompressedSizeBytes;
        m_currentCompressedSizeBytes += remoteCompressedFile.compressedSizeBytes;
        ...
        remoteCompressedFile.hasDownloadedLocally = true;
        task.ScheduleNow();
    });
    ...
}
```
**Fix:** Capture the stable index (or a pointer/reference to the vector element itself) by value, then re-fetch the element inside the callback before mutating it.

---

## Important Issues
### II-1: GUID generation has a data race and can emit duplicate IDs under concurrent use
**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:19-27`
`CreateGUID()` ultimately depends on a process-global `internal_seed` that is read and written with no synchronization. If two work threads call into GUID creation at the same time—for example, two simultaneous uploads building file IDs or a telemetry reset racing another operation—they execute undefined behavior and can initialize the seed from the same second-level timestamp. That can produce duplicate GUID strings, which then get reused as file IDs, folder IDs, or session IDs.

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
**Fix:** Protect the seed with a mutex/atomic state, or replace this generator with a thread-safe UUID source.

---
### II-2: Folder-only local changes are excluded from conflict detection, violating the atomic conflict model
**File:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp:500-509,642-672`
The compare pass explicitly computes folder creates/deletes, but `ScanForConflicts()` only checks local file uploads and file deletions against `topLevelFoldersNeedingDownload`. If the local device deletes or creates an empty folder inside a top-level save slot while another device changes files in that same slot, no conflict UI is raised even though the SDK's conflict model says the whole root-level folder is atomic. The operation then silently mixes local folder mutations with remote file changes instead of forcing the user to choose TakeLocal vs. TakeRemote.

```cpp
MarkFoldersToCreateUponUpload(localFileFolderSet, remoteFileFolderSet);
MarkFoldersToCreateUponDownload(localFileFolderSet, remoteFileFolderSet);
MarkFoldersToDeleteUponUpload(localFileFolderSet, remoteFileFolderSet);
MarkFoldersToDeleteUponDownload(localFileFolderSet, remoteFileFolderSet);
// TODO: ScanForConflicts currently only checks file uploads/deletions against
// topLevelFoldersNeedingDownload. Folder-only changes (empty folder create/delete)
// in an atomic unit that also has remote changes will not trigger conflict UI.
ScanForConflicts(localFileFolderSet, remoteFileFolderSet, conflictFound);
...
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
```
**Fix:** Include `FoldersToCreateUponUpload` and `FoldersToDeleteUponUpload` in the conflict scan using the same top-level-folder comparison used for files.

---
### II-3: Malformed `CompressionType::None` manifests can complete successfully without restoring any file
**File:** `Source\PlayFabGameSave\Source\Types\ExtendedManifest.cpp:100-148; Source\PlayFabGameSave\Source\SyncManager\DownloadStep.cpp:64-82`
The parser always records the compressed-file entry, but it only creates extracted `FileDetail` records when `Extract` is a non-empty array. A malformed manifest can therefore describe a `CompressionType::None` blob with an empty or missing `Extract` list. In release builds, `UncompressFile()` only checks `assert(numExtractedFiles == 1)`, so the download path reports success even though nothing was moved from temp storage into the save folder.

```cpp
f.archiveContext = MakeShared<ArchiveContext>();
size_t compressionFileIndex = AddCompressedFile(std::move(f));

JsonValue extractedFilesJson;
JsonUtils::ObjectGetMember(fileJson, "Extract", extractedFilesJson);

if (extractedFilesJson.is_array() && extractedFilesJson.size() > 0)
{
    for (auto& extractedFileJson : extractedFilesJson.get<Vector<JsonValue>>())
    {
        FileDetail e{};
        ...
        e.compressedFileIndex = compressionFileIndex;
        ...
        if (!e.skipFile)
        {
            if (AddFileDetail(std::move(e)) == SIZE_MAX)
            {
                TRACE_WARNING("[GAME SAVE] ExtendedManifest: AddFileDetail failed for file in compressed bundle, skipping");
            }
        }
    }
}
...
const Vector<FileDetail>& files = remoteFileFolderSet->GetFiles();
int numExtractedFiles = 0;
for (const FileDetail& extractedFile : files)
{
    if (extractedFile.compressedFileIndex == remoteFile.compressedFileIndex)
    {
        ...
        RETURN_IF_FAILED(FilePAL::MoveLocalFile(fullCompressedFilePath, fullExtractedFilePath));
        ...
        numExtractedFiles++;
    }
}
assert(numExtractedFiles == 1); // should be 1 extracted file in CompressionType::None file
```
Trigger this by syncing against a remote extended manifest whose `Files[]` entry declares `Compression: "none"` but omits the extracted file metadata. The SDK downloads the payload, returns success, leaves the blob in temp storage, and finishes with the save folder still missing the file.

**Fix:** Reject malformed entries during `InitWithExtendedManifest()` (for `CompressionType::None`, require exactly one extracted file) and replace the `assert` in `UncompressFile()` with a release-build runtime check that fails the sync when the manifest shape is inconsistent.

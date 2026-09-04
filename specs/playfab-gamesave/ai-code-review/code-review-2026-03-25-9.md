# PlayFabGameSave Code Review — 2026-03-25

**Reviewer:** GPT-5.4
**Scope:** `Source\PlayFabGameSave\` (excluding Generated/)

## Summary

| Severity | Count |
|----------|-------|
| Critical | 2 |
| Important | 1 |
| Minor | 0 |

## Critical Issues
### CR-1: File links inside the save root let uploads exfiltrate arbitrary local files
**File:** `Source\PlayFabGameSave\Source\Types\LocalStateManifest.cpp:249-315`; `Source\PlayFabGameSave\Source\SyncManager\UploadStep.cpp:231-239`
`MergeLocalFiles` enumerates every file under the save root and records it for upload without checking whether the entry is a file symlink / other reparse point / hard link. `UploadStep::AddCompressedFile` then joins that relative path back onto `saveFolder` and hands it to the archive layer, which reads the target file contents. A title (or any local code able to write into the save root) can create `saveRoot\slot1\save.dat` as a link to some unrelated file, then call `PFGameSaveFilesUploadWithUiAsync`; the SDK uploads the linked target instead of a file genuinely contained in the save container.
```cpp
Result<Vector<String>> localFilesResult = FilePAL::EnumFiles(fullFolderPath);
...
String localFullFilePath;
RETURN_IF_FAILED(JoinPathHelper(fullFolderPath, localFile, localFullFilePath));
...
fd.fileName = localFile;
...
if (AddFileDetail(std::move(fd)) == SIZE_MAX)
{
    TRACE_WARNING(...);
}
```
```cpp
String relFilePath = localFileFolderSet->GetRelFilePath(fileToUpload);
RETURN_IF_FAILED(JoinPathHelper(saveFolder, relFilePath, afd.fullPath));
u.archiveContext->AddFile(relFilePath, std::move(afd));
```
**Fix:** Reject linked files during enumeration/upload just like linked directories are already rejected. On Windows, check `FILE_ATTRIBUTE_REPARSE_POINT` (and, if possible, resolve and verify the final path stays under the canonical save root) before adding a file to `FileFolderSet` or archiving it.

### CR-2: AddUser deletes local data before the remote download succeeds, so a failed/cancelled sync can permanently lose save data
**File:** `Source\PlayFabGameSave\Source\SyncManager\FolderSyncManager.cpp:287-357`; `Source\PlayFabGameSave\Source\SyncManager\DownloadStep.cpp:217-275,331-340`
The AddUser download path performs destructive local deletes before any remote payload has been downloaded or extracted. If the same activation later hits a network error, unzip failure, or user cancellation, the operation returns failure but the deleted local files/folders are already gone. A concrete trigger is: remote state deleted `slot1\old.bin` and added/changed another file in `slot1`; local still has `old.bin`; AddUser reaches `DeleteFiles`, removes `old.bin`, then the subsequent download is cancelled or fails. The async completes with an error, but the player has already lost the local copy.
```cpp
if (m_localFileFolderSet->GetFilesToDeleteUponDownload().size() > 0)
{
    HRESULT hr = m_downloadStep.DeleteFiles(m_saveFolder, m_localFileFolderSet);
    ...
}
...
HRESULT hr = m_downloadStep.Download(...);
if (FAILED(hr))
{
    ...
    return hr;
}
```
```cpp
if (uiCallbackManager.IsProgressCancelRequested())
{
    ...
    return E_PF_GAMESAVE_USER_CANCELLED;
}
...
if (GetForceSyncFailedError() || FAILED(result.hr))
{
    ...
    m_stage = DownloadStage::WaitForFailedUI_Download;
}
```
**Fix:** Make download application transactional: stage remote files first, and only delete/replace local content after every required remote blob has been fetched and validated. At minimum, defer `DeleteFiles` / `DeleteFolders` until after successful download+extract, or preserve a rollback copy so failure paths can restore the pre-sync state.

## Important Issues
### II-1: Folder-only local changes bypass the atomic conflict model and can silently merge divergent saves
**File:** `Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp:510-515,648-679,744-847`
The code tracks folder creates/deletes separately, but `ScanForConflicts` only looks at file uploads and file deletions. That means a root-level atomic unit can have a local folder-only change and a remote change at the same time without ever showing the conflict UI. Example: locally create an empty folder under `slotA`, while another device changes files under `slotA`. `MarkFoldersToCreateUponUpload` queues the local folder create, `changedRemoteFolderIndexes` marks `slotA` for download, but `ScanForConflicts` ignores folder operations and reports no conflict, so the SDK merges both sides even though the API contract says the top-level folder is all-or-nothing.
```cpp
// TODO: ScanForConflicts currently only checks file uploads/deletions against
// topLevelFoldersNeedingDownload. Folder-only changes (empty folder create/delete)
// in an atomic unit that also has remote changes will not trigger conflict UI.
ScanForConflicts(localFileFolderSet, remoteFileFolderSet, conflictFound);
```
```cpp
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
**Fix:** Include `GetFoldersToCreateUponUpload()` and `GetFoldersToDeleteUponUpload()` in conflict detection, using the same top-level-folder comparison as files. The sync should force conflict resolution whenever any local mutation exists in an atomic unit that also has remote mutations.

## Minor Issues
No issues found.

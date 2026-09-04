# OS-Level GameSave / Connected Storage Test Inventory

This document catalogs every test in the two OS-level test suites that exercise the Connected Storage and GameSave subsystems. Use it to understand what each test does without reading the C++ source.

**Source directories:**
- \os\xbox\base\appmodel\ConnectedStorage\windows\unittests\lib\*\ — Connected Storage library unit tests
- \os\xbox\base\appmodel\ConnectedStorage\windows\unittests\exe\*\ — Connected Storage service unit tests
- \os\xbox\xtestsrc\tests\base\appmodel\gamesave\*\ — Xbox GameSave scenario / integration tests

---

## Section Map

| Section | Test Count | Scope |
|---------|-----------|-------|
| [A. Library Unit Tests](#a-library-unit-tests) | ~140 | In-proc unit tests for Connected Storage internals |
| [B. Service Unit Tests](#b-service-unit-tests) | 3 | Out-of-proc tests against the Connected Storage Service |
| [C. Scenario / Integration Tests](#c-scenario--integration-tests) | 88 | End-to-end Xbox console tests (ETW, lifecycle, PlayFab) |

---

## A. Library Unit Tests

Source: os\xbox\base\appmodel\ConnectedStorage\windows\unittests\lib\

These are in-process unit tests for the Connected Storage library. They use mocked web services, file systems, and UI providers to test individual components in isolation. Framework: TAEF (WEX).

---

### A1. ActivatingContainer.UnitTests.cpp

Tests container activation — downloading cloud data and handling conflicts during container bring-up.

| Test | Description |
|------|-------------|
| BasicDownload\Sync | Container successfully downloads and syncs data from cloud when activated. |
| BasicDownload\SyncFail\Retry | Download fails, retry callback fires, operation succeeds on retry. |
| BasicDownload\SyncFail\Retry\Atoms | Retry behavior for atom (blob data) downloads when initial download fails. |
| BasicDownload\SyncFail\Cancel | Canceling a failed download returns sync failure error and preserves local state. |
| BasicDownload\SyncFail\Cancel\Atoms | Cancel behavior for failed atom downloads with proper error handling. |
| Conflict\Cancel | Canceling during conflict resolution returns user cancellation error. |
| Conflict\TakeRemote | Accepting remote version during conflict downloads remote data. |
| Conflict\TakeLocal | Choosing local version during conflict skips cloud download and keeps local data. |

---

### A2. ActivatingContext.UnitTests.cpp

Tests context activation lifecycle — lock acquisition, abort handling, and contention.

| Test | Description |
|------|-------------|
| Abort | Basic abort handling during context activation. |
| Abort\On\AquireLock\403 | 403 error during lock acquisition properly aborts activation. |
| AbortDueToPackageStateChangeDuringFirstLockAcquire | Abort if package state changes while acquiring lock. |
| CustomQuota | Activation with custom storage quota settings. |
| NoLockContention | Normal activation flow without lock conflicts. |
| TrackContainersChanged | Container changes are properly tracked during activation. |
| LockContention | Lock contention scenarios handled during context activation. |

---

### A3. Allocator.UnitTests.cpp

Tests the memory allocator used for data buffering.

| Test | Description |
|------|-------------|
| AllocatorFailures | Allocator error handling including size limit overruns and callback exceptions. |
| InOrderAllocationTest | Allocations are satisfied in queue order when memory is freed. |
| AfterAllocationCallbackTest | After-allocation callbacks execute correctly when pending allocations are fulfilled. |

---

### A4. AtomManager.UnitTests.cpp

Tests atom (data blob) lifecycle — creation, reference counting, cleanup.

| Test | Description |
|------|-------------|
| DeletesIncompleteAtoms\OnStartup | Partially written atom files (.tmp) are cleaned up on manager init. |
| DoesNotDeleteGoodAtoms\OnStartup | Completed atom files are preserved during startup. |
| When\DeleteUnreferenced\WithNoReferencedAtoms\AllAtomsAreDeleted | Unused atoms are cleaned up when no longer referenced. |
| When\AtomsReferenced\Then\DeleteUnreferenced\DoesNotDeleteIt | Referenced atoms are preserved during cleanup. |
| When\GetExistingAtom\ForUnknownAtom\ItThrows | Exception for accessing non-existent atoms. |
| When\AtomDestructed\ForUnreferencedAtom\ItFails | Proper error when destroying unreferenced atoms. |
| When\AtomDestructed\AtomIsDeletedWhenNoLongerReferenced | Reference counting and deletion of atoms. |
| When\OpenForReadCalled\ItReturnsValidFileObject | Atom files can be opened and read correctly. |
| When\OpenForReadCalled\ForUnknownAtom\ItFails | Error handling when opening non-existent atoms. |
| When\OpenForReadCalled\ForUnreferenced\ItFails | Unreferenced atoms cannot be opened. |
| When\WriteNewAtomCalled\ItCreatesReferencedAtom | Successful atom creation and reference tracking. |
| When\WriteNewAtomCalled\ForAlreadyKnownAtom\ItFails | Duplicate atom creation is rejected. |
| When\DirectoryDoesNotExist\ConstructionSucceeds | Atom manager construction with non-existent directories. |

---

### A5. BlobRecords.UnitTests.cpp

Tests blob record persistence, JSON serialization, and merge logic.

| Test | Description |
|------|-------------|
| When\SaveAndLoadCalled\DataIsSuccessfullyLoaded | Blob record persistence and retrieval. |
| When\GetAtomsToUpload\CalledOnEmptyBlobRecords\EmptyVectorIsReturned | Empty blob record handling. |
| When\GetAtomsToUpload\CalledOnLocallyModifiedBlobRecords\AppropriateVectorIsReturned | Upload atom collection for modified blobs. |
| When\MergeUploadedCalledAgainstSelf\GetAtomsToUploadReturnsEmptySet | Merged uploads clear pending items. |
| When\GetJsonCalled\ValidJsonIsReturned | Blob records serialized to valid JSON. |
| When\ConstructFromJsonCalled\ValidBlobRecordsReturned | Blob record construction from JSON. |
| MergeDownload | Complex blob merge scenarios during downloads with various remote/local states. |

---

### A6. Container.UnitTests.cpp

Tests container CRUD — atom lifecycle, corruption recovery, blob reads, upload edge cases.

| Test | Description |
|------|-------------|
| When\AtomNoLongerReferenced\ItIsDeleted | Atom no longer referenced by any blob is deleted from file system. |
| When\ContainerUpdatedAndUploaded\UploadedAtomsAreMarkedAsUploaded | After update + upload, atoms are marked as uploaded in metadata. |
| When\ClientFailsToReadBlobInTime\NothingBadHappens | Blob stream kept open beyond lifetime is handled gracefully. |
| When\ContainerFileIsCorrupt\RecoversOk | Corrupt container file fails on read but recovers on overwrite. |
| ReadAllBlobs | All blobs in a container can be read back including multiple sizes. |
| MissingAtomFile | Handles case where an atom file referenced by container is deleted from disk. |
| UploadSucceeds\After\ServiceDeletedSomeAtoms | Upload succeeds even if the service deleted some previously-uploaded atoms. |

---

### A7. ContainerIndex.UnitTests.cpp

Tests the container index structure (name-to-metadata lookup).

| Test | Description |
|------|-------------|
| ExerciseCreateAndGet | Container index creation, retrieval, and deletion operations. |
| SizeReporting | Container size calculations in the index. |

---

### A8. ContainerSnapshot.UnitTests.cpp

Tests snapshot creation for upload and deletion.

| Test | Description |
|------|-------------|
| When\UploadCalled\AtomsAndContainerUploaded | Atoms and container metadata are uploaded together. |
| When\UploadCalledOnDeletedSnapshot\DeleteContainerCalled | Deleted containers trigger cloud deletion. |

---

### A9. Context.UnitTests.cpp

The largest lib test file — tests the context lifecycle: sync, upload, lock management, partial sync, file system faults, user sign-out.

| Test | Description |
|------|-------------|
| When\DeleteContainerCalled\LocalContainerFileIsDeleted | Deleting a container removes its local file. |
| When\ContainerCreatedAndDeleted\NothingBadHappens | Create/delete 100 times in a loop with no adverse effects. |
| CheckUploads\HasLock\Synchronized | Update and delete uploads work with lock held + synchronized. |
| CheckUploads\NoLock\Synchronized | Uploads behave correctly without lock but synchronized. |
| CheckUploads\HasLock\NotSynchronized | Uploads behave correctly with lock but not synchronized. |
| CheckUploads\NoLock\NotSynchronized | Uploads behave correctly with neither lock nor synchronization. |
| ContextInSyncAfterUpload | Context remains synchronized after container update + upload. |
| AtomsMarkedUploaded\AfterUpload | After successful upload, atoms are marked as uploaded. |
| When\QueryResultsOutliveContext\TheResultsCanStillBeUsed | Query results usable after context is destroyed. |
| When\UploadAtomsFails\UploadsAreDisabled | Upload failure disables further upload attempts. |
| When\DeletingNonExistentContainer\DontExpectDelete | Deleting non-existent container doesn't trigger web service call. |
| When\ContextCreated\PendingChangesAreUploaded | Recreated context auto-uploads pending changes from previous session. |
| When\ContextDestroyedWithPendingUploads\CompletedUploadDoesNotCauseCrash | Completing upload after context destruction doesn't crash. |
| When\ContextDestroyedWithNoContainersPendingUploadsAndNoLockHeld\ContextGoesIdleAndUploadsAreCleared | No-lock, no-pending context goes idle and clears uploads. |
| When\UpdateContainerCalled\ContextMetaDataIsUpdated | Container update refreshes AUMID and display name in metadata. |
| When\DeleteContainerCalled\ContextMetaDataIsUpdated | Container deletion updates AUMID in metadata. |
| OnNetworkConnected\ReacquireLock\ReleaseLockIfHeldByOther | Network restored + lock held by other device releases lock. |
| SuspendTests\ContextIsIdle\IdleAfterSuspend | Context transitions to idle on app suspend. |
| LockTests\OnNetworkConnected\ReacquireLock\LockNotAquiredNetworkLost | Lock re-acquire fails when held by another device. |
| TerminationTests\OnNetworkConnected\ReacquireLock\LockAcquiredNetworkLost | Lock successfully re-acquired after network restoration. |
| AutoRetryLockAcquire\WhenDisableUploads | Auto-retries lock acquisition after upload disable timeout. |
| ScheduleUploadsAgain\WhenUploadsFailAndWeRetry | Failed uploads are retried after delay and succeed. |
| UserDataSafeAfterResuming | User data persists through sign-out, suspend, sign-in, resume. |
| PartialSync\ReadAndUpdate | Read and update containers in partial sync mode with conflict resolution. |
| PartialSync\Delete | Delete containers in partial sync mode with conflict handling. |
| PartialSync\BlobQuery | Query blobs in partial sync mode with correct ordering. |
| BlobQuery\AfterUpdate | Blob queries return all updated blobs with accurate sizes. |
| BlobQuery\AfterRecreateContext | Blob queries consistent after context destroy/recreate. |
| BlobQuery\AfterDeleteContainer | Blob queries return empty after container deletion. |
| PartialSync\FailSync\EverythingIsOkay | Failed partial sync + user cancel leaves context in valid state. |
| When\FileSystemFails\MoveFile\EverythingWorksAsExpected | MoveFile failure handled gracefully, existing data preserved. |
| When\FileSystemFails\Flush\EverythingWorksAsExpected | Flush failure handled gracefully, existing data preserved. |
| When\FileSystemFails\Write\EverythingWorksAsExpected | Write failure handled gracefully, existing data preserved. |
| When\FileSystemFails\CreateFile\EverythingWorksAsExpected | CreateFile failure puts context in error state for subsequent ops. |
| When\FileSystemFails\RecreateContextAndBlobExists | After FS failure + context recreate, previously written data is still accessible. |
| When\UserSignsOut\ContextEventuallyGetsDestroyed | Sign-out transitions context to idle for proper destruction. |
| When\ContextCreated\ProviderInfoAvailable | Provider info available after context creation; AUMID populated on container create. |
| When\ContextCreated\AttemptsToRemoveContextFolder\AreBlocked | Removing context folder while active is blocked. |

---

### A10. ContextDesc.UnitTests.cpp

| Test | Description |
|------|-------------|
| ContextDescTest | Context descriptor creation, property init, XUID formatting, directory naming. |

---

### A11. DownloadContainer.UnitTests.cpp

Tests container download paths — atom fetching, partial failures, JSON parsing, old-data preservation.

| Test | Description |
|------|-------------|
| When\ContainerDownloadFails\CallbackGetsFail | Web service download failure propagates to completion callback. |
| When\ContainerDownload\IsEmpty\CallbackGetsSuccess | Empty container (no atoms) downloads successfully. |
| When\ContainerDownload\ListsBlobs\AtomsAreDownloaded | All listed blobs trigger atom downloads from service. |
| When\NewContainerFormat\AtomsAreDownloaded | New JSON container format parsed and downloaded correctly. |
| PartialDownload\Failure\ReportsCorrectBytesDownloaded | Partial download failure reports accurate byte count. |
| When\ContainerDownload\FileAlreadyExists\StillSucceeds | Download succeeds even if local atom files already exist. |
| BadJson\Fails | Malformed JSON in container metadata fails appropriately. |
| When\ContainerDownload\AtomFails\OldDataPersists | Individual atom download failure preserves old local data. |
| When\ContainerDownload\Succeeds\OldAtomsAreDeleted | Successful download cleans up unreferenced old atoms. |
| DeleteContainerWhenScheduledWithDelete | Container scheduled for deletion skips download. |

---

### A12. File.UnitTests.cpp

Tests file system operations — path parsing, directory creation, reparse points, transacted renames.

| Test | Description |
|------|-------------|
| FindNextSlash | File path parsing for backslash locations. |
| CreateParentDirectories\NoDir | Directory hierarchy creation (no parent). |
| CreateParentDirectories\OneDir | Directory hierarchy creation (one level). |
| CreateParentDirectories\TwoDirs | Directory hierarchy creation (two levels). |
| TestReparseHooks | Reparse point handling in file operations. |
| RecurseDeleteDirectory\WithDir\RemovesFiles | Recursive directory deletion with files. |
| RecurseDeleteDirectory\WithFileReparse\FailsTxn | Transaction fails with file reparse points. |
| RecurseDeleteDirectory\WithDirReparse\FailsTxn | Transaction fails with directory reparse points. |
| RenameDirectory\WithDir\MovesFiles | Directory renaming and file movement. |
| CreateDirectory\WithDir\CreatesDirs | Directory creation operations. |
| RenameFileCreateDestPath\WithDir\MovesData | File renaming with destination path creation. |

---

### A13. FileSync.UnitTests.cpp

Tests XGS (Xbox Game Save) file sync — snapshot creation, blob mapping, special characters, file change detection.

| Test | Description |
|------|-------------|
| Test\FileHash | File hashing functionality. |
| CopyToXgs\WithData\CreatesPeerFiles | XGS file creation from container data. |
| CopyToXgs\WithData\RemovesUnusedFiles | Cleanup of orphaned XGS files. |
| CopyToXgs\WithNoData\CreatesNoFiles | No-op when source is empty. |
| CopyToXgs\WithSpecialChars\CreatesXgsDataFiles | Special character handling in file names. |
| CopyToXgs\WithNestedContainers\RenamesOnlySubs | Nested container renaming. |
| SnapshotXgs\WithData\CreatesPayload | XGS snapshot creation with content. |
| SnapshotXgs\WithNoData\CreatesNoPayload | Empty snapshot creation. |
| SnapshotXgs\WithSpecialChars\RenamesPayload | Special characters in snapshots. |
| SnapshotXgs\WithFileRemove\DeletesBlobs | Blob deletion on file removal. |
| SnapshotXgs\WithFolderRemove\DeletesContainers | Container deletion on folder removal. |
| SnapshotXgs\WithMultiRemoveAndMultiAdd\DeletesContainersAndAddsNew | Complex additions and removals. |
| SnapshotXgs\WithFileUpdates\UpdatesBlobs | Blob updates from file changes. |
| SnapshotXgs\WithRecentWrite\WillFailSnap | Fails when files were recently modified. |
| SnapshotXgs\WithOpenFiles\WillFailSnap | Fails with open file handles. |
| SnapshotXgs\WithFilesChanging\WillFailSnap | Fails when files are actively changing. |
| SnapshotXgs\WithFileRename\DeletesAndReaddsBlobs | Blob recreation on file renames. |
| SnapshotXgs\WithMultiLevelFolder\CreatesSeperateContainers | Nested folders map to separate containers. |

---

### A14. LocalStorage.UnitTests.cpp

| Test | Description |
|------|-------------|
| DeleteOneDir | Selective directory deletion while preserving others. |

---

### A15. MultiFileWrite.UnitTests.cpp

Tests transacted multi-file writes — commit semantics, rollback, ordering.

| Test | Description |
|------|-------------|
| When\CreateFileCalled\AfterPreviousFileReleased\Success | Sequential file creation succeeds. |
| When\CreateFileCalled\BeforePreviousFileReleased\Success | Overlapping file creation succeeds. |
| When\CommitCalled\WhileFileOpen\CallFails | Commit fails with open files. |
| When\CreateNewCalled\After\Commit\CallFails | Creation after commit fails. |
| When\Commit\CalledMultipleTimes\CallFails | Repeat commit fails (idempotence). |
| When\CommitNotCalled\FileDoesNotReachFinalLocation | No commit = rollback, files don't persist. |
| When\CommitCalled\FileDoesReachFinalLocation | Commit persists files to final location. |
| When\DestinationFileHasDirectories\TheCommitSucceeds | Nested directory creation during commit. |

---

### A16. NtmWebService.UnitTests.cpp

Tests the Network Transfer Manager web service client — uploads, downloads, compression, lock management.

| Test | Description |
|------|-------------|
| AppendW3cDateTime | W3C datetime string formatting. |
| RegisterUnregisterTrustedDevice | Trusted device registration and unregistration. |
| AcquireAndReleaseLock | Lock acquire and release cycle. |
| UploadZeroAtoms | Upload with no atoms succeeds. |
| UploadOneSmallAtom | Upload a single small atom. |
| UploadZeroByteAtom | Upload a zero-byte atom. |
| UploadOneAtomTwice | Upload same atom twice (idempotence). |
| UploadDifferentSizeAtoms | Upload atoms of varying sizes. |
| UploadAtoms | Bulk atom upload. |
| UploadContainer | Container metadata upload. |
| UploadContainer\NoDisplayName | Container upload without display name. |
| DownloadContainer | Container metadata download. |
| DownloadAtom | Single atom download. |
| ListContainers | Container enumeration with pagination. |
| DeleteContainer | Container deletion on the service. |
| UploadCompressedAtoms | Compressed atom upload (zlib). |

---

### A17. NtmWrapper.UnitTests.cpp

Tests the NTM wrapper — download lifecycle, shutdown, timeout, cancellation.

| Test | Description |
|------|-------------|
| TestSimpleDownload | Basic HTTP download via NTM wrapper. |
| DuringShutdown\BehavesCorrectly | Graceful shutdown handling. |
| TestTimeout | Timeout behavior during transfer. |
| CompleteTransfer\Is\Called\For\Completed\Transfers | Callback fires for completed transfers. |
| CallbackCalled\When\NtmWrapper\Destroyed | Cleanup callbacks during destruction. |
| Externally\Canceled\Completes\With\Abort | External cancellation returns abort. |
| Rundown\Triggers\Abort | Rundown process triggers transfer abort. |

---

### A18. Operation.UnitTests.cpp

| Test | Description |
|------|-------------|
| When\OperationIsCanceled\ItReportsItIsCanceled | Operation cancellation flag. |
| When\OperationIsNoLongerHeld\ItDoesNotExist | Operation cleanup when no longer referenced. |

---

### A19. ParseListContainerJson.UnitTests.cpp

| Test | Description |
|------|-------------|
| ValidJson | Parsing valid container list JSON with blobs and pagination. |
| NullContinuationToken | Handling null continuation tokens in pagination. |
| ParseW3cDateTime | W3C datetime parsing with various formats and precision. |

---

### A20. ProactiveDownloader.UnitTests.cpp

| Test | Description |
|------|-------------|
| Backoff\Basics | Exponential backoff behavior for failed downloads. |
| Backoff\ShiftCap | Backoff shifting and maximum backoff cap enforcement. |

---

### A21. ResumeUploadPolicyTests.cpp

Tests the policy engine that decides when to resume uploads after system state changes.

| Test | Description |
|------|-------------|
| ResumeCalledOnBoot | Resume upload triggered on system boot. |
| ResumeCalledAfterNetworkConnectivityRestored | Resume after network recovery. |
| ResumeCalledAfterLeavingConnectedStandby | Resume after exiting standby mode. |
| ResumedCalledAfterLeavingConnectedStandbyAndNetworkConnectivityRestored | Combined standby + network recovery. |
| NetworkRestoredAndOnBootOnlyCallsOnce | Resume called only once during boot with network restoration. |
| PendingUploadLogger\AddEnumRemove | Pending upload tracking and enumeration. |

---

### A22. ResumeUploadTests.cpp

Tests the resume-on-boot upload flow — lock acquisition, abort, conflict, retry.

| Test | Description |
|------|-------------|
| EnumerateContexts\NoContexts | Enumerating with no contexts on disk completes without error. |
| EnumerateExistingContexts | All expected contexts on disk are correctly enumerated. |
| ScanExistingContexts | Scanning creates UploadingContext entries for USER contexts. |
| GoldenPath | Normal happy path: lock, upload, delete, cleanup. |
| NoNetworkConnection | Uploads skipped offline, resume on network restore. |
| AcquireLockFails | Lock acquisition failure handled gracefully. |
| AcquireLockHeldByOtherDevice | Lock held by other device still allows upload/delete to proceed. |
| AbortWhileWaitingForAcquireLock | Abort during lock acquisition wait. |
| ListContainersFails | ListContainers failure keeps lock held (prevents false no-pending assumption). |
| AbortWhileWaitingForListContainers | Abort during ListContainers wait, lock remains held. |
| AbortWhileWaitingForUploadContainer | Abort during upload wait, lock remains held. |
| UploadingContexts\WithPendingUploads\AbortedWhenLosesLock | Pending uploads aborted when lock lost to another device. |
| UploadingContexts\WithPendingUploads\RetryWhenUploadFails | Pending uploads retried automatically on failure. |
| UploadingContext\WithConflicts\DoesNotUpload | Conflict detected (container already exists) skips upload. |
| DontCrashOnCallbacks\WithUploadingContext | Callbacks during pending uploads don't crash. |

---

### A23. Uploader.UnitTests.cpp

| Test | Description |
|------|-------------|
| When\MarkedAsDirty\ContainerIsSnapshottedAndUploaded | Dirty container is snapshotted and uploaded. |

---

### A24. UserManager.UnitTests.cpp

| Test | Description |
|------|-------------|
| TestUser | User authentication, sign-in/sign-out events, and user context retrieval. |

---

### A25. XgsMigration.UnitTests.cpp

Tests migration from legacy Connected Storage format to XGS format.

| Test | Description |
|------|-------------|
| Context\WithData\Migrates | Container data migration to XGS format with verification. |
| Context\WithData\Scans | Scanning/enumeration of migrated contexts. |
| Context\WithSpecialData\Migrates | Migration with special characters and UTF-8 data. |
| Context\WithLargeData\Migrates | Large dataset migration handling. |
| Context\WithData\DiskFullErrors | Error handling during disk space exhaustion. |
| EnumerateContexts | Context enumeration after migration. |

---

### A26. XgsSnap.UnitTests.cpp

Tests XGS snapshot round-tripping — create from container, restore from file.

| Test | Description |
|------|-------------|
| Context\WithData\CreatesXgsFile | XGS snapshot creation from populated container. |
| Context\WithNoData\CreatesNoXgsFile | No snapshot for empty container. |
| Context\WithSpecialData\CreatesXgsFile | Snapshot creation with special characters. |
| XgsFile\WithValidData\CreatesContext | Context reconstruction from valid XGS file. |
| XgsFile\WithLargeValidData\CreatesContext | Large XGS file handling. |
| XgsFile\WithBadIdxData\CreatesNoContext | Error handling with corrupted XGS data. |

---

### A27. ZLibFileStream.UnitTests.cpp

| Test | Description |
|------|-------------|
| ZlibTranscodeStream\InvalidInputs\ReturnsFailureResults | Zlib error handling with invalid inputs. |
| ZlibTranscodeStream\WithVariousCompressOptions\EndsWithTheSameContents | Compression/decompression round-trip integrity. |

---

### A28. PFContext.UnitTests.cpp

The largest unit test file (~6500 lines). Tests the PlayFab context activation, upload, conflict resolution, offline handling, version management, and telemetry.

**Activation basics:**

| Test | Description |
|------|-------------|
| PFActivator\Prepare | Basic successful activation with UI interaction and cloud download. |
| PFActivator\Prepare\NoManifests | ListManifests returns 404 (no manifests); GetManifestDetails/GetChunk not called. |
| PFActivator\Prepare\NoManifests\ErrorsConnecting | InitManifest fails, UI retry/cancel flow, eventually offline. |
| PFActivator\Prepare\NoManifests\UploadChanges | Activation with empty manifests, then write + verify upload completes. |
| PFActivator\Prepare\NoManifests\UploadChanges\ErrorRetry | PushUpload 500 error on first attempt, retry succeeds, finalize completes. |
| PFActivator\Prepare\EmptyManifestList | ListManifests returns 200 with empty array (not 404); fresh context created. |
| PFActivator\Prepare\EmptyManifestList\UploadChanges | Empty manifest activation + write + upload with extended manifest verification. |

**Offline and error paths:**

| Test | Description |
|------|-------------|
| PFActivator\Prepare\Offline | Login fails, UI retries then cancels, offline activation with local data. |
| PFActivator\Prepare\Offline\503 | Server 503 triggers sync failure and offline fallback. |
| PFActivator\Prepare\Offline\500 | Server 500 with S\OK login triggers sync failure and offline fallback. |
| PFActivator\Prepare\Offline\401 | Auth 401 triggers retry/cancel and offline activation. |
| PFActivator\Prepare\Online\200 | Successful online activation with HTTP 200, no offline fallback. |
| PFActivator\Prepare\DiskFull | Disk exhaustion during download; UI prompts clear space, retry succeeds. |
| PFActivator\Prepare\CancelOnLogin | User cancels during login phase via sync progress callback; offline. |
| PFActivator\Prepare\CancelOnSync | User cancels during chunk download after login; offline fallback. |

**Conflict and version resolution:**

| Test | Description |
|------|-------------|
| PFActivator\Prepare\FileConflict | Local files conflict with remote; TakeLocal first, TakeRemote second iteration. |
| PFActivator\Prepare\VersionConflict | Two manifests (Uploading + Finalized); lock contention UI, Retry then DoNotBreakLock. |
| PFActivator\OfflineBase\ServerHigherVersion | Offline version exists, server has higher version; new cloud-backed version created. |
| PFActivator\OfflineBase\ServerHigherVersion\ConflictingLocalFile | Offline conflict triggers UI; user chooses TakeRemote. |

**Manifest state handling:**

| Test | Description |
|------|-------------|
| PFActivator\Prepare\LatestManifestInitialized\SkipVersion | Latest manifest Initialized (locked by other device); BreakLock, NextAvailableVersion used. |
| PFActivator\Prepare\LatestManifestPendingDeletion\SkipVersion | PendingDeletion manifest skipped for new init without prompts. |
| PFActivator\Prepare\OneManifestRemoteUploading\BreakLockToSkipVersion | Remote Uploading from different device triggers contention; BreakLock to skip. |
| PFActivator\Prepare\OneManifestLocalUploading\NoLocalDataSkipVersion | Local Uploading without local folder advances to NextAvailableVersion. |
| PFActivator\Prepare\OneManifestLocalUploading\LocalDataSameVersion | Local Uploading with matching folder reused without contention. |
| PFActivator\Prepare\MultiManifestBadStates\NoSyncNewInit | Multiple bad-state manifests trigger contention or fresh init. |
| PFActivator\Prepare\SyncErrors\MultipleRetry | GetChunk fails intermittently; UI retry loop handles before succeeding/canceling. |
| PFActivator\Prepare\InProcData\SkipInitVersion | In-proc data version with lock contention retry then break; new cloud version. |

**Upload:**

| Test | Description |
|------|-------------|
| PFContext\Upload | Successful upload with one data file and extended manifest; two uploads + finalize. |
| PFContext\Upload\WithThumbnail | Upload includes pfthumbnail.png (uploaded directly, not chunked); three uploads + finalize. |
| PFContext\Upload\WithThumbnail\ModifyPayload | Multi-phase upload with thumbnail, payload modifications, large file chunking; persistence across reactivation. |
| PFContext\Upload\VerifyStreamNotNull | Upload validates IStream passed to PushUpload is never null. |
| PFUploadContext\MultipleFailures | PushUpload fails first two attempts, retries succeed, finalization completes. |
| PFUploadContext\TerminalError\ClearsSession | FinalizeManifest returns terminal error 20312; session cleared, no perpetual retry. |
| PFUploadContext\UnknownFile\TriggersRepackage | FinalizeManifest returns 20306 (UnknownFile); repackage with new InitUpload and retry. |

**Version consolidation and conflict resolution internals:**

| Test | Description |
|------|-------------|
| Test\VersionConsolidation\Persistence | ActivationVersions struct usage during activation. |
| Test\TakeLocal\StoresRemoteVersion | TakeLocal stores remote version as loser; activation succeeds. |
| Test\TakeRemote\PreservesLocal | TakeRemote triggers loser upload; captures version metadata; verifies InitManifest. |
| Test\BreakLock\PreservesLocal | BreakLock uses same loser upload path as TakeRemote. |
| Test\LoserUpload\NetworkFailure\ContinuesWithWinner | Loser upload network failure triggers degraded mode; activation succeeds with winner. |
| Test\NextAvailable\RefreshAfterLoserUpload | After loser upload, ListManifests refreshes NextAvailableVersion. |
| Test\FailOperation\CleansNewVersionNotBase | Local==remote InitManifest; FailOperation redirects cleanup to new version. |
| Test\FailOperation\SameVersion\CleansNormally | InitManifest returns same version; FailOperation cleanup targets directly. |

**Same-device recovery:**

| Test | Description |
|------|-------------|
| Test\SameDevice\NoLocalData\FallsThrough | Same-device Uploading manifest without local folder falls through to cloud download. |
| Test\SameDevice\StaleInit\NoLocalData\DownloadsFinalized | Stale Initialized manifest skipped; latest Finalized downloaded. |
| Test\SameDevice\OfflineLocal\CreatesNewVersion | Offline local + same-device Uploading remote creates new cloud-backed version. |

**Offline edge cases:**

| Test | Description |
|------|-------------|
| PFActivator\Prepare\Offline\WithExistingCloudData\CopiedToOffline | Pre-existing cloud data copied to offline folder when login fails + cancel. |
| PFActivator\Prepare\Offline\StaleCloudManifest\PhantomEntry | Stale cloud manifest without folder doesn't corrupt ActivationVersions. |
| PFActivator\Prepare\Offline\DuplicateOffline\DedupPreservesLarger | Duplicate offline versions from RTC reset; dedup preserves larger version. |
| PFActivator\Prepare\Offline\StaleCloudManifests\DataSurvivesReload | Multiple stale cloud manifests; data survives 100 reload iterations. |
| PFActivator\Prepare\Offline\LastVersionStomp\CascadingDataLoss | Manifest JSON stomp in cleanup loop prevents cascading data loss across reboots. |

**Telemetry:**

| Test | Description |
|------|-------------|
| PFTelemetry\Offline\WithData\ReportsNonZeroSize | Offline activation reports non-zero totalSizeBytes via disk-size fallback (REQ-494). |
| PFTelemetry\Activation\Success\EmitsCorrectFields | Success emits syncState=Complete, resolution=NoConflictsExpected, canceled=false. |
| PFTelemetry\Activation\Offline\EmitsEvent | Offline activation emits event with launcherName and version fields. |
| PFTelemetry\ActivationFailure\EmitsCorrectHResult | Failure emits ContextActivationFailure with hresult, syncState, callingLocation. |
| PFTelemetry\ConflictResolution\TakeLocal\EmitsCorrectEnum | TakeLocal emits CR\KeepLocal (value 1). |
| PFTelemetry\ConflictResolution\TakeRemote\EmitsCorrectEnum | TakeRemote emits CR\TakeRemote (value 2). |
| PFTelemetry\ConflictResolution\Cancel\EmitsCorrectEnum | Cancel emits CR\NoResolutionChosen (value 4), canceled=true. |
| PFTelemetry\Upload\EmitsContextSync | Upload emits ContextSync with syncDownload=false, blockCount, fileCount, syncSizeBytes. |
| PFTelemetry\SyncError\EmitsOnFailure | Chunk failure emits ContextSyncError with hresult, errorSource, retryCount. |
| PFTelemetry\LockContention\BreakLock\EmitsSelectVersion | BreakLock emits CR\SelectVersion or CR\TakeRemote. |
| PFTelemetry\AllEventTypes\Capturable | All 5 event types capturable with correct field round-tripping. |

---

## B. Service Unit Tests

Source: os\xbox\base\appmodel\ConnectedStorage\windows\unittests\exe\

These are out-of-process tests that exercise the Connected Storage Service COM server. They use RPC to restart the service and verify data persistence, storage cleanup, and provider enumeration. Framework: TAEF (WEX), runs as User with System fixture.

**Setup:** Each test resets the service with mock web service enabled. RPC server registered as "ConnectedStorageUnitTests".

| Test | Description |
|------|-------------|
| When\LargeNumberOfContainersAreCreatedAndDeleted\StorageDoesNotFillUp | Creates and deletes 16 MB containers 64 times; verifies local cache doesn't grow unbounded. *(Currently Ignored)* |
| DataPersistsWhenTheServiceIsRestarted | Stores 12,345 bytes, restarts service via RPC, reads data back, verifies match. |
| SpaceEnumerator | Stores data under 4 different SCIDs, enumerates all saved game providers, verifies all 4 SCIDs appear with correct AUMID/XUID/SCID. *(Runs as System)* |

**Helper infrastructure:**
- WaitingOperationHandler — Waits for async operations; stores HRESULT.
- NaturalNumberGeneratingStream — Generates deterministic byte sequences for write tests.
- VerifyNaturalNumberStreamHandler — Reads and verifies data matches expected natural number pattern.
- WaitingQueryHandler — Handles provider enumeration results; 120s timeout.
- ResetStorageService() — RPC call to reset service with configurable test flags.

---

## C. Scenario / Integration Tests

Source: os\xbox\xtestsrc\tests\base\appmodel\gamesave\ScenarioTests.cpp (~6200 lines, 88 tests)

These are end-to-end Xbox console tests that launch real apps (ShamWow, ForteBaseGame), control the app lifecycle (suspend/resume/terminate), monitor ETW events in real time, and inject mock responses via registry. They test the full stack from game API call through the Connected Storage Service to the cloud.

**Framework:** TAEF (WEX) with ETW listener (GameSaveListener), mock injection (MockUtils), and Xbox test shell app helpers.

**Key parameters:**
- /p:UseRealService=true — Use real PlayFab service instead of mocks
- /p:EventTimeout=<ms> — ETW wait timeout (default 2500ms)
- /p:InstallApps=true — Force reinstall test apps

---

### C1. Basic Init / Quota (5 tests)

| Test | Status | Description |
|------|--------|-------------|
| InitGS\InitFS | Active | Basic GameSave + FileSync initialization. |
| QuotaGS\QuotaFS | Active | Quota validation for both providers. |
| CrashGS\CrashFS | Active | Crash handling with DMP file generation. |
| BadAPIUsageGS\BadAPIUsageFS | Ignored | Bad API usage error handling. *ShamWow crash not predictable.* |
| XLaunchNewGameGS\XLaunchNewGameFS | Ignored | Game relaunch behavior. *Intermittent relaunch issues.* |

---

### C2. Measure Processing Time (5 tests)

Performance measurement tests with platform-aware timeouts (Scarlett vs non-Scarlett).

| Test | Status | Timeout | Description |
|------|--------|---------|-------------|
| XGameSave\MPT\Short | Active | 5-10 min | Short processing time measurement. |
| XGameSave\MPT\Mid | Active | 40-60 min | Mid-range processing time. |
| XGameSave\MPT\Long | Active | 60-90 min | Long processing time. |
| XGameSave\MPT\Parallel | Active | 15-30 min | Parallel load performance. |
| XGameSave\MPT\ParallelShort | Active | 5-10 min | Short parallel load. |

---

### C3. Orphan / USB Storage (4 tests)

| Test | Status | Description |
|------|--------|-------------|
| OrphanCheck\OnNewlyCreatedData\IsPersistedAfterTerminateAndRelaunch | Active | Orphan file cleanup via DELETEORPHANS registry key; data persists across terminate/relaunch. |
| UsbStorageBackup\OnUSBKeyInsertion\StorageIsPersisted | Active | Comprehensive USB storage backup and restore cycle. |
| UsbStorage\OnUSBKeyInsertion\ConflictingFilesIgnored | Active | Conflict file handling during USB backup. |
| UsbStorage\OnMultiUSBKeyInsertion\SecondaryFilesAreUsed | Active | Secondary USB key usage. |

---

### C4. Enumerator / Deadlock (1 test)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\EnumeratorDuringKeyStates\NoDeadlockReported | Active | Enumerator calls during various service states; validates no deadlock ETW events. |

---

### C5. Crash / Power Loss Recovery (2 tests)

| Test | Status | Description |
|------|--------|-------------|
| InvokeFileSync\WhenServiceCrashesOrPowerLoss\NextStartRecoversPendingXVD | Active | Kills svchost; verifies XVD recovery on next start. |
| InvokeFileSync\LargePayload\WhenServiceCrashesOrPowerLoss\NextStartRecoversPendingXVD | Active | Same as above with large payload data. |

---

### C6. Storage Full / XVD (2 tests)

| Test | Status | Description |
|------|--------|-------------|
| InvokeFileSync\LargePayload\WhenXVDFull\OlderDataWillBeRemoved | Active | Automatic storage cleanup when XVD full; older data removed. |
| InvokeFileSync\LargePayload\WhenXCrdUserContentFull\OutOfLocalStorageIsPrompted | Active | Out-of-storage UX prompt when XCrdUserContent full. |

---

### C7. Suspend / Snap (2 tests)

| Test | Status | Description |
|------|--------|-------------|
| SuspendSave\IsPersisted | Active | Suspend with small/mid/large payloads; data survives suspend cycle. |
| InvokeFileSync\WithLargePayload\SuspendAndTerminateOnlySnapsOnce | Active | 24 iterations of SuspendOpt combinations; only one snap per cycle. |

---

### C8. File Validation / USB ZIP (3 tests)

| Test | Status | Description |
|------|--------|-------------|
| InitQuotaGS\AfterSync\ExpectedFilesArePresent | Mock Only | Compares XML payload structure after sync. *Skips if real service.* |
| InvokeUsbZip\WithLargePayload\CreatesZipBackup | Active | Creates ZIP backup on USB insertion. |
| InvokeUsbZip\WithLargePayload\RestoresZipBackup | Active | Restores ZIP backup. *Depends on previous test's output.* |

---

### C9. Storage Manager (1 test)

| Test | Status | Description |
|------|--------|-------------|
| StorageManager\LargePayload\WhenXVDFull\OlderDataWillBeRemoved | Active | Uses Settings JSON override; tests cleanup behavior when XVD full. |

---

### C10. PlayFab Core (5 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFabInit\TerminateStartsUpload | Active | Init then terminate triggers upload. |
| Invoke\PlayFabInit\XLaunch\ShouldUploadOnce | Active | XLaunch after PlayFab init; upload happens once. |
| Invoke\PlayFabInit\BadAPIUsage\ShouldFailProvider | Active | Bad API usage fails provider gracefully. |
| Invoke\PlayFabInit\BadUserUsage\ShouldFailProvider | Active | Bad user usage fails provider gracefully. |
| Invoke\PlayFabInit\GameCrash\ShouldStillUpload | Active | Upload persists after game crash. |

---

### C11. Sign-Out (3 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\XGameSave\SignOutWhileRunning\ShouldUploadChanges | Active | Sign-out triggers upload (XGameSave provider). |
| Invoke\XGameSaveFiles\SignOutWhileRunning\ShouldUploadChanges | Active | Sign-out triggers upload (FileSync provider). |
| Invoke\PlayFab\SignOutWhileRunning\ShouldUploadChanges | Active | Sign-out triggers upload (PlayFab provider). |

---

### C12. PlayFab Suspend / Resume (4 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFab\Suspend\ShouldUploadChanges | Active | Suspend triggers upload. |
| Invoke\PlayFab\SuspendResume\ReinitWhilePeerCheck | Active | **Bug repro** — reinit race during peer check validation. |
| Invoke\PlayFab\SuspendResume\ReinitWhileUploadInProgress | Active | **Bug repro** — 11 iterations with 50ms-15s delays; upload race stress test. |
| Invoke\PlayFab\SuspendResume\ContinuousNoTerminate | Active | 5 iterations, crash tolerance, ETW JSON export. |

---

### C13. PlayFab Storage Full (3 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFabInit\WhenUserContentFull\ShouldPromptCleanup | Active | Creates XVDs until full; expects cleanup UX prompt. |
| Invoke\PlayFabInit\WhenXVDInsufficientForSync\ShouldFlushOtherData | Needs ZIP | XVD insufficient; flush behavior. *Requires manual ZIP at d:\data\mockresponses\.* |
| Invoke\PlayFabExit\WhenXVDInsufficientForCopyBack\ShouldFlushOtherData | Active | Flush during copy-back when XVD insufficient. |

---

### C14. PlayFab Crash Recovery (1 test)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFabInit\WhenServiceCrashesOrPowerLoss\NextStartRecoversPendingXVD | Active | Kills svchost; verifies PlayFab XVD recovery. |

---

### C15. Quick Resume — Title Storage (6 tests)

| Test | Status | Description |
|------|--------|-------------|
| QR\TitleStorage\GetInfo\HitOnActivatedContext | Active | TitleStorage get cache hit on activated context. |
| QR\TitleStorage\SetInfo\Basic | Active | Basic TitleStorage set operation. |
| QR\TitleStorage\GetInfo\MissWhenNoContext | Active | Cache miss when no context exists. |
| QR\TitleStorage\GetInfo\MissWhenNoLock | Active | Cache miss when not locked. |
| QR\TitleStorage\SetThenGet\Roundtrip | Active | Set then get roundtrip validation. |
| QR\TitleStorage\MultipleCycles | Active | Stress test: multiple set/get cycles. |

---

### C16. Quick Resume — PlayFab (7 tests)

| Test | Status | Description |
|------|--------|-------------|
| QR\PlayFab\SaveRestore\Basic | Active | Basic QR save/restore round-trip. |
| QR\PlayFab\PeerConflict\NewerVersionOnCloud | Active | Cloud version newer; evict local. |
| QR\PlayFab\DuringUpload | Active | QR data available during active upload. |
| QR\PlayFab\Timeout | Active | Timeout prevents hangs; deadlock guard. |
| QR\PlayFab\ResumeAfterStateReset\NoSyncPanel | Active | Hub#134 fix: simulates "not tracked" on QR resume; sync <5s, no TCUI. |
| QR\PlayFab\ContinuousHibernate | Active | Repeated suspend/resume cycle stress test. |
| QR\PlayFab\Bug016\StateAccumulation | Active | Bug#016 state accumulation edge case. |

---

### C17. PlayFab Conflict Resolution (7 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFab\ConflictLoser\TakeRemote\ShouldUploadLoserData | Active | Remote wins; loser data uploaded. |
| Invoke\PlayFab\ConflictLoser\TakeLocal\ShouldNotUploadLoserData | Active | Local wins; loser data not uploaded. |
| Invoke\PlayFab\ConflictLoser\TakeRemote\NoExtManifest | Active | Handles missing external manifest. |
| Invoke\PlayFab\ConflictLoser\TakeRemote\MismatchGuid | Active | Handles GUID mismatch. |
| Invoke\PlayFab\ConflictLoser\TakeRemote\ZeroBlob | Active | Handles zero-size blob. |
| Invoke\PlayFab\ConflictLoser\TakeRemote\RapidSuspendResume | Active | Rapid suspend/resume cycles during conflict. |
| QR\PlayFab\LockContention\DialogWithProgressQueued | Active | Dialog with progress queued during contention. |

---

### C18. PlayFab Conflict / Lock Cancel (7 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFab\ConflictCancel\Relaunch\ShouldRepromptConflict | Active | Cancel conflict then relaunch reprompts. |
| Invoke\PlayFab\ConflictCancel\Relaunch\TakeLocal | Active | Cancel then relaunch then take local. |
| Invoke\PlayFab\ConflictCancel\Relaunch\CancelAgain | Active | Cancel then relaunch then cancel again. |
| Invoke\PlayFab\LockContentionCancel\Relaunch\ShouldRepromptConflict | Active | Lock contention cancel then reprompt. |
| Invoke\PlayFab\LockContentionCancel\Relaunch\TakeLocal | Active | Lock contention cancel then take local. |
| Invoke\PlayFab\LockContentionCancel\Relaunch\CancelAgain | Active | Lock contention cancel twice. |
| QR\PlayFab\LockContention\CancelGuardsUpload | Active | Cancel prevents unintended upload. |

---

### C19. PlayFab Recovery / Reactivation (9 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFab\Recovery\VersionSelection\HighestVersionWins | Active | Highest version selected in recovery. |
| Invoke\PlayFab\ReactivationCancelWipe\EndToEnd | Active | Full reactivation cancel wipe cycle. |
| Invoke\PlayFab\SuspendDuringReactivation\ShouldPreserveLocalData | Active | Local data preserved during reactivation suspend. |
| Invoke\PlayFab\SuspendDuringReactivation\CleanupDoesNotDestroyBase | Active | Cleanup doesn't destroy base during reactivation. |
| Invoke\PlayFab\DeleteLocal\Bug3\StaleInit\ShouldDownloadFinalized | Active | Bug#3: stale init handled; download finalized. |
| Invoke\PlayFab\DeleteLocal\Bug1\QRFlush\ShouldClearResumeInfo | Active | Bug#1: QR flush clears resume info. |
| Invoke\PlayFab\DeleteEverywhere\ShouldFlushQRAndDeleteCloud | Active | Deletes QR and cloud data. |
| Invoke\PlayFab\SameDeviceMissingLocalData\ShouldRedownload | Active | Redownloads on same device if local missing. |
| Invoke\PlayFab\TerminalUploadError\ShouldClearSession | Active | Terminal error clears session. |

---

### C20. PlayFab Lock Contention / Offline (6 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFab\LockContention\CancelDuringProgress\ShouldNotCrash | Active | No crash when cancel during progress. |
| Invoke\PlayFab\SameDeviceOffline\ShouldSkipConflict | Active | Same device offline skips conflict. |
| Invoke\PlayFab\DuplicateOfflineManifest\ShouldDeduplicate | Active | Deduplicates duplicate offline manifests. |
| Invoke\PlayFab\StaleJunction\ShouldBeCleanedOnBoot | Active | Stale junctions cleaned on boot. |
| Invoke\PlayFab\TerminalFinalizeError\ShouldAbandonAndClearSession | Active | Abandon and clear on terminal finalize error. |
| Invoke\PlayFab\OfflineInit\StaleManifestJson\ShouldCleanup | Active | Stale manifest.json cleaned up offline. |

---

### C21. PlayFab Version / Folder Management (3 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFab\VersionFolderConflict\ShouldCopyFromPrevious | Active | Copies from previous version on conflict. |
| Invoke\PlayFab\LockContention\StaleProgressCallback\ShouldDetect | Active | Detects stale progress callbacks. |
| Invoke\PlayFab\UTC\OnlineActivation\FieldsCorrect | Active | UTC fields correct during online activation. |

---

### C22. PlayFab Telemetry / Offline (3 tests)

| Test | Status | Description |
|------|--------|-------------|
| Invoke\PlayFab\UTC\OfflineActivation\ReportsNonZeroSize | Active | Offline UTC reports non-zero size. |
| Invoke\PlayFab\UTC\UploadError\EmitsContextSyncError | Active | Upload error emits sync error telemetry. |
| Invoke\PlayFab\OfflineSuspendResume\ShouldNotHang | Active | Offline suspend/resume doesn't hang. |

---

### C23. Forte QR Real-Title (3 tests)

Real-title validation with ForteBaseGame. Requires TURN.1 sandbox, `/p:SkipSignIn=true`.

| Test | Status | Description |
|------|--------|-------------|
| Forte\QR\BasicSaveRestore | Pass | Basic QR round-trip. Validated March 5, 2026. |
| Forte\QR\MultipleConsecutiveCycles | Active | 3 consecutive QR cycles. |
| Forte\QR\SwitchToOther\RestoreForte | Active | Switch to DefaultApp then restore Forte. |

---

## Test Infrastructure Summary

### ETW Monitoring (GameSaveListener)

Real-time ETW event capture with 25+ event types tracked:
- **Web operations**: AcquireLock, ReleaseLock, UploadAtom, UploadSummary, DownloadAtom
- **PlayFab context**: InitStart, InitFinish, Created, Activation
- **Upload context**: Finalized, Complete, UploadChunk, Abandoned, UploadError
- **Service**: InternalError, PackageState, Deadlock
- **Storage**: CleanupStart, CleanupFinish

All events exportable to JSON at `xd:\xgs\<timestamp>.json`.

### Mock Injection (MockUtils)

Registry-based mock response injection at `HKLM\OSDATA\Software\Microsoft\Durango\ConnectedStorage\MockResponses`:
- **Web service mocks**: AcquireLock, ReleaseLock, UploadAtoms, DownloadAtom, UploadContainer, DownloadContainer, DeleteContainer, ListAllContainers, DownloadSettings
- **PlayFab mocks**: LoginWithXbox, ListManifests, GetManifestDetails, GetChunk, InitManifest, InitUpload, UpdateUpload, PushUpload, FinalizeManifest, DeleteManifest

### Test Run Recommendations

| Scope | Tests | Time | Mode |
|-------|-------|------|------|
| Quick validation | 5 | ~10 min | Mock |
| Feature tests | 38 | ~2-3 hr | Mock |
| Final validation | All | Varies | UseRealService=true |

### Test Selectors

```
/select:"@TestScope='Feature'"       -- All feature tests (~38)
/select:"@TestScope='QR'"            -- Quick Resume tests (~10)
/select:"@TestScope='Measure'"       -- Performance tests (~5)
/select:"@Name='*PlayFab*'"          -- PlayFab tests (~17)
/select:"@Name='*SuspendResume*'"    -- Race condition tests
```

---

*End of Test Inventory*

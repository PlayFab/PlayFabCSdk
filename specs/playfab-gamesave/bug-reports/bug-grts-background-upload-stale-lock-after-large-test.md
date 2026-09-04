# Bug: GRTS Background Upload Holds Lock — Causes Next Test to Fail

## Classification
- **Type:** Test isolation / GRTS state leakage between test runs
- **Severity:** Low (flaky, only triggers after large-upload tests)
- **Affected tests:** 86 (Custom File Location Init and Sync), potentially any test following test 83
- **First observed:** pass118 (2026-05-06)

## Symptoms

`PFGameSaveFilesUploadWithUiAsync` fails with `E_FAIL` (0x80004005) from `XAsyncGetStatus`. The API call itself succeeds (hr=0x00000000) and the result is OK (resultHr=0x00000000), but the wait returns E_FAIL.

```
[PlayFab] GameSaveAPIProviderGRTS::UploadWithUiAsync: option=1
[PlayFab] GameSaveAPIProviderGRTS::UploadWithUiAsync: using UploadReleaseActive
[PlayFab] GameSaveAPIProviderGRTS::UploadWithUiAsync: hr=0x00000000
PFGameSaveFilesUploadWithUiAsync (hr=0x00000000, waitHr=0x80004005, resultHr=0x00000000)
```

## Root Cause

**GRTS background upload leaks into the next test.**

1. Test 83 (Large Dataset Sync, 100 MB+) uploads 110 MB to GRTS successfully
2. GRTS `UploadWithUi` returns success immediately (data written to local storage)
3. GRTS continues uploading the 110 MB to blob storage **in the background** via the GamingServices service process
4. Test 83 completes and fully cleans up (PFGameSaveFilesUninitializeAsync, PFUninitializeAsync — all succeed)
5. Only **17 seconds** later, test 86 starts in a new device process
6. GRTS detects "active device contention" — the GamingServices service still has a lock from the ongoing background upload
7. The GRTS descriptor confirms: `totalBytes=115343360, uploadedBytes=0` — the 110 MB upload hasn't started on the network side yet
8. The active device contention auto-responder fires and chooses "SyncLastSavedData"
9. Test 86 subsequently calls `UploadWithUiAsync` with `UploadReleaseActive` to force-release the lock
10. GRTS cannot release the lock while the background upload is in progress → returns E_FAIL

## Evidence

### GRTS Descriptor at Test 86 Start (showing test 83's pending 110 MB upload)
```
ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor: input time=1778086924, totalBytes=115343360, uploadedBytes=0
```

### Timeline
```
10:07:18  Test 83 cleanup completes (all uninits succeed)
10:07:35  Test 86 starts (new device process, same GamingServices)
10:07:38  AddUserWithUi detects contention: "Device A - Entity Auth Upload"
10:07:41  UploadWithUiAsync called (UploadReleaseActive path)
10:07:43  XAsyncGetStatus returns E_FAIL
```

Gap between test 83 end and test 86 failure: **25 seconds** — not enough time for GRTS to upload 110 MB.

## Impact

- Only affects test ordering where a large-upload test (83) runs immediately before another GRTS test (86)
- The 110 MB upload needs time to complete over the network
- Without a mechanism to wait for GRTS background sync to finish, the next test may encounter stale locks

## Possible Fixes

1. **Test ordering:** Move test 83 to the end of the run (after all other GRTS tests)
2. **Inter-test delay:** Add a configurable delay after large-upload tests in the run script
3. **GamingServices restart:** Restart the GamingServices service between test 83 and subsequent tests to flush background state
4. **FlushGrts step:** Add a test step that calls `FlushGrtsAndWait` or polls until `uploadedBytes == totalBytes` before ending test 83
5. **Product fix:** GRTS `UploadReleaseActive` should wait for background sync completion rather than returning E_FAIL

## Affected Environment
- Platform: pc-grts (GDK with GamingServices)
- Passes affected: pass118
- Not reproducible on pc-inproc (no background upload behavior)

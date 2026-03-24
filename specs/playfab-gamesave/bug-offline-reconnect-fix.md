# Bug: PFGameSaveFilesAddUserWithUiAsync fails to reconnect after offline mode

## Title
PFGameSaveFiles: AddUserWithUiAsync returns E_PF_GAMESAVE_USER_ALREADY_ADDED when attempting to reconnect from offline mode

## Repro Steps
1. Initialize PFGameSaveFiles and call `PFGameSaveFilesAddUserWithUiAsync`
2. Simulate or experience network failure during sync (e.g., 503 errors from service)
3. When the `PFGameSaveFilesUiSyncFailedCallback` is invoked, respond with `UseOffline` action
4. Verify `PFGameSaveFilesIsConnectedToCloud` returns `false` (user is now in offline/disconnected mode)
5. Later, when network connectivity is restored, call `PFGameSaveFilesAddUserWithUiAsync` again to reconnect to cloud
6. **Expected:** Reconnection succeeds and sync resumes
7. **Actual:** Returns `E_PF_GAMESAVE_USER_ALREADY_ADDED` (0x89237002)

## Root Cause
The `AddUserWithUiAsync` implementation only checked if `syncState != NotStarted` to determine if a user was already added. This didn't account for the documented reconnection scenario where a user in "disconnected from cloud" state should be allowed to call `AddUserWithUiAsync` again to reconnect.

Additionally, when reconnecting, the internal sync manager state (manifests, file sets) was not being reset, which could cause stale state issues.

## Fix Description
Two changes were made:

### 1. PFGameSaveFilesAPIProvider_Win32.cpp
Modified `AddUserWithUiAsync` to allow the call when the user is in forced-disconnect-from-cloud state:
- Added check for `IsForcedDisconnectFromCloud()` in addition to the `NotStarted` state check
- When reconnecting (forced disconnect state), call `InitForDownload()` to reset sync state before proceeding

### 2. FolderSyncManager.cpp
Enhanced `InitForDownload()` to fully reset sync manager state:
- Reset `m_isForcedDisconnectFromCloud` flag to false
- Clear cached manifests (`m_latestFinalizedManifest`, `m_latestPendingManifest`)
- Clear file/folder sets (`m_localFileFolderSet`, `m_remoteFileFolderSet`)

This ensures a clean state when reconnecting, preventing stale data from affecting the new sync operation.

## Workaround (for unfixed versions)
Games can work around this issue by fully uninitializing and reinitializing the GameSave system:
1. `PFLocalUserCloseHandle`
2. `PFGameSaveFilesUninitializeAsync`
3. `PFGameSaveFilesInitialize` (with same folder path - local data is preserved)
4. Re-register callbacks
5. `PFLocalUserCreateHandleWithPersistedLocalId`
6. `PFGameSaveFilesAddUserWithUiAsync`

---

## Release Notes (GDK)

**PFGameSaveFiles: Fixed offline-to-online reconnection**

Fixed an issue where `PFGameSaveFilesAddUserWithUiAsync` incorrectly returned `E_PF_GAMESAVE_USER_ALREADY_ADDED` when attempting to reconnect to cloud services after operating in offline mode.

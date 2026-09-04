# PFGameSave Xbox Console Automated Test Plan

## Purpose
Define the automation strategy for PFGameSave on Xbox console and GRTS PC — both platforms use the same out-of-process GRTS provider (`GameSaveAPIProviderGRTS`) and share the same code path. This plan complements the existing in-proc automation plan (`testing/test-automation/1-test-automation-plan.md`) by targeting scenarios unique to the out-of-process model: background uploads, PLM lifecycle, TCUI-driven dialogs, console power states, active device contention through the platform shell, and title switching.

**Test app**: `Test/GameTestAppXbox` — an ATG-based GDK application that connects to the `GameTestController` via WebSocket. It shares 100+ handler files with `GameTestAppWindows` through `GameTestAppShared/` and supports the full PFGameSaveFiles command surface. Xbox Live authentication uses `XUserAddAsync` → `PFLocalUserCreateHandleWithXboxUser`.

**UI callback approach**: TCUI (stock platform UI) is optional. The test app registers programmatic UI callbacks via `PFGameSaveFilesSetUiCallbacks` and `PFGameSaveFilesSetActiveDeviceChangedCallback`, giving automation full control over conflict, contention, out-of-storage, and progress dialogs — the same mechanism used in the in-proc test harness. Where TCUI validation is needed (visual checks, accessibility), those remain manual.

**Key difference from in-proc plan**: The GRTS provider runs uploads out-of-process through Xbox Gaming Runtime Services. HTTP calls cannot be intercepted via LHC mocks because they originate from the OS service, not the game process. Fault injection must use external mechanisms (network control, storage manipulation, process lifecycle management) rather than HTTP-level interception.

## Section Map
- **Selection Philosophy** — decision filters adapted for Xbox/GRTS.
- **Priority Bands** — P0–P3 urgency framework.
- **Test Scenario List** — numbered automation backlog with gap-test cross-references.
- **Automation Actions Requiring Investigation** — actions with unknown automation feasibility; each needs a spike to determine tooling.
- **Additional Manual-Only Gaps** — tests that require human judgment or visual verification.
- **Test Automation Framework Features** — harness capabilities needed for Xbox/GRTS execution.
- **Scenario Details** — step-by-step execution notes for each numbered scenario.

## Selection Philosophy
- **Player-Critical Risk First**: Prioritize scenarios where the out-of-process upload model introduces unique data-loss vectors — background upload after app close, PLM eviction during sync, power loss with pending uploads, and active device arbitration through the platform shell.
- **GRTS Provider Parity**: Validate that the GRTS code path produces identical data outcomes as the in-proc provider for overlapping scenarios (golden path, conflict resolution, rollback). Divergences are high-priority bugs.
- **Lifecycle Resilience**: Console-specific transitions (suspend/resume, connected standby, title eviction, Quick Resume) are the primary gap versus the in-proc plan. Each PLM state change must be tested against every game-save phase (init, download, local write, upload).
- **Deterministic and Assertable**: Use `CaptureSaveContainerSnapshot` + `CompareSaveContainerSnapshots` for file-level verification. UI callback responses are scripted; assertions check HRESULTs, snapshot hashes, and callback ordering.
- **Reuse Existing Harnesses**: Build on the `GameTestController` ↔ `GameTestAppXbox` WebSocket architecture. New scenarios are YAML manifests using existing commands (`WriteGameSaveData`, `DeleteSaveRoot`, `PFGameSaveFilesUploadWithUiAsync`, etc.). Platform-specific actions (suspend, network toggle) extend the command surface.
- **External Fault Injection**: Since LHC mocks cannot intercept GRTS out-of-proc HTTP traffic, use external mechanisms: managed network switches or dev-kit network APIs for connectivity faults, `ConsumeDiskSpace` harness command for storage pressure, Xbox Device Portal REST API or `xbrun` for process lifecycle control.

## Priority Bands
- **P0 — Foundational Guardrails**: Must-run smoke proving Xbox Live sign-in, cloud sync via GRTS, and background upload complete without data loss. Failures block merges.
- **P1 — High-Risk GRTS Scenarios**: Volatile flows unique to the out-of-process model — background upload survival, active device contention, conflict resolution, suspend/resume, title termination during sync, and auth/SPOP. Run nightly or per-commit on dev kits.
- **P2 — Depth and Scale**: Broader coverage for storage pressure, large datasets, multi-file sync, deletion propagation, rollback, network instability, user switching, and title switching. Results guide targeted fixes before release.
- **P3 — Resilience, Power States, and Soak**: Power loss, connected standby, regulatory standby, structured shutdown, long-running soak, and bug regression scenarios that require specialized hardware or timing. Scheduled weekly or before milestones.

## Test Scenario List

> Scenarios are grouped by functional area. Each scenario retains its original number (not renumbered). Priority (P0–P3) is shown in parentheses after the title.

### A. Golden Path / Smoke

ID1. **Single-Device Golden Path Sync — Xbox Live Auth (P0)**
    *Gap refs*: 53971519 (golden path online init)
    *Scope*: Initialize via `XUserAddAsync` + `PFLocalUserCreateHandleWithXboxUser`, register UI callbacks, write a 10 KB payload, upload with `ReleaseDeviceAsActive`, tear down, relaunch, confirm the payload downloads intact via snapshot comparison. Run on both Xbox Scarlett and Xbox One configs.
    *Why*: Proves the GRTS provider end-to-end with Xbox Live auth. Every subsequent scenario depends on this.
    *Key assertions*: All API calls return success, snapshot hashes match across sessions, no unexpected UI callbacks fire, `PFGameSaveFilesIsConnectedToCloud` returns `true`.
    *Status*: `gamesave-xbox-01-single-device-golden-path.yml` exists — extend with snapshot comparison.

ID2. **Two-Device Golden Path Sync — Xbox Live Auth (P0)**
    *Gap refs*: 53971519, 59026195 (golden path mid-game upload)
    *Scope*: Device A uploads, Device B downloads and modifies, Device A confirms propagation. Both use Xbox Live auth and UI callbacks with `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` for contention.
    *Why*: Validates multi-device cloud propagation through the GRTS provider and active device handoff.
    *Key assertions*: Snapshot hashes match, save descriptions propagate, contention auto-response fires on Device B, no data loss.
    *Status*: `gamesave-xbox-02-two-device-golden-path.yml` exists.

ID3. **Background Upload Verification — Out-of-Game Upload (P0)**
    *Gap refs*: 53972820 (golden path out-of-game upload), SD-GAP-001
    *Scope*: Device A creates a payload, closes the app (triggering GRTS background upload), waits 2–5 minutes, then Device B launches and verifies the payload arrived. This is the fundamental GRTS differentiator — upload continues after the game process exits.
    *Why*: The entire GRTS value proposition depends on background uploads completing reliably. A failure here means player progress is silently lost.
    *Key assertions*: Device B receives full payload (snapshot match), no contention dialog on Device B (Device A released the lock), upload completes within the wait window.

ID4. **Stock TCUI — Golden Path (P1)**
    *Scope*: Do NOT call `PFGameSaveFilesSetUiCallbacks` (or call with `enable: false`). Run the single-device golden path — write, upload, close, relaunch, verify download. With stock TCUI, the platform renders progress/contention/conflict dialogs automatically. Verify the flow completes without custom callback intervention.
    *Why*: All existing scenarios use custom UI callbacks. Stock TCUI is the default mode and must work end-to-end.
    *Key assertions*: Init succeeds, upload/download work, snapshot matches, no crash from missing callbacks. Note: assertion of UI appearance is manual.

### B. Active Device Contention

ID5. **Active Device Contention — Take Over (P1)**
    *Gap refs*: MD-GAP-006, MD-GAP-007
    *Scope*: Device A holds the active lock. Device B launches with the same user, receives the contention UI callback, and selects `SyncLastSavedData` to take ownership. Verify Device A's active-device-changed callback fires. Device B uploads, Device A re-syncs.
    *Why*: Active device arbitration is the primary multi-device safety mechanism in GRTS.
    *Key assertions*: Contention callback fires on Device B with correct metadata, Device A transitions gracefully, Device B's upload succeeds, Device A's subsequent sync reflects Device B's data.

ID6. **Active Device Contention — During Download (P1)**
    *Gap refs*: MD-GAP-008
    *Scope*: Device B is mid-download when Device A takes over the active lock. Verify Device B handles the interruption gracefully (title terminates or sync fails cleanly).
    *Why*: Contention during an active transfer is a high-risk race condition.
    *Key assertions*: No data corruption on either device, no orphaned partial downloads, Device A syncs cleanly after takeover.

ID7. **Active Device Contention — During Upload (P1)**
    *Gap refs*: MD-GAP-009
    *Scope*: Device B closes the app (background upload in progress). Device A launches and takes over. Verify the interrupted upload does not corrupt cloud state.
    *Why*: Background upload interrupted by contention is a GRTS-specific risk.
    *Key assertions*: Cloud manifest reflects a coherent state (either Device B's upload completed before takeover or reverted), Device A syncs without corruption.

ID8. **Contention — Retry Response (P1)**
    *Scope*: Device B holds active lock. Device A launches, contention callback fires. Respond with `Retry`. Verify the system re-checks the lock state. If Device B releases the lock between retries, verify Device A eventually proceeds. If not, verify the contention callback fires again.
    *Key assertions*: Retry re-checks lock, eventual success when lock clears, no hang, correct callback sequence.

ID9. **Stock TCUI — Contention Resolution (P1)**
    *Scope*: Without custom UI callbacks, set up active device contention. Device B holds lock. Device A launches without `PFGameSaveFilesSetUiCallbacks`. The platform renders the stock contention dialog. Use `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` to script the response (verify auto-response still works in stock TCUI mode). Verify sync completes.
    *Why*: Auto-response should work regardless of callback mode. Some games may mix auto-response with stock TCUI.
    *Key assertions*: Auto-response fires even without custom callbacks, sync completes, snapshot matches.

ID10. **IsConnectedToCloud After Active Device Changed (P2)**
    *Scope*: Device A is active. Device B takes over (SyncLastSavedData). On Device A, the active device changed callback fires. After the callback, call `PFGameSaveFilesIsConnectedToCloud` on Device A. Verify it returns `false` (device is now disconnected from cloud because it's no longer active).
    *Why*: Documented behavior: "disconnection can happen in multiple ways... Active device changed: Another device takes over as the active device, automatically putting this device in offline mode."
    *Key assertions*: `IsConnectedToCloud` returns `false` after active device changed, upload attempts return `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`.

ID11. **Active Device Changed — Full Re-Init Lifecycle (P1)**
    *Scope*: Device A is active and playing. Device B takes over (SyncLastSavedData). Device A receives active device changed callback. Device A: uninitializes Game Saves, returns to "main menu" state, re-initializes, calls `AddUserWithUiAsync` again. Verify Device A re-syncs and receives Device B's latest data.
    *Why*: Documented best practice: "Return to a safe state (main menu)" then "Re-initialize Game Saves system when ready to play again." This full lifecycle flow is not tested.
    *Key assertions*: Callback fires on Device A, uninit succeeds, re-init succeeds, AddUser re-syncs with Device B's data, no orphaned state.

ID12. **Contention — “Go Back” from Confirmation Dialog (P2)**
    *Gap refs*: UI-GAP-001
    *Scope*: Device B holds the active lock. Device A launches, contention fires. Instead of resolving immediately, simulate the sample-app’s 2-step flow: choose “Continue from Cloud Data” → Confirmation dialog → press “Go Back” → re-enter contention dialog. Then resolve with “Retry” (contention fires again) and finally “SyncLastSavedData.” Verify callback re-entry doesn’t hang or double-fire.
    *Why*: The sample app’s Contention → Confirmation → Go Back flow exercises callback re-entry. If the system doesn’t handle it, the async operation may hang.
    *Key assertions*: Delayed response causes no hang, Retry re-fires contention, final SyncLastSavedData resolves correctly, snapshot matches expected data.

ID13. **Contention — Cancel Response (P1)**
    *Gap refs*: UI-GAP-009
    *Scope*: Device B holds the active lock. Device A launches, contention fires. Respond with Cancel. Verify `AddUserWithUiAsync` returns `E_PF_GAMESAVE_USER_CANCELLED`. Then retry — contention fires again, resolve with `SyncLastSavedData`.
    *Why*: Every contention test uses SyncLastSavedData or Retry. The Cancel path — where the player decides not to proceed — has zero coverage.
    *Key assertions*: Cancel returns E_PF_GAMESAVE_USER_CANCELLED, no partial state, retry works and contention re-fires, no orphaned lock.

### C. Conflict Resolution

ID14. **Conflict Resolution — Local Wins via UI Callback (P1)**
    *Gap refs*: MD-GAP-010, MD-GAP-011
    *Scope*: Device A uploads. Device B creates divergent local data while offline (simulate via network disconnect or stale local). Device B reconnects and syncs — conflict callback fires. Respond with `UseLocal`. Verify local data wins and subsequent upload propagates to Device A.
    *Why*: Validates conflict resolution through the GRTS provider with UI callbacks (not TCUI).
    *Key assertions*: Conflict callback fires once with expected folder list, `UseLocal` response resolves correctly, snapshot hashes match Device B's local data, Device A receives the resolved state.

ID15. **Conflict Resolution — Cloud Wins via UI Callback (P1)**
    *Gap refs*: MD-GAP-010, MD-GAP-012
    *Scope*: Same conflict setup as Scenario ID14, but respond with `UseCloud`. Verify cloud data wins and local divergent data is discarded.
    *Key assertions*: `UseCloud` discards local changes, snapshot matches Device A's original upload, no stale local data remains.

ID16. **Conflict Resolution — Play Offline (P1)**
    *Gap refs*: MD-GAP-010, MD-GAP-013
    *Scope*: Same conflict setup, but dismiss/cancel the conflict dialog (play offline). Verify local data is loaded without cloud merge. On next launch, verify the conflict re-prompts.
    *Key assertions*: Offline mode loads local-only data, cloud data not mixed in, conflict re-fires on next online launch, eventual resolution works.

ID17. **Stock TCUI — Conflict Resolution (P1)**
    *Scope*: Without custom UI callbacks, set up a conflict. Use `PFGameSaveFilesSetUiSyncConflictAutoResponse(UseLocal)` to script the response. Verify conflict resolves correctly through the GRTS provider with stock TCUI.
    *Key assertions*: Auto-response works without callbacks, conflict resolves, snapshot matches local data.

ID18. **Multi-Folder Merge — No Conflict (P1)**
    *Scope*: Device A modifies files in `Save1/`. Device B modifies files in `Save2/`. Both upload. On next sync, verify both changes merge automatically with NO conflict dialog. Validates that changes in different atomic units (root-level subfolders) do not trigger conflicts.
    *Why*: The conflict detection granularity (per root subfolder) is a core design feature documented in `conflicts.md`. A false conflict here would block players unnecessarily.
    *Key assertions*: No conflict callback fires, both folders' changes present after sync, snapshot contains data from both devices.

ID19. **Same-Subfolder Conflict — Different Files (P1)**
    *Scope*: Device A modifies `Save1/stats.json`. Device B modifies `Save1/inventory.json` (different file, same atomic unit `Save1/`). On sync, verify a conflict IS triggered because both changes are in the same root-level subfolder.
    *Why*: Validates that atomic unit detection works at the folder level, not the file level. This is counter-intuitive behavior that must be correct.
    *Key assertions*: Conflict callback fires, resolution applies to entire `Save1/` folder, non-conflicted folders unaffected.

ID20. **Root-Level File Conflict (P1)**
    *Scope*: Device A modifies `rootfile1.txt` (at save root). Device B modifies `rootfile2.txt` (different file, also at save root). On sync, verify a conflict IS triggered because all root-level files share one atomic unit.
    *Why*: Root-level files sharing a single atomic unit is a documented "special case" that games must understand. Incorrect behavior here causes silent data loss.
    *Key assertions*: Conflict callback fires for root-level changes, resolution is all-or-nothing.

ID21. **All-or-Nothing Conflict Resolution — Keep Local Loses Cloud-Only Changes (P1)**
    *Scope*: Set up a mixed scenario: Device A modifies `SlotA/` and `SlotC/`. Device B modifies `SlotA/` and `SlotB/`. Conflict triggers (both modified `SlotA/`). User chooses "Keep Local". Verify: SlotA keeps local, SlotB is uploaded, but SlotC cloud-only changes are LOST (replaced by local state).
    *Why*: This is the documented all-or-nothing behavior — "Keep Local loses cloud-only changes." Players must understand this, and the SDK must implement it correctly.
    *Key assertions*: SlotA = local data, SlotB = local data uploaded, SlotC = local state (cloud change lost), all verified via snapshots.

ID22. **All-or-Nothing Conflict Resolution — Keep Cloud Loses Local-Only Changes (P1)**
    *Scope*: Same mixed setup as Scenario ID21. User chooses "Keep Cloud". Verify: SlotA keeps cloud, SlotC is downloaded, but SlotB and SlotD local-only changes are LOST (overwritten by cloud state).
    *Key assertions*: SlotA = cloud data, SlotC = cloud data, SlotB = cloud state (local change lost), all verified via snapshots.

ID23. **Delete on Both Sides — No Conflict (P2)**
    *Scope*: Device A and Device B both delete the same file (or files in the same atomic unit). On sync, verify NO conflict occurs — the system recognizes both devices agree the file should be removed.
    *Key assertions*: No conflict callback, file is deleted in final state, other data unaffected.

ID24. **Multi-Callback Sequence — Contention then Conflict (P2)**
    *Gap refs*: UI-GAP-002
    *Scope*: Device B holds the lock AND has divergent data. Device A launches — first contention fires (Device B holds lock), then after contention resolves the conflict callback fires (divergent data). Verify both callbacks fire in sequence and resolutions chain correctly.
    *Why*: Games must handle back-to-back callbacks in a single `AddUserWithUiAsync` call. A system expecting only one callback per operation may silently drop the second.
    *Key assertions*: Both contention and conflict fire in sequence, contention resolves first, final data reflects conflict choice, no hang between callbacks.

### D. Sync Failure / Offline

ID25. **SyncFailed — Cancel Response (P1)**
    *Scope*: Go offline. Launch app. Sync failure callback fires. Respond with `Cancel`. Verify the app handles cancellation gracefully — `AddUserWithUiAsync` returns `E_PF_GAMESAVE_USER_CANCELLED`, no partial state, app can retry later.
    *Key assertions*: Correct HRESULT returned, no partial data, relaunch after going online succeeds.

ID26. **SyncFailed — Retry Response (P1)**
    *Scope*: Go offline. Launch app. Sync failure callback fires. Respond with `Retry` (while still offline — should fail again). Go online. Respond with `Retry` again. Verify sync eventually succeeds.
    *Why*: The Retry path for sync failure is untested. Games present "Try Again" buttons that depend on this.
    *Key assertions*: First retry fails (still offline), second retry succeeds (online), data syncs correctly.

ID27. **Stock TCUI — Sync Failure Offline Fallback (P1)**
    *Scope*: Without custom UI callbacks, go offline and launch. Stock TCUI renders the sync failure dialog. Use `PFGameSaveFilesSetUiSyncFailedAutoResponse(UseOffline)` to script the response. Verify offline mode activates, local data accessible.
    *Key assertions*: Auto-response works, offline mode activates, no hang, data accessible.

ID28. **Upload in Offline Mode — E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD (P1)**
    *Scope*: Go offline. Complete `AddUserWithUiAsync` with `UseOffline` response. Write save data. Call `PFGameSaveFilesUploadWithUiAsync`. Verify: the call returns `S_OK` immediately, but the `XAsyncBlock` completion returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` (0x89237004).
    *Why*: Documented offline API behavior that games must handle correctly. A game that doesn't check the async result will think the upload succeeded.
    *Key assertions*: `UploadWithUiAsync` call returns `S_OK`, async result returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`, local data intact.

ID29. **GetRemainingQuota in Offline Mode (P2)**
    *Scope*: Enter offline mode (same as Scenario ID28). Call `PFGameSaveFilesGetRemainingQuota`. Verify it returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`.
    *Key assertions*: Correct HRESULT returned, no crash.

ID30. **Return to Online Without Re-Init (P1)**
    *Scope*: Go offline. Enter offline mode via `UseOffline`. Write data locally. Reconnect network. Call `PFGameSaveFilesAddUserWithUiAsync` again (without calling Uninitialize/Initialize). Verify sync completes and local data uploads — the docs state no re-init is needed.
    *Why*: This is explicitly documented: "Call `PFGameSaveFilesAddUserWithUiAsync()` again to attempt reconnection. No need to fully re-initialize the Game Saves system."
    *Key assertions*: AddUser succeeds without re-init, `IsConnectedToCloud` returns `true` after reconnect, local data syncs to cloud.

ID31. **SyncFailed Error Code Specificity (P2)**
    *Gap refs*: UI-GAP-003
    *Scope*: Trigger sync failures with different root causes (network down, auth failure). Verify the HRESULT in the SyncFailed callback distinguishes between failure types rather than returning a generic code.
    *Why*: Games display error text based on the HRESULT. If all failures return the same generic code, games can’t give useful guidance.
    *Key assertions*: Network failure → network-related HRESULT, auth failure → auth HRESULT, codes are distinct and documented, no catch-all hiding the real failure.

### E. Progress, Cancel, and Mid-Sync Actions

ID32. **Progress Query During Download (P1)**
    *Scope*: Seed a moderately large payload (10+ MB) on Device B. Device A starts download. During the progress callback, call `PFGameSaveFilesUiProgressGetProgress` to query current sync state, bytes transferred, and total bytes. Verify values are sane (state transitions forward, bytes monotonically increase, total > 0).
    *Why*: The progress query API is fully exposed but never tested. Games rely on this for progress bars.
    *Key assertions*: `GetProgress` returns `S_OK`, sync state transitions are monotonic (NotStarted → PreparingForDownload → Downloading → SyncComplete), byte counts are sane, download completes.

ID33. **Cancel Sync During Download (P1)**
    *Scope*: Seed cloud data on Device B. Device A starts download. During the progress callback, respond with `PFGameSaveFilesSetUiProgressResponse(Cancel)`. Verify download aborts cleanly. Relaunch and verify a fresh download can complete.
    *Why*: Players may cancel a long sync. The cancel path must leave no partial or corrupt state.
    *Key assertions*: Cancel returns `E_PF_GAMESAVE_USER_CANCELLED` (0x800704c7), no partial files on disk, relaunch download completes cleanly.

ID34. **Cancel Sync During Upload (P1)**
    *Scope*: Device A writes data and begins upload via `PFGameSaveFilesUploadWithUiAsync`. During the upload progress callback, respond with Cancel. Verify upload aborts. Close and relaunch. Verify data state — local data should persist, cloud state should be coherent.
    *Key assertions*: Cancel returns `E_PF_GAMESAVE_USER_CANCELLED`, local data intact, cloud state coherent (no partial upload), retry upload succeeds.

ID35. **Write to Save Folder During Upload (P2)**
    *Scope*: Start an upload via `PFGameSaveFilesUploadWithUiAsync`. Monitor the progress callback for sync state `Uploading` (0x4). Once `Uploading` state is reached, immediately write new data to the save folder. Verify the write succeeds and the current upload completes with the pre-write data. Then upload again to sync the new data.
    *Why*: Documented: "Once the sync state transitions to `Uploading`, the system has finished reading your files and it's safe to write to the save folder again."
    *Key assertions*: Write during `Uploading` state succeeds, current upload contains pre-write data only, subsequent upload captures new data.

ID36. **Sync State Transition Sequence Verification (P2)**
    *Gap refs*: UI-GAP-004
    *Scope*: During a large download, record every `syncState` from progress callbacks. Verify transitions follow `NotStarted → PreparingForDownload → Downloading → SyncComplete`. Repeat for upload path. Check `currentBytes` increases monotonically and `totalBytes` is consistent.
    *Why*: Games use sync state for progress bars and “safe to write” detection. Out-of-order or skipped states cause incorrect UI.
    *Key assertions*: No backward state transitions, SyncComplete is final, totalBytes stable, currentBytes ≤ totalBytes throughout.

### F. UI Callback Data Verification

ID37. **Descriptor Fields — Contention Callback (P2)**
    *Scope*: Set up active device contention (Device B holds lock, Device A launches). When the contention callback fires on Device A, inspect both `localGameSave` and `remoteGameSave` `PFGameSaveDescriptor` structs. Verify: `deviceType` is non-empty, `deviceId` is non-empty, `time` is reasonable (within 24 hours), `totalBytes` matches expected payload size, `shortSaveDescription` matches what was set via `PFGameSaveFilesSetSaveDescriptionAsync`.
    *Why*: Descriptor fields are exposed to all callback consumers but never asserted.
    *Key assertions*: All descriptor fields populated, timestamps reasonable, sizes match, description matches.

ID38. **Descriptor Fields — Conflict Callback (P2)**
    *Scope*: Set up a conflict (Device A uploads, Device B creates divergent data offline). When the conflict callback fires on Device B, inspect `localGameSave` and `remoteGameSave` descriptors. Verify fields match expected state for each side.
    *Key assertions*: Local descriptor reflects Device B's data (size, timestamp), remote descriptor reflects Device A's cloud data, `shortSaveDescription` for each side is correct.

ID39. **Thumbnail in Descriptors (P2)**
    *Scope*: Write a `pfthumbnail.png` file at the root of the save folder before uploading. On the receiving device, set up contention or conflict. Verify the `thumbnailUri` field in the `PFGameSaveDescriptor` is populated and points to a valid path. Optionally verify the file contents match.
    *Why*: Thumbnail support is documented in the GRTS API but never tested.
    *Key assertions*: `thumbnailUri` is non-null in descriptor, file exists at the URI path, thumbnail data matches what was written.

ID40. **GetRemainingQuota — Explicit Verification (P2)**
    *Scope*: After init and first upload, call `PFGameSaveFilesGetRemainingQuota` and record the value. Write and upload a known-size payload. Call `GetRemainingQuota` again. Verify the delta matches the payload size (within margin). Write near-quota data and verify the value approaches zero or goes negative.
    *Why*: Quota API is exposed but never explicitly tested. Scenario ID47 tests exhaustion but doesn't call this API.
    *Key assertions*: `GetRemainingQuota` returns `S_OK`, values decrease as expected, negative value when over quota.

ID41. **Thumbnail Absent — Verify Empty URI (P2)**
    *Gap refs*: UI-GAP-005
    *Scope*: Upload save data WITHOUT a `pfthumbnail.png`. Trigger contention or conflict on another device. Verify the `thumbnailUri` field in the `PFGameSaveDescriptor` is a clean empty string (not null, not garbage).
    *Why*: Scenario ID39 tests with a thumbnail present. This covers the absence case — games checking `thumbnailUri[0] != '\0'` need a clean empty string.
    *Key assertions*: thumbnailUri is empty string, no crash on empty thumbnail, other descriptor fields still populated.

ID42. **shortSaveDescription Round-Trip in Conflict Descriptor (P2)**
    *Gap refs*: UI-GAP-006
    *Scope*: Set a save description via `PFGameSaveFilesSetSaveDescriptionAsync`, upload, trigger conflict on another device. Verify `shortSaveDescription` in the conflict callback’s `PFGameSaveDescriptor` matches what was set for both local and remote saves.
    *Why*: Conflict UI shows “This Device” vs “Cloud Save” with descriptions. Verifying the round-trip ensures games can display accurate save summaries.
    *Key assertions*: Local descriptor has local description, remote descriptor has remote description, not truncated or corrupted, empty description → empty string.

ID43. **Out of Storage — requiredBytes Accuracy (P2)**
    *Gap refs*: UI-GAP-007
    *Scope*: Fill storage to leave N bytes free. Trigger download needing more than N bytes. Verify `requiredBytes` in the out-of-storage callback is approximately the shortfall (needed − free), not a meaningless default.
    *Why*: Games display “Need X MB free” based on `requiredBytes`. Inaccurate values cause bad UX (user frees space but it’s still not enough).
    *Key assertions*: requiredBytes > 0, within 20% of expected shortfall, not a hardcoded default, no crash on report.

### G. Out of Storage

ID44. **Storage Full During Init (P2)**
    *Gap refs*: SD-GAP-038, SD-GAP-041
    *Scope*: Fill storage before init. Verify out-of-storage callback fires. Free space and retry. Use `ConsumeDiskSpace` harness command.
    *Key assertions*: Out-of-storage callback fires, no crash, retry after freeing space succeeds, no partial containers.

ID45. **Storage Full During Download (P2)**
    *Gap refs*: SD-GAP-039, SD-GAP-042
    *Scope*: Fill storage mid-download. Verify graceful failure. Free space and retry.
    *Key assertions*: Download aborts cleanly, no corruption, retry succeeds.

ID46. **Storage Full During Upload (P2)**
    *Gap refs*: SD-GAP-040, SD-GAP-043
    *Scope*: Fill storage, write data, close app (triggering background upload). Verify upload behavior under storage pressure.
    *Key assertions*: Upload reads from local data (should not need free space to upload), verify on Device B. Document behavior if upload fails.

ID47. **Cloud Quota Exhaustion — 256 MB Limit (P2)**
    *Gap refs*: SD-GAP-046
    *Scope*: Write data approaching the 256 MB quota limit. Attempt to exceed it. Verify the upload is blocked with appropriate error. Delete data to recover quota and verify recovery.
    *Key assertions*: Quota error surfaces correctly, no silent data loss, quota recovery works.

ID48. **OutOfStorage — Cancel Response (P2)**
    *Scope*: Fill storage. Start a download. Out-of-storage callback fires. Respond with `Cancel` instead of `Retry`. Verify `AddUserWithUiAsync` returns `E_PF_GAMESAVE_USER_CANCELLED`, no partial data left, relaunch after freeing space works.
    *Key assertions*: Cancel returns correct HRESULT, no partial files, retry after freeing space succeeds.

ID49. **OutOfStorage — Cancel During Upload (P2)**
    *Gap refs*: UI-GAP-008
    *Scope*: Fill storage. Write data, start upload. If out-of-storage fires during upload, respond Cancel. Verify upload returns `E_PF_GAMESAVE_USER_CANCELLED` and local data remains intact. (GRTS may not need local free space for upload — document if callback doesn’t fire.)
    *Why*: Scenario ID48 tests Cancel during download-time out-of-storage. This covers upload-time, which may differ since GRTS handles uploads out-of-process.
    *Key assertions*: If callback fires: Cancel returns correct HRESULT, local data intact. If not: document behavior.

### H. Foreground / Background

ID50. **Foreground ↔ Background During Init (P1)**
    *Gap refs*: SD-GAP-002, SD-GAP-005
    *Scope*: During `PFGameSaveFilesAddUserWithUiAsync` (while a contention dialog is pending), move the app to background, wait 10–15 seconds, return to foreground. Verify the dialog reappears and sync can complete.
    *Why*: Players frequently switch away during loading. The GRTS provider must survive the focus loss.
    *Key assertions*: No crash on background/foreground transition, UI dialog resumes on return, sync completes with correct data.

ID51. **Foreground ↔ Background During Download (P1)**
    *Gap refs*: SD-GAP-003, SD-GAP-006
    *Scope*: During a cloud download, move the app to background, wait 15–30 seconds, return. Verify the download completes without corruption.
    *Key assertions*: Download completes successfully, snapshot matches source, no partial files.

ID52. **Foreground ↔ Background During Local Write (P1)**
    *Gap refs*: SD-GAP-004, SD-GAP-007
    *Scope*: Immediately after `WriteGameSaveData`, move to background, wait, return. Close app and verify background upload completes. Confirm on Device B.
    *Key assertions*: Local write persists, background upload succeeds, Device B receives the data.

### I. Suspend / Resume

ID53. **Suspend → Resume During Init (P1)**
    *Gap refs*: SD-GAP-008
    *Scope*: During init (contention dialog pending), trigger PLM suspend. Wait 1 minute. Resume. Verify the app recovers and contention dialog re-prompts.
    *Why*: Console PLM suspend is involuntary and frequent; the SDK must survive it at every phase.
    *Key assertions*: No crash on suspend/resume, dialog re-prompts, sync completes after resume.

ID54. **Suspend → Resume During Download (P1)**
    *Gap refs*: SD-GAP-009
    *Scope*: During a cloud download, trigger suspend. Wait 1 minute. Resume. Verify download retries or completes.
    *Key assertions*: Download succeeds after resume, snapshot matches, no partial files.

ID55. **Suspend → Resume During Local Write (P1)**
    *Gap refs*: SD-GAP-010
    *Scope*: Immediately after writing save data, trigger suspend. Resume. Close app and verify background upload. Confirm on Device B.
    *Key assertions*: Local write survives suspend, background upload completes, Device B verifies.

ID56. **Connected Standby → Resume During Init (P3)**
    *Gap refs*: SD-GAP-020
    *Scope*: During init, enter connected standby. Wait 5 minutes. Resume. Verify sync can complete.
    *Key assertions*: App resumes, sync completes, data correct.

ID57. **Connected Standby → Resume During Download (P3)**
    *Gap refs*: SD-GAP-021
    *Scope*: During download, enter connected standby. Resume. Verify download completes.
    *Key assertions*: Download succeeds, snapshot matches.

ID58. **Connected Standby → Resume During Local Write (P3)**
    *Gap refs*: SD-GAP-022
    *Scope*: After local write, connected standby. Resume. Close and verify upload on Device B.
    *Key assertions*: Write persists, upload completes.

ID59. **Regulatory Standby — All Phases (P3)**
    *Gap refs*: SD-GAP-023, SD-GAP-024, SD-GAP-025, SD-GAP-026
    *Scope*: Console in energy-saving mode (regulatory standby). Test shutdown/reboot cycle during init, download, local write, and upload. Verify recovery on each.
    *Why*: Regulatory standby is a full shutdown — distinct from connected standby. Tests cold-boot recovery.
    *Key assertions*: Clean recovery from cold boot, no corruption, sync completes.

### J. Title Termination / Eviction

ID60. **Title Termination During Init (P1)**
    *Gap refs*: SD-GAP-011
    *Scope*: During init/sync dialog, force-terminate the process. Wait 30 seconds. Relaunch. Verify sync restarts cleanly with no corruption.
    *Why*: Users and PLM can kill the title at any time; the GRTS provider must leave no corrupt state.
    *Key assertions*: No partial/corrupt containers on relaunch, sync completes fresh, no orphaned active-device lock.

ID61. **Title Termination During Download (P1)**
    *Gap refs*: SD-GAP-012
    *Scope*: During a cloud download, force-terminate. Relaunch. Verify download restarts cleanly.
    *Key assertions*: No partial files, clean restart, snapshot matches after completion.

ID62. **Title Termination During Local Write (P1)**
    *Gap refs*: SD-GAP-013
    *Scope*: Immediately after `WriteGameSaveData`, force-terminate. Relaunch and verify state. Close app and verify background upload on Device B.
    *Key assertions*: Either the write persisted and uploads, or it cleanly rolled back. No corruption.

ID63. **Title Switching During Init (P2)**
    *Gap refs*: SD-GAP-049
    *Scope*: During init/contention dialog, launch another title (evicting the game). Play for 2 minutes. Return via Quick Resume. Verify the app recovers and sync can complete.
    *Key assertions*: App recovers from eviction, contention dialog re-prompts, sync completes.

ID64. **Title Switching During Download (P2)**
    *Gap refs*: SD-GAP-050
    *Scope*: During download, launch another title. Return via Quick Resume. Verify download state.
    *Key assertions*: Either download resumes or restarts cleanly, no corruption.

ID65. **Title Switching During Local Write / Upload (P2)**
    *Gap refs*: SD-GAP-051
    *Scope*: After writing save data, launch another title. Return via Quick Resume. Verify save state. Close and verify background upload on Device B.
    *Key assertions*: Local write persists or rolls back cleanly, background upload completes if data was committed.

ID66. **Quick Resume Invalidation (P2)**
    *Gap refs*: SD-GAP-052
    *Scope*: Device A creates save, enters Quick Resume (launch another title). Device B modifies and uploads. Device A returns via Quick Resume. Verify Device A detects the cloud state change and triggers re-init/sync.
    *Why*: Quick Resume can leave the game with stale in-memory state; the SDK must detect this.
    *Key assertions*: Re-init triggers, cloud data from Device B is received, no stale data served.

### K. Sign-Out / User Management

ID67. **Sign-Out During Init (P1)**
    *Gap refs*: SD-GAP-031
    *Scope*: During init/sync, trigger user sign-out. Verify app handles it gracefully (error, no crash). Sign back in, retry, verify sync completes.
    *Key assertions*: Graceful error handling, no crash, re-sign-in works, data intact.

ID68. **Sign-Out During Download (P1)**
    *Gap refs*: SD-GAP-032
    *Scope*: During download, trigger sign-out. Verify download aborts gracefully. Sign in and retry.
    *Key assertions*: Clean abort, no partial data, retry succeeds.

ID69. **Sign-Out During Local Write (P1)**
    *Gap refs*: SD-GAP-033
    *Scope*: During/after local write, trigger sign-out. Verify graceful handling. Sign back in and verify data state.
    *Key assertions*: No crash, data state deterministic, background upload status correct.

ID70. **User Switch Same Device (P2)**
    *Gap refs*: SD-GAP-048
    *Scope*: Sign in as User A, create save. Switch to User B without closing app. Verify User A data is inaccessible. Create User B save. Switch back and verify isolation.
    *Key assertions*: Complete data isolation between users, no cross-user data bleed, each user's data intact after switch.

### L. Auth / SPOP / Entity Auth

ID71. **Auth/SPOP Expiry During Init (P1)**
    *Gap refs*: SD-GAP-034
    *Scope*: Device 1 is in init/contention dialog. Device 2 launches the same title with the same user (triggering SPOP). Verify Device 1 handles the auth revocation gracefully.
    *Why*: SPOP is Xbox's single-point-of-presence enforcement; auth can be pulled at any time.
    *Key assertions*: Device 1 closes or restarts without data loss, Device 2 syncs correctly, no orphaned locks.

ID72. **Auth/SPOP Expiry During Download (P1)**
    *Gap refs*: SD-GAP-035
    *Scope*: Device 1 is mid-download. Device 2 launches (SPOP). Verify Device 1 handles auth loss during transfer.
    *Key assertions*: No partial/corrupt data, clean recovery on Device 1 relaunch.

ID73. **Auth/SPOP Expiry During Local Write (P1)**
    *Gap refs*: SD-GAP-036
    *Scope*: Device 1 just wrote data. Device 2 launches (SPOP). Verify Device 1 handles auth loss gracefully.
    *Key assertions*: Local write state is deterministic, no data bleed to wrong session.

ID74. **Auth/SPOP Expiry During Out-of-Game Upload (P1)**
    *Gap refs*: SD-GAP-037
    *Scope*: Device 1 writes and closes (background upload in progress). Device 2 launches immediately. Verify upload status and data integrity.
    *Key assertions*: Either upload completed before SPOP or was cleanly abandoned, Device 2 data is coherent.

ID75. **Entity Auth — Golden Path with UI Callbacks (P1)**
    *Scope*: Initialize PFGameSaveFiles using PF entity ID + entity token (via `PFEntityGetEntityKey` / `PFEntityGetEntityTokenAsync`) instead of relying on automatic Xbox Live auth. Write, upload, close, relaunch, verify download with entity auth. Uses custom UI callbacks.
    *Why*: The SDK auto-negotiates entity auth but we have zero explicit coverage of this code path. Games using cross-platform entity IDs depend on this.
    *Key assertions*: Init succeeds with entity auth, upload/download round-trip works, snapshot matches, correct auth headers sent to GRTS.

ID76. **Entity Auth — Two-Device Sync (P1)**
    *Scope*: Device A uses entity auth to write and upload. Device B uses entity auth to download. Verify cross-device sync works identically to Xbox Live auth path.
    *Key assertions*: Snapshot comparison passes, contention auto-response works with entity auth, no auth-related failures.

ID77. **Entity Auth — Expired/Invalid Token Handling (P1)**
    *Scope*: Initialize with entity auth using a stale or invalid entity token. Verify the SDK returns `E_GS_PLAYFAB_LOGIN_FAILURE` (0x80832300) and the sync failure callback fires with the correct error. Retry with a valid token.
    *Key assertions*: Correct HRESULT returned, sync failure callback fires, retry with valid token succeeds, no crash or hang.

### M. Rollback

ID78. **Rollback to Last Conflict (P1)**
    *Gap refs*: SD-GAP-044
    *Scope*: Create a conflict, resolve it (choose local). Then relaunch with `RollbackToLastConflict` flag. Verify the discarded cloud branch is restored.
    *Why*: Validates rollback through the GRTS provider — the rollback flag must reach the service correctly through the out-of-proc path.
    *Key assertions*: Rollback restores the losing conflict branch, snapshot matches expected content, no side effects on unaffected data.

ID79. **Rollback to Last Known Good (P1)**
    *Gap refs*: SD-GAP-045
    *Scope*: Create a baseline, upload, modify, upload again. Relaunch with `RollbackToLastKnownGood`. Verify baseline is restored.
    *Key assertions*: Rollback targets the correct version, snapshot matches baseline, promotion rules applied correctly.

### N. Multi-File / Large Data / Network

ID80. **Network Flapping During Sync (P2)**
    *Gap refs*: SD-GAP-047
    *Scope*: During a download, rapidly toggle network connectivity (3 seconds off, 3 seconds on, repeat 3–5 times). Verify sync eventually succeeds or fails cleanly with a retry option.
    *Key assertions*: No hang, no corruption, either succeeds or provides clean retry path.

ID81. **Multi-File Sync Across Devices (P2)**
    *Gap refs*: MD-GAP-002, MD-GAP-003, MD-GAP-004, MD-GAP-005
    *Scope*: Device A creates multiple save files/folders via `WriteGameSaveData`. Upload. Device B syncs and verifies all files present. Device B modifies, uploads. Device A confirms. Test both clean local (Device B has no prior data) and stale local (Device B has older data).
    *Key assertions*: All files propagate, no partial state, modifications apply consistently, stale local data is overwritten.

ID82. **File/Folder Deletion Propagation Across Devices (P2)**
    *Gap refs*: MD-GAP-014
    *Scope*: Device A creates multiple save items. Device B syncs. Device A deletes a specific item and uploads. Device B syncs. Verify deleted item is gone, remaining items intact.
    *Key assertions*: Deletion propagates, no stray remnants, unrelated data untouched.

ID83. **Large Dataset Sync — 100 MB+ (P2)**
    *Gap refs*: MD-GAP-015
    *Scope*: Device A creates 100 MB+ of save data. Upload. Device B syncs with clean local. Verify progress dialog shows meaningful progress (not frozen), download completes within timeout, all data intact.
    *Key assertions*: No timeout/stall, progress callbacks fire, snapshot matches, no corruption.

### O. Rate Limiting / Upload Stress

ID84. **Rapid Upload Rate Limit (P2)**
    *Scope*: Upload save data more than 10 times per minute (the documented limit is 100 requests per 2 minutes, ~10 uploads/min). Verify the system handles rate limiting gracefully — either the sync failure callback fires with a rate-limit error, or the upload queues internally.
    *Why*: Documented in `limits.md`: "Title players are limited to 100 service endpoint requests in any 2-minute period."
    *Key assertions*: No crash, rate limit error surfaces via sync failure callback or HRESULT, data eventually uploads after backoff, no data loss.

ID85. **Upload Sync Failure — Rate Limit During UploadWithUiAsync (P2)**
    *Scope*: Trigger a sync failure specifically during `PFGameSaveFilesUploadWithUiAsync` (not `AddUserWithUiAsync`). Rapidly upload to hit rate limits. Verify the SyncFailed callback fires during the upload operation. Test Retry and Cancel responses.
    *Why*: All our sync failure tests are during init (AddUserWithUiAsync). The docs explicitly state SyncFailed can fire during both AddUser and Upload operations.
    *Key assertions*: SyncFailed callback fires during upload, Retry works, Cancel returns correct HRESULT, data state coherent.

### P. GRTS API Edge Cases

ID86. **Custom File Location — Init and Sync (P2)**
    *Scope*: Set `PFGameSaveInitArgs.saveFolder` to a non-default path during `PFGameSaveFilesInitialize`. Write, upload, close, relaunch with the same custom path. Verify data persists at the custom location and syncs correctly across devices.
    *Why*: Custom file location is exposed in the SDK and used by some platforms (e.g., Steam Deck). Needs validation through the GRTS provider.
    *Key assertions*: Save files written to custom path, snapshot comparison passes, sync works across devices.

ID87. **Custom File Location — Invalid Path (P2)**
    *Scope*: Set `PFGameSaveInitArgs.saveFolder` to an invalid/inaccessible path. Verify `PFXGameSaveInitializeConfig` returns `E_GS_PLAYFAB_INVALID_LOCATION` (0x80832305). Retry with a valid path.
    *Key assertions*: Correct HRESULT returned, no crash, retry with valid path succeeds.

ID88. **ResetCloud — E_NOTIMPL Verification (P2)**
    *Scope*: After a successful init and upload, call `PFGameSaveFilesResetCloudAsync`. Verify it returns `E_NOTIMPL` on GRTS. Document the alternative approach (upload empty state).
    *Why*: ResetCloud is explicitly not implemented on GRTS. Tests should verify the error rather than silently assuming it.
    *Key assertions*: `PFGameSaveFilesResetCloudAsync` returns `E_NOTIMPL`, no side effects on existing data.

ID89. **GRTS Error Codes — Specific HRESULT Validation (P2)**
    *Scope*: Trigger and verify specific GRTS error codes:
    - `E_GS_PLAYFAB_LOGIN_FAILURE` (0x80832300): Init with invalid credentials.
    - `E_GS_PLAYFAB_SYNC_FAILURE` (0x80832302): Network disconnect during sync.
    - `E_GS_PLAYFAB_INVALID_LOCATION` (0x80832305): Invalid file location.
    - `E_PF_GAMESAVE_USER_CANCELLED` (0x800704c7): Cancel via progress response.
    - `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` (0x89237004): Offline operations after disconnect.
    *Key assertions*: Each triggered error returns the documented HRESULT, callback receives matching error, no crash.

### Q. Power State

ID90. **Structured Shutdown During Init (P3)**
    *Gap refs*: SD-GAP-014
    *Scope*: During init/contention dialog, perform a structured shutdown. Power on, relaunch. Verify sync restarts cleanly.
    *Key assertions*: No corruption, contention re-prompts, sync completes.

ID91. **Structured Shutdown During Download (P3)**
    *Gap refs*: SD-GAP-015
    *Scope*: During download, perform structured shutdown. Reboot, relaunch. Verify download restarts.
    *Key assertions*: Clean restart, snapshot matches after completion.

ID92. **Structured Shutdown During Local Write (P3)**
    *Gap refs*: SD-GAP-016
    *Scope*: After local write, structured shutdown. Reboot, relaunch, verify state. Close and verify upload on Device B.
    *Key assertions*: Data either persisted and uploads, or cleanly rolled back.

ID93. **Structured Shutdown During Upload Window (P3)**
    *Gap refs*: 53973372
    *Scope*: Close app (triggering background upload), then immediately perform structured shutdown before upload completes. Power on and verify whether the upload completed or can be retried.
    *Key assertions*: Cloud state is coherent, Device B receives data or gets clean first-launch.

ID94. **Power Loss During Init (P3)**
    *Gap refs*: SD-GAP-017
    *Scope*: During init, pull power. Reconnect, boot, relaunch. Verify no corruption.
    *Key assertions*: Sync completes cleanly, no corrupt containers, no orphaned locks.

ID95. **Power Loss During Download (P3)**
    *Gap refs*: SD-GAP-018
    *Scope*: During download, pull power. Recover. Verify download completes on retry.
    *Key assertions*: No partial files, clean recovery.

ID96. **Power Loss During Local Write (P3)**
    *Gap refs*: SD-GAP-019
    *Scope*: After writing, pull power. Recover, relaunch, verify state.
    *Key assertions*: Data is either intact or cleanly rolled back.

ID97. **Power Loss During Upload Window (P3)**
    *Gap refs*: 53973301
    *Scope*: Close app (background upload pending), pull power before completion. Recover and verify cloud state.
    *Key assertions*: Cloud manifest is coherent (either upload finalized or rolled back).

### R. Bug Regressions

ID98. **Bug Regression — Delete Everywhere While Running (P3)**
    *Gap refs*: BUG-REG-03 (Bug 61136195)
    *Scope*: Launch Forte/test app on console. Create save. While game is still running, navigate to dashboard → Manage game → Saved data → Delete Everywhere. Wait for eviction. Relaunch. Verify fresh start. Launch on Device B and verify no orphaned lock or old cloud data.
    *Key assertions*: Local and cloud data deleted, no orphaned device lock, Device B gets clean first-launch.

ID99. **Bug Regression — Same-Device Contention After Crash (P3)**
    *Gap refs*: BUG-REG-04 (Bug 61456073)
    *Scope*: Create save. Force-crash the process. Wait 30 seconds. Relaunch on the same device. If contention dialog appears (same device conflicting with itself), select "Stop syncing and continue." Verify `AddUser` completes within 2 minutes. Repeat crash cycle twice more.
    *Key assertions*: No indefinite hang at `AddUser`, contention clears on subsequent launch, save data intact.

ID100. **Bug Regression — Suspend Online → Go Offline → Resume Hang (P3)**
    *Gap refs*: BUG-REG-05 (Bug 61557570)
    *Scope*: Launch on console, sync. Suspend. While suspended, go offline. Resume. Select "Use Offline" if prompted. Verify game resumes within 60 seconds with no hang.
    *Key assertions*: No hang at AddUser, save data accessible, eventual upload after reconnect.

ID101. **Bug Regression — Uninitialize During OS Termination (P3)**
    *Gap refs*: BUG-REG-07 (Bug 61444916)
    *Scope*: Trigger a large save write, immediately initiate quit, then quickly evict the game while uninitialize is still running. Wait 2 minutes. Relaunch. Verify no HANG_QUIESCE.
    *Key assertions*: No indefinite hang, no crash, data either intact or cleanly rolled back.

ID102. **Bug Regression — TCUI Dialog Stuck After Eviction Mid-Sync (P3)**
    *Gap refs*: BUG-REG-08 (Bug 61173408)
    *Scope*: Go offline. Launch (sync failure dialog appears). Evict the game by launching another title. Verify no stuck/flickering TCUI remains. Go online, relaunch, verify clean sync.
    *Key assertions*: No residual TCUI after eviction, no rapid-fire dialog loops, clean sync on relaunch.

### S. Soak / Stress

ID103. **Long-Running Sync Soak — Xbox (P3)**
    *Scope*: Loop the two-device golden path on Xbox for several hours with randomized payload sizes (using chaos mode parameters). Monitor for memory leaks, handle exhaustion, manifest drift, and GRTS service stability.
    *Key assertions*: Memory/handle usage stays bounded, manifests remain consistent, sync duration stable.


### T. Multi-User Xbox

ID104. **Two Users Active Simultaneously — Data Isolation (P1)**
    *Scope*: Sign in User A AND User B on the same console (both via `XUserAddAsync`). Call `PFGameSaveFilesAddUserWithUiAsync` for User A — write data (pattern `0xAA`), upload. Without uninitializing User A, call `PFGameSaveFilesAddUserWithUiAsync` for User B — write data (pattern `0xBB`), upload. Verify both users have completely independent save folders, manifests, and cloud state. Download each user's data and compare snapshots independently.
    *Why*: Xbox supports up to 16 simultaneous signed-in users with a fixed slot array. ID70 tests sequential switching (sign out A then sign in B). This tests both active at once — the realistic "family console" scenario.
    *Key assertions*: Both AddUser calls succeed, save folders are distinct paths, neither user sees the other's data, independent upload/download works, no cross-user data bleed, no unexpected callbacks fire for the other user.

ID105. **Second User — No Cross-User Contention (P1)**
    *Scope*: User A holds the active device lock (`KeepDeviceActive`). User B calls `PFGameSaveFilesAddUserWithUiAsync` on the same console. Verify User B gets its own independent session with NO contention — contention is per-user, not per-device. User A's active device changed callback must NOT fire.
    *Why*: A naive implementation might issue contention per-console instead of per-user. User B starting a game session must never interfere with User A's cloud lock.
    *Key assertions*: User B's AddUser succeeds immediately (no contention callback), User A's active device changed callback does NOT fire, both users can read/write/upload independently.

ID106. **User Switch During Active Upload (P2)**
    *Scope*: User A writes data and starts `PFGameSaveFilesUploadWithUiAsync`. While the upload progress callback is firing (mid-upload), trigger `xbuser signout` for User A and `xbuser signin` for User B. Verify the upload either completes or fails cleanly. Sign User A back in and verify data state.
    *Why*: ID70 tests user switch at idle. This tests the dangerous timing window: switching users during an active sync operation.
    *Key assertions*: No crash or hang, upload either completes or returns clean error, User A's data deterministic on re-sign-in, no orphaned active device locks.

ID107. **Multi-User Multi-Device — Cross-User Isolation Under Contention (P2)**
    *Scope*: Console 1 has User A and User B both signed in and initialized. Console 2 has User A signed in. User A on Console 1 uploads. User A on Console 2 syncs — standard contention flow (SyncLastSavedData). Verify User B on Console 1 is completely unaffected by User A's cross-device contention. User B's session, callbacks, and data must be untouched.
    *Why*: Multi-user + multi-device is the realistic living-room scenario. User B must never observe User A's contention or experience any side effects.
    *Key assertions*: User A contention resolves normally across consoles, User B receives zero callbacks, User B's save data unchanged, no cross-user state leakage.

### U. Cross-Platform — Xbox GRTS and In-Proc

ID108. **Cross-Platform Golden Path — In-Proc to GRTS Bidirectional Sync (P1)**
    *Scope*: Phase 1: Device A (PC, in-proc provider) uploads a 10 KB payload via `PFGameSaveFilesUploadWithUiAsync`. Device B (Xbox, GRTS provider) launches with the same PlayFab user, downloads, and verifies via snapshot comparison. Phase 2: Device B (Xbox) modifies the data, uploads with `ReleaseDeviceAsActive`. Device A (PC) re-syncs and verifies snapshot match.
    *Why*: The fundamental cross-platform promise — same user's cloud saves sync between PC (in-proc) and Xbox (GRTS). Zero explicit test coverage exists for cross-provider sync. Different providers serialize manifests and upload data differently; this validates end-to-end compatibility.
    *Key assertions*: Snapshot hashes match in both directions, contention auto-response works across providers, no format incompatibility errors, `IsConnectedToCloud` returns `true` on both.

ID109. **Cross-Platform Active Device Contention (P1)**
    *Scope*: Device A (Xbox, GRTS) holds the active device lock (`KeepDeviceActive`). Device B (PC, in-proc) launches and calls `AddUserWithUiAsync`. Verify contention callback fires on Device B with correct metadata (including Device A's `deviceType` showing Xbox). Device B responds with `SyncLastSavedData`. Verify Device A's active device changed callback fires. Then reverse: Device B (PC) holds lock, Device A (Xbox) requests.
    *Why*: Contention between GRTS and in-proc exercises cloud-side lock arbitration across provider boundaries. The lock is managed server-side but each provider acquires/releases differently.
    *Key assertions*: Contention fires correctly in both directions, `deviceType` in descriptor reflects the actual platform, resolution works, data integrity after cross-provider handoff.

ID110. **Cross-Platform Conflict Resolution (P1)**
    *Scope*: Device A (Xbox, GRTS) uploads baseline. Device B (PC, in-proc) creates divergent data in the same atomic unit while offline. Device B reconnects and syncs — conflict callback fires. Resolve with `UseLocal` on Device B (PC wins). Verify Device A (Xbox) receives the PC-resolved state on next sync. Repeat the scenario with `UseCloud` (Xbox wins).
    *Why*: Conflict resolution across providers is higher risk — the cloud manifest must be interpretable by both GRTS and in-proc for the winning and losing branches. Rollback metadata must also be cross-provider compatible.
    *Key assertions*: Conflict detected correctly across providers, `UseLocal`/`UseCloud` both work, resolved data readable by both providers, descriptor fields correct for each side.

ID111. **GRTS Background Upload then In-Proc Download (P1)**
    *Scope*: Device A (Xbox, GRTS) writes data, closes the app (triggering GRTS out-of-process background upload). Wait 2-5 minutes for GRTS to complete the upload. Device B (PC, in-proc) launches and downloads. Verify the background-uploaded data arrives intact via snapshot comparison.
    *Why*: GRTS's unique background upload continues after the game process exits — the upload is handled by the OS service. The resulting cloud state must be consumable by an in-proc device that does not use GRTS. This is the cross-platform GRTS differentiator test.
    *Key assertions*: Background upload completes (no game process running), in-proc download succeeds, snapshot match, no contention (Device A released lock on close).

ID112. **Cross-Platform Rollback Compatibility (P2)**
    *Scope*: Create a conflict between Xbox (GRTS) and PC (in-proc), resolve it (PC chooses `UseLocal`). On the Xbox (losing side), relaunch with `RollbackToLastConflict`. Verify the rollback restores the Xbox's discarded branch correctly, even though the conflict was resolved by the in-proc provider.
    *Why*: Rollback metadata (winner/loser branch markers) must be cross-provider compatible. If the in-proc provider writes the conflict resolution and the GRTS provider reads the rollback metadata, it must work.
    *Key assertions*: Rollback succeeds, restored data matches Xbox's pre-conflict state, no format errors from cross-provider metadata.

### Priority Band Summary

| Priority | Count | Description |
|----------|-------|-------------|
| P0 | 3 | Foundational guardrails — block merges on failure |
| P1 | 52 | High-risk GRTS scenarios — nightly or per-commit |
| P2 | 39 | Depth and scale — pre-release coverage |
| P3 | 18 | Resilience, power states, soak — weekly or milestone |
| **Total** | **112** | |

## Automation Actions Requiring Investigation

The following actions are referenced by scenarios above but have unknown or unverified automation feasibility on Xbox dev kits. Each needs a spike to determine the tooling approach. **Do not discard any scenario based on these gaps** — document findings and revisit.

### Process Lifecycle Control

| Action | Description | Possible Approaches | Status |
|--------|-------------|---------------------|--------|
| **[SUSPEND]** | Trigger PLM suspend on Xbox dev kit | Xbox Device Portal REST API (`POST /api/taskmanager/app`?), `xbrun` commands, launch Settings app (causes suspend after ~10 min), or programmatic PLM notification via test harness | Needs investigation |
| **[RESUME]** | Resume from PLM suspend | Press Xbox button on controller (needs HID automation or xbrun), Xbox Device Portal API | Needs investigation |
| **[TERMINATE]** | Force-kill the game process | Xbox Device Portal → Running Apps → Terminate (`DELETE /api/taskmanager/app`), `xbrun /terminate`, or `PFGameSaveFilesUninitializeAsync` + exit | Needs investigation |
| **[EVICT-GAME]** | Evict the running game by launching another title | `xbrun /launch` a different title, Xbox Device Portal launch API, or have the test controller deploy and launch a stub title | Needs investigation |
| **Quick Resume** | Invalidate Quick Resume state | Evict via another title launch → modify cloud state on Device B → return to original title | Needs investigation (return-to-title mechanism) |

### Power State Control

| Action | Description | Possible Approaches | Status |
|--------|-------------|---------------------|--------|
| **[SHUTDOWN]** | Structured shutdown | `xbrun /shutdown`, Xbox Device Portal API, or remote PowerShell | Needs investigation |
| **[POWER-PULL]** | Sudden power loss | PDU (Power Distribution Unit) with network control (e.g., APC switched rack PDU with SNMP/HTTP API), or smart plug with automation API | Needs investigation — requires lab hardware |
| **[STANDBY]** | Connected standby (instant-on) | Xbox Device Portal, console power management API, or `xbrun` | Needs investigation |
| **Regulatory Standby** | Energy-saving mode shutdown | Requires console power options preset to "energy saving" + structured shutdown | Needs investigation (prerequisite configuration) |
| **Reboot After Power Event** | Boot console and relaunch | PDU power-on, Xbox Device Portal boot detection, `xbrun /launch` after boot | Needs investigation |

### Network Control

| Action | Description | Possible Approaches | Status |
|--------|-------------|---------------------|--------|
| **[DISCONNECT]** | Disable network on console | Managed network switch port disable (SNMP/SSH), Xbox console network settings API, dev kit network management tool, or `xbconfig NetworkEnabled=false` | Needs investigation |
| **[RECONNECT]** | Re-enable network | Reverse of disconnect | Needs investigation |
| **Network Flapping** | Rapid on/off toggle | Managed switch scripting with timed port toggles | Needs investigation — likely requires network hardware |

### Storage Control

| Action | Description | Possible Approaches | Status |
|--------|-------------|---------------------|--------|
| **[FILL-STORAGE]** | Fill console storage | `ConsumeDiskSpace` harness command (already exists in GameTestAppShared), or `xbcp` to copy large files to dev kit | **Likely feasible** — verify ConsumeDiskSpace works on Xbox |
| **[FREE-STORAGE]** | Free console storage | Harness command to delete filler files (complement to ConsumeDiskSpace) | **Likely feasible** — verify existing implementation |
| **[DELETE-LOCAL]** | Delete local save data | `DeleteSaveRoot` harness command (already exists), or dashboard navigation | **Feasible** — already in harness |

### User / Auth Control

| Action | Description | Possible Approaches | Status |
|--------|-------------|---------------------|--------|
| **[SIGN-OUT]** | Sign out current Xbox user | `XUserCloseHandle` in harness, Xbox Device Portal user management, or Guide automation | Needs investigation |
| **[SIGN-IN]** | Sign in Xbox user | `XUserAddAsync` in harness (already exists), but may need UI interaction for profile selection | Partially feasible — harness has `XUserAddAsync` |
| **[SWITCH-USER]** | Switch gamertag without closing app | Sign-out + sign-in sequence via harness commands, or Xbox Device Portal user switching | Needs investigation |
| **SPOP Trigger** | Force auth expiry by signing in elsewhere | Launch same title on Device 2 with same user — this is orchestratable via the controller | **Feasible** — multi-device controller handles this |

### Dashboard / System Actions

| Action | Description | Possible Approaches | Status |
|--------|-------------|---------------------|--------|
| **[DELETE-EVERYWHERE]** | Dashboard → Manage game → Delete Everywhere | Xbox Device Portal API, `xbrun` with dashboard automation, or dedicated harness command that calls `XGameSaveDeleteContainerAsync` + service-side cleanup | Needs investigation |
| **Title Launch (for eviction)** | Launch a different installed title | `xbrun /launch <AUMID>`, Xbox Device Portal launch API | Needs investigation |

### Foreground / Background Control

| Action | Description | Possible Approaches | Status |
|--------|-------------|---------------------|--------|
| **Move to Background** | Send app to background | Launch Guide or another app (causes game to lose focus), Xbox Device Portal, or `xbrun /launch` a lightweight app | Needs investigation |
| **Return to Foreground** | Bring app back to foreground | `xbrun /switch` back to the test app, Xbox Device Portal, or navigate via Guide | Needs investigation |

## Additional Manual-Only Gaps

These tests require human judgment, visual verification, or physical interaction that automation cannot reliably replace.

- **TCUI Visual Validation**: Verify conflict, contention, storage, and progress dialogs render correctly — correct text, timestamps, thumbnails, device names, button labels. Refer to `page3-UXSharedValidationChecklist.txt` for the full validation matrix.
- **Accessibility Sweeps**: Screen reader narration, contrast ratios, focus order, and controller navigation across TCUI dialogs.
- **Localization Fidelity**: Dialog text matches locale, no truncation, correct date/time formats.
- **Controller vs. Keyboard Input Parity**: Verify D-pad + A/B navigation works identically to keyboard Enter/Escape on TCUI dialogs.
- **Progress Dialog UX**: Verify progress bar updates smoothly, shows meaningful byte counts, does not freeze for large downloads.
- **Bug Regression BUG-REG-06** (OS Downgrade → EntityId Change): Requires flashing dev kits to different OS builds — too infrastructure-heavy for standard automation; keep as manual dev-kit test.

## Test Automation Framework Features

To execute the Xbox/GRTS scenario backlog, the framework needs these capabilities beyond what the in-proc harness already provides:

- **Xbox Dev Kit Deployment Pipeline**: Automated build, package (`makepkg`), deploy (`xbcp` or `xbrun /deploy`), and launch (`xbrun /launch`) for GameTestAppXbox to Scarlett and Xbox One dev kits.
- **Xbox Live Auth Automation**: `XUserAddAsync` + `PFLocalUserCreateHandleWithXboxUser` flow with pre-configured test accounts. Handle SPOP constraints when multiple devices use the same account.
- **Console Lifecycle Commands**: New harness commands (or Xbox Device Portal REST wrappers) for suspend, resume, terminate, and evict. These extend the GameTestAppShared command surface.
- **Power State Control Integration**: Interface to managed PDU or smart power hardware for power-pull and cold-boot scenarios. Boot detection to know when the console is ready for `xbrun /launch`.
- **Network Fault Injection**: Integration with managed network switch (port enable/disable/flap) or dev kit network configuration API. Must support timed toggles for flapping tests.
- **Storage Pressure Tooling**: Verify `ConsumeDiskSpace` and its inverse work on Xbox persistent storage. May need Xbox-specific path resolution for the `PersistentLocalStorage` partition.
- **Multi-Device Coordination on Dev Kits**: Extend the `GameTestController` to manage multiple Xbox dev kits simultaneously, including IP-based WebSocket connections (update `controllerip.txt` per device or use command-line `/controller` argument).
- **Background Upload Wait and Verify**: A controller-level "wait-and-verify" command that waits a configurable period (2–5 minutes) after app close, then launches Device B to check cloud state. This is the key GRTS-specific assertion pattern.
- **Snapshot Comparison on Xbox**: Verify `CaptureSaveContainerSnapshot` and `CompareSaveContainerSnapshots` work correctly on Xbox persistent storage paths.
- **HRESULT and Telemetry Collection**: Centralized logging from multiple dev kits, capturing HRESULTs, callback sequences, and timing for each scenario step.

## Scenario Details

Step-by-step execution notes for each scenario, organized by section. Common init/cleanup/reset blocks are in the shared file.

| File | Section | Scenarios |
|------|---------|----------|
| [`_common-blocks.md`](details/_common-blocks.md) | Common Xbox Init / Cleanup / Reset | — |
| [`A-golden-path.md`](details/A-golden-path.md) | A. Golden Path / Smoke | ID1–ID4 |
| [`B-active-device-contention.md`](details/B-active-device-contention.md) | B. Active Device Contention | ID5–ID13 |
| [`C-conflict-resolution.md`](details/C-conflict-resolution.md) | C. Conflict Resolution | ID14–ID24 |
| [`D-sync-failure-offline.md`](details/D-sync-failure-offline.md) | D. Sync Failure / Offline | ID25–ID31 |
| [`E-progress-cancel-mid-sync.md`](details/E-progress-cancel-mid-sync.md) | E. Progress, Cancel, and Mid-Sync Actions | ID32–ID36 |
| [`F-ui-callback-data.md`](details/F-ui-callback-data.md) | F. UI Callback Data Verification | ID37–ID43 |
| [`G-out-of-storage.md`](details/G-out-of-storage.md) | G. Out of Storage | ID44–ID49 |
| [`H-foreground-background.md`](details/H-foreground-background.md) | H. Foreground / Background | ID50–ID52 |
| [`I-suspend-resume.md`](details/I-suspend-resume.md) | I. Suspend / Resume | ID53–ID59 |
| [`J-title-termination-eviction.md`](details/J-title-termination-eviction.md) | J. Title Termination / Eviction | ID60–ID66 |
| [`K-sign-out-user-management.md`](details/K-sign-out-user-management.md) | K. Sign-Out / User Management | ID67–ID70 |
| [`L-auth-spop-entity.md`](details/L-auth-spop-entity.md) | L. Auth / SPOP / Entity Auth | ID71–ID77 |
| [`M-rollback.md`](details/M-rollback.md) | M. Rollback | ID78–ID79 |
| [`N-multi-file-large-data.md`](details/N-multi-file-large-data.md) | N. Multi-File / Large Data / Network | ID80–ID83 |
| [`O-rate-limiting.md`](details/O-rate-limiting.md) | O. Rate Limiting / Upload Stress | ID84–ID85 |
| [`P-grts-edge-cases.md`](details/P-grts-edge-cases.md) | P. GRTS API Edge Cases | ID86–ID89 |
| [`Q-power-state.md`](details/Q-power-state.md) | Q. Power State | ID90–ID97 |
| [`R-bug-regressions.md`](details/R-bug-regressions.md) | R. Bug Regressions | ID98–ID102 |
| [`S-soak-stress.md`](details/S-soak-stress.md) | S. Soak / Stress | ID103 |
| [`T-multi-user-xbox.md`](details/T-multi-user-xbox.md) | T. Multi-User Xbox | ID104–ID107 |
| [`U-cross-platform.md`](details/U-cross-platform.md) | U. Cross-Platform — Xbox GRTS and In-Proc | ID108–ID112 |


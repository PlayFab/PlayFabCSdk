# PFGameSave Test Automation Plan

## Purpose
Provide a concise framework for selecting the next PFGameSave automation investments and capture the philosophy that will guide those choices. The goal is to keep the team aligned on why certain scenarios rise to the top and ensure the upcoming backlog is easy to justify without digging through other references.

## Section map
- **Selection Philosophy** defines the decision filters.
- **Priority Bands** frame urgency.
- **Test Scenario List** queues the recommended automation backlog.
- **Additional Noteworthy Test Automation Gaps** calls out still-missing coverage.
- **Additional Noteworthy Manual Test Gaps** catalogs human-only sweeps.
- **Test Automation Framework Features** outlines the harness capabilities needed to run and expand the backlog.
- **Scenario Details** spell out execution notes for each numbered case.

## Selection Philosophy
- **Player-Critical Risk First**: Start with scenarios where a defect could silently delete or regress player progress—multi-device merges, rollback flags, active device arbitration, reconnect flows, or anything else that can strand a save file. These are the tests that protect trust.
- **Deterministic and Assertable**: Focus on flows that yield clear, machine-verifiable outcomes such as specific HRESULTs, manifest state transitions, hash comparisons, or callback ordering. Deterministic assertions keep the lab reliable and avoid flake work.
- **Guard High-Churn Code**: Track components that change frequently—Steam Deck pipeline, offline reconnection, conflict handling, active device change handling, rollback options—and add automation that will act as a safety net for future commits.
- **Maximise Platform Reach**: Select tests that illuminate platform-specific divergences (in-process versus out-of-process behaviour, Steam Deck in-process sync, Xbox background uploads, Windows MSIXVC flows). Each run should inform confidence across the supported matrix.
- **Reuse Existing Harnesses**: Build on the current multi-device controller/device tooling so new scenarios inherit coordination, telemetry, and fault injection. Reusing infrastructure keeps authoring cost low and results comparable.
- **Operational ROI**: Target scenarios that can run in per-commit and nightly pipelines without heavy babysitting. Reserve manual test time for subjective UX checks and let automation carry the repeatable, objective validation.

## Priority Bands
- **P0 – Foundational Guardrails**: Must-run smoke that proves players can sign in, sync, and share saves without data loss. Failures stop merges until resolved.
- **P1 – High-Risk Accelerators**: Volatile or high-impact flows (conflicts, rollback, active device changes, platform churn) that run per-commit or nightly to catch regressions fast.
- **P2 – Depth and Scale**: Broader coverage for quotas, corruption, payload scale, and integration guardrails. Results guide targeted fixes before release.
- **P3 – Resilience and Soak**: Long-running and outage drills that validate recovery and stability; scheduled less frequently but inform earlier bands.

## Test Scenario List
Assuming no automation coverage exists yet, start with the fundamentals that prove the happy paths work every time. Each scenario below is phrased as an automation blueprint: what it exercises, the minimum platform set to run on, and the key assertions that keep regressions from sneaking in. Items appear in recommended execution order and align with the priority bands called out above so planning can map directly to investment tiers.


1. **Single-Device Golden Path Sync Across Platforms (Priority P0)**  
	*Scope*: Initialize the library, call `AddUserWithUiAsync`, write a small payload (e.g., 10 KB file), trigger `UploadWithUiAsync`, terminate the process, clear the local container, relaunch, and confirm the payload downloads intact. Run the same flow on each supported runtime configuration (Xbox Gen8/Gen9, Windows MSIXVC, Steam PC, Steam Deck) to ensure parity.  
	*Why first*: This offers the fastest signal that the core API chain still works, and immediately exposes platform-specific divergences without authoring new logic. Every subsequent test depends on this staying green.  
	*Key assertions*: All API calls return success, the save folder path is stable, uploaded file hash matches after relaunch, no unexpected callbacks fire, and platform-specific init succeeds without falling back to offline mode.

2. **Two-Device Golden Path Sync (Priority P0)**  
	*Scope*: Device A performs the same golden path as above and publishes a simple payload. Device B signs into the same account, `AddUserWithUiAsync` completes without conflict, the payload arrives intact, and B can write a new version that A later receives.  
	*Why next*: Validates multi-device cloud propagation and basic manifest handling without introducing contention.  
	*Key assertions*: Hash match on both sides, synchronous state progression through the controller, and proof that only changed files transfer.

3. **Basic Offline-to-Online Reconnect (Priority P1)**  
	*Scope*: Use LHC mocks to force `AddUserWithUiAsync` into retryable failures, respond first with `Retry`, then with `UseOffline`, confirm `PFGameSaveFilesIsConnectedToCloud` transitions to offline, capture offline upload return codes plus quota API behaviour while disconnected, create a local payload, swap the mocks to succeed, run `AddUserWithUiAsync` again, and ensure the offline progress pushes to the cloud.  
	*Why*: Validates offline mode code paths and protects players who start without connectivity.  
	*Key assertions*: Correct HRESULTs while offline, `PFGameSaveFilesIsConnectedToCloud` reflects state changes, quota surface responds deterministically while disconnected, the offline upload reports the documented deferred code, persistent local save, and successful upload once online.

4. **Active Device Release and Reacquire (Priority P1)**  
	*Scope*: Device A acquires the active lock and stays active. Device B attempts `AddUserWithUiAsync`, hits the active device contention UI callback, selects `SyncLastSavedData` to take control, then uploads with `ReleaseDeviceAsActive`. Finally, Device A re-syncs to confirm it can regain the lock without stale state.  
	*Why*: Validates the standard hand-off flow, including contention handling, before testing more aggressive multi-device arbitration paths.  
	*Key assertions*: Device B receives the contention callback and sends `PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData`, Device A observes the active device changed callback and transitions to offline mode, B’s upload completes with the release flag, and a subsequent `AddUserWithUiAsync` on A succeeds immediately afterward with up-to-date manifest state.

5. **Large Cloud Dataset Initialization (Priority P1)**  
	*Scope*: Call `PFGameSaveFilesAddUserWithUiAsync` for an account with ~100 MB of existing cloud data spread across ~1000 files, allow the full download to complete, and ensure the manifest hydrates locally without UI stalls.  
	*Why*: Players returning to a rich save are high-value; a failure here silently blocks access to progress. Validating the bulk download path keeps the onboarding experience trustworthy.  
	*Key assertions*: API completes within expected time, progress callbacks fire in order, the hydrated file set (count + hashes) matches a pre-recorded manifest snapshot, and `PFGameSaveFilesIsConnectedToCloud` remains true after completion.

6. **Conflict Resolution – Local Wins (Priority P1)**  
	*Scope*: Device A writes `slotA` and uploads; Device B modifies the same slot offline, comes online, hits a conflict, chooses “Use Local,” and confirms the manifest reflects B’s data while untouched slots remain unchanged.  
	*Why*: Introduces deliberate conflict handling without complex folder structures, ensuring the conflict UI path and atomic-unit decisions work before layering bigger cases.  
	*Key assertions*: Conflict callback fires once, chosen branch wins, no unrelated folders change, and post-upload hashes align with the expected choice.

7. **Conflict Resolution – Cloud Wins (Priority P1)**  
	*Scope*: Mirror the prior scenario but select “Use Cloud” during the conflict dialog, then verify the local offline changes are discarded while untouched slots survive.  
	*Why*: Confirms both conflict branches behave deterministically and prevents regressions that strand stale local data after choosing the cloud state.  
	*Key assertions*: Conflict UI offers deterministic choices, post-sync manifest matches cloud version, and discarded local files are removed.

8. **Multi-Atomic Conflict Decision Application (Priority P1 — Recommended #6)**  
	*Scope*: Populate at least three root subfolders (`slotA`, `slotB`, `settings`). Cause divergent edits on Devices A and B so `slotA` and `slotB` both conflict while `settings` does not. On the conflict callback, choose `TakeLocal` and assert both conflicting folders resolve to Device B’s data and the untouched folder still syncs down from Device A. Repeat with `TakeRemote` to prove the decision applies atomically across all conflicted units.  
	*Why*: The service applies a single decision per conflict pass. A regression that only resolves one folder would silently drop data. This scenario is higher priority than the soak and complements the single-folder conflict tests.  
	*Key assertions*: Conflict callback enumerates all conflicting folders in one decision, the selected action applies to each folder consistently, unaffected folders merge normally, and logs show a single conflict resolution event.

9. **Rollback to Last Known Good Recovery (Priority P1)**  
	*Scope*: On Device A, upload a non-empty baseline manifest (version N), rehydrate it, then mutate the payload to produce a `GOOD_PATCH` version N+1. After rehydrating N+1 so it becomes the active baseline, upload an intentionally bad successor (version N+2) with the release option so the promotion rules mark N+1—not the original baseline—as the last known good manifest. On Device B, call `PFGameSaveFilesAddUserWithUiAsync` twice—first normally to observe it receives N+2, then a second time supplying `PFGameSaveFilesAddUserOptions` with `rollbackOption = PFGameSaveFilesRollbackOption::RollbackToLastKnownGood`—and verify the rollback call restores the `GOOD_PATCH` snapshot without manual intervention.  
	*Why*: Titles rely on the last-known-good flag to recover from bad payloads; a regression here strands corrupted saves across the fleet.  
	*Key assertions*: The rollback-enabled `AddUser` call succeeds (or is explicitly gated via title configuration), manifest metadata shows version N+1 flagged `IsKnownGood = true` while the baseline N remains non-destructive, local file hashes match the `GOOD_PATCH` content after rollback, and logs record the rollback path including any title-configuration gating.  

10. **Rollback to Last Conflict Recovery (Priority P1)**  
	*Scope*: Reuse the conflict flow from Scenario 6 to produce a preserved conflict loser (Device B picks the wrong branch and uploads). On Device A, call `PFGameSaveFilesAddUserWithUiAsync` again with `PFGameSaveFilesAddUserOptions.rollbackOption = PFGameSaveFilesRollbackOption::RollbackToLastConflict` and confirm the previously losing branch is restored when the service configuration allows that flag. Include a gated-title variant to validate the API surfaces the appropriate denial.  
	*Why*: `RollbackToLastConflict` lets support staff undo incorrect conflict decisions; verifying the flow prevents permanent data loss after a mistaken choice.  
	*Key assertions*: The rollback call either succeeds and restores the losing branch’s manifest/files or returns the documented gating HRESULT, unaffected folders remain intact, and logs distinguish the rollback invocation from normal sync.

11. **Upload Interruption Recovery (Priority P1)**  
	*Scope*: Kick off `PFGameSaveFilesUploadWithUiAsync`, then terminate the title or sever connectivity between the `InitiateUpload` and `FinalizeManifest` calls (use LHC mocks or crash injection) so the operation never finalizes. Relaunch the title, rerun `PFGameSaveFilesAddUserWithUiAsync`, and confirm the cloud manifest still reflects the last committed version before retrying the upload successfully.  
	*Why*: Protects against silent corruption if an upload crashes mid-flight; partial manifests are a player-critical risk and belong ahead of several P2 items.  
	*Key assertions*: No new manifest version appears after the interrupted upload, orphaned blobs and any staged zip archives from the prior attempt are removed, the retry completes without needing manual cleanup, and logs record the interrupted attempt.

12. **Out-of-Storage UI Handling (Priority P1)**  
	*Scope*: Fill the local device’s persistent storage (or mock disk failures) so `PFGameSaveFilesUploadWithUiAsync` triggers the out-of-storage UI callback. Exercise both responses—`Retry` after freeing space and `UseOffline` without freeing space—to ensure the SDK surfaces the correct HRESULTs, maintains offline progress, and resumes once capacity returns.  
	*Why*: Running out of disk is common and high impact; players risk losing progress unless the UI path behaves deterministically. This should sit alongside the other P1 device/UX guardrails.  
	*Key assertions*: Out-of-storage callback fires exactly once per failure, selected response produces the documented HRESULT, offline mode preserves pending writes, and a subsequent upload after space is reclaimed succeeds without manual repair.

13. **User-Canceled Sync/Upload Recovery (Priority P1)**  
	*Scope*: Register custom UI callbacks, start a long-running `PFGameSaveFilesAddUserWithUiAsync` download and respond to the progress callback with `PFGameSaveFilesSetUiProgressResponse(..., Cancel)` mid-transfer, verifying the async returns `E_PF_GAMESAVE_USER_CANCELLED`, no partial manifest is written locally, and a subsequent retry resumes cleanly. Repeat for `PFGameSaveFilesUploadWithUiAsync`: begin an upload, cancel via progress, confirm the cloud manifest version is unchanged, then reissue the upload and let it succeed. Include at least one sync-failed path that surfaces the failure dialog and exercise the `Cancel` response to ensure offline progress rights remain intact.  
	*Why*: Players regularly bail out of long syncs; regressions here can strand partial files or orphan manifests, silently losing progress. Guarding both download and upload cancel paths is more critical than several P2 quota/scale scenarios.  
	*Key assertions*: Cancel callbacks produce `E_PF_GAMESAVE_USER_CANCELLED`, local/remote manifests remain on the previous version, temporary files are cleaned up, offline mode stays available where applicable, and logs record the cancellation without follow-up errors.

14. **Concurrent Upload Arbitration (Priority P1)**  
	*Scope*: Devices A and B both attempt to upload without releasing the active device. Intentionally overlap uploads to exercise arbitration—drive the contention callback on the losing device, respond with `PFGameSaveFilesSetUiActiveDeviceContentionResponse` (first `Retry`, then `SyncLastSavedData`)—ensuring one upload succeeds and the other receives the expected failure path, then retries successfully after acquiring the lock.  
	*Why*: Active device contention is a high-risk path for progress loss; automation keeps the arbitration contract honest.  
	*Key assertions*: Losing upload surfaces `E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE`, the contention callback fires exactly once with the expected response path, and a subsequent retry after `SyncLastSavedData` completes without manual cleanup.

15. **Mid-Operation Disk Exhaustion Recovery (Priority P1)**  
	*Scope*: Begin a large upload or download, then exhaust local disk space mid-transfer (e.g., background filler process or API to shrink provisioned storage). Ensure the out-of-storage callback fires during the active operation, the async returns `E_PF_GAMESAVE_DISK_FULL`, and cleanup removes any partial blobs. After freeing space, retry the operation and confirm it resumes from scratch without stale temp files. Cover both upload and download directions.  
	*Why*: Real world storage exhaustion often happens while data is streaming, not before starting. Without deterministic failure and recovery, titles can corrupt saves. This protection outranks low-priority soak coverage.  
	*Key assertions*: Mid-transfer exhaustion triggers the callback exactly once, partial artifacts are removed, the retry succeeds after freeing space, and `PFGameSaveFilesIsConnectedToCloud` still reports accurate status.

16. **Large Payload Incremental Sync (Priority P2)**  
	*Scope*: Upload a payload near (but under) the default 256 MB quota—e.g., ~220 MB split across multiple files—change only a subset of files, and confirm subsequent uploads transfer just the diffs while downloads reconstruct the full payload. Titles that use larger quotas can additionally validate at their configured ceiling.  
	*Why*: Stresses quota-aware chunking and ensures incremental sync logic remains efficient even when only some files are touched between uploads.  
	*Key assertions*: Initial upload succeeds without hitting quota, diff upload skips unchanged chunks (verified via debug apis and local hash comparison), and the reconstructed payload hashes match the expected post-diff state.

17. **Quota Limit Handling (Priority P2)**  
	*Scope*: Attempt to exceed configured storage or bandwidth quotas deliberately—by expanding the root folder beyond limits or provisioning a too-small container—and validate the API surfaces the correct error path, then demonstrate recovery once the payload is trimmed.  
	*Why*: Protects against silent data loss when titles mis-size payloads or platforms enforce tight storage envelopes.  
	*Key assertions*: Deterministic quota HRESULTs, no partial writes, and the next compliant upload succeeds without manual intervention.

18. **High File Count Upload (Priority P2)**  
	*Scope*: Starting from an empty cloud state, upload ~1 MB of data split across ~1000 files, then repeat with partial updates touching only a subset of files.  
	*Why*: Explores metadata scale limits and uncovers regressions in directory enumeration or diff computation when file counts spike.  
	*Key assertions*: Upload completes without timeout, manifest accurately lists all files, and subsequent incremental upload transfers only touched files.

19. **Filesystem Boundary Validation Across Platforms (Priority P2)**  
	*Scope*: Populate the save root with edge-case paths—reserved filenames, deeply nested directories, 260+ character paths (where allowed), mixed Unicode normalization, and a manifest containing several thousand files—then upload, download, and diff the results across Windows MSIXVC, Steam PC, Steam Deck, and Xbox GDK.  
	*Why*: Validates that path normalization, manifest enumeration, and compression logic remain stable at platform-specific limits while preventing silent truncation or omitted entries.  
	*Key assertions*: Upload succeeds without truncating or rejecting legal paths, downloads recreate the exact hierarchy and filenames, Unicode normalization remains stable, and manifest size/count never silently caps entries.

20. **Manifest Corruption Detection (Priority P2)**  
	*Scope*: Inject a malformed manifest or corrupt download on disk before a sync, ensuring validation rejects the payload and performs a clean recovery.  
	*Why*: Defends against disk or network corruption that could otherwise cascade into data loss.  
	*Key assertions*: Validation catches corruption, produces the expected error code, and a subsequent clean sync restores a valid manifest.

21. **Interrupted Download Resume (Priority P2)**  
	*Scope*: Begin a multi-file download, drop connectivity mid-transfer, and verify that the next sync resumes or restarts cleanly without leaving partial files.  
	*Why*: Network instability is common; confirming deterministic recovery prevents half-written saves that strand players.  
	*Key assertions*: Download interruption detected, resume path produces identical hashes, and no orphaned temp files remain.

22. **File/Folder Deletion Propagation (Priority P2)**  
	*Scope*: Device A deletes a payload folder or file and uploads; Device B syncs and proves the deletion occurs locally while unrelated content stays intact. Include a third device that remained offline to ensure the deletion still applies once it reconnects.  
	*Why*: Deletions are irreversible player-facing operations; automation keeps the contract consistent across devices.  
	*Key assertions*: Deleted content removed everywhere, manifest version increments correctly, and no stray payload remnants persist on disk.

23. **Mutating During Upload Guard (Priority P2)**  
	*Scope*: Kick off `PFGameSaveFilesUploadWithUiAsync` and have the title attempt to write additional files into the save root while the upload is running, ensuring those writes are blocked or deferred safely.  
	*Why*: Prevents race conditions where developers accidentally mutate the save during transfer, leading to corruption.  
	*Key assertions*: Upload surfaces the expected error or ignores late writes, no partial payload is committed, and a retry with clean sequencing succeeds.

24. **Online-to-Offline Recovery (Priority P3)**  
	*Scope*: While already connected, force server calls to fail with retryable errors, validate the title transitions to offline mode gracefully, then recover when availability returns.  
	*Why*: Ensures resilient behaviour during transient outages and guards trust when PlayFab endpoints wobble or fail over.  
	*Key assertions*: Expected error surfaces, offline mode engages without crashing, and a subsequent successful sync restores cloud parity.

25. **Long-Running Sync Soak (Priority P3)**  
	*Scope*: Loop the two-device golden path for hours with randomized payload sizes and periodic restarts to expose resource leaks or cumulative manifest drift.  
	*Why*: Captures churn-induced regressions that single-pass tests miss, especially around telemetry buffers and file handle cleanup.  
	*Key assertions*: Memory and handle usage stay bounded, manifests remain identical across devices, and sync duration stays within expected thresholds.

## Additional Noteworthy Test Automation Gaps
Beyond the numbered scenario list above, these are additional targeted flows we still lack in automated coverage and that deserve harness investment as the core backlog stabilises.

- Platform-driven account linking/unlinking while offline, ensuring manifests stay consistent once reconnected.  
	*Why*: Offline identity churn is error-prone and repetitive; automation guarantees we catch regression when platform shells break link/unlink flows.
- Server-initiated cloud manifest rollback (support tool workflows) and associated telemetry hooks.  
	*Why*: Ensures tooling-driven rollbacks keep manifests and telemetry in sync without manual oversight, reducing high-severity recovery bugs.
- Token refresh and alternate identity flows to ensure 401/reauthentication paths persist progress without user-visible failures.  
	*Why*: Automated coverage catches subtle auth expiry regressions that only appear under timed refresh windows.
- Compression and manifest boundary coverage for payloads above 64 MB, chunk reuse, extended schema validation, and rollback behaviour when archive construction fails mid-flight (beyond Scenario 16’s incremental diff focus).  
	*Why*: Large payload regressions surface only under scale; automation lets us stress compression logic reliably across runs.
- Service throttling and rate-limit response handling when repeated sync/upload bursts exceed PlayFab limits.  
	*Why*: Automated stress loops reproduce throttling consistently to ensure SDK backs off gracefully without manual babysitting.
- Active-device stale-lock recovery, timeout-based auto release, and reliable active-device-changed callbacks after unexpected terminations or publisher-side takeovers.  
	*Why*: These flows hinge on precise timing; automation validates we release locks correctly without relying on manual race reproduction.
- Resource pressure resilience across low-memory budgets, handle exhaustion, callback exceptions, and cleanup after failure or queue starvation.  
	*Why*: Automated fault injection keeps the runtime hardened against leak regressions that are tedious to discover manually.
- System clock skew and time-based reconciliation behaviour to ensure large clock jumps do not corrupt sync state.  
	*Why*: Automation can simulate skew deterministically, something manual testing cannot do at scale.
- Network interface switching, partial connectivity, high latency, and reconnection drifts across offline/online transitions.  
	*Why*: Scripted network chaos validates reconnection logic objectively, preventing flake-prone manual sweeps.
- Power-state transitions including suspend, connected standby, app termination, and background uploads to ensure pending manifests finalize or roll back safely.  
	*Why*: Automated harnesses can hammer suspend/resume sequences repeatedly to flush timing bugs.
- SPOP alignment validation so console-managed background sync never conflicts with PlayFab active-device ownership or rollback expectations.  
	*Why*: Automation is the only scalable way to verify the interplay between platform SPOP agents and PlayFab locks.
- `PFGameSaveFilesResetCloudAsync` coverage to ensure cloud resets leave a consistent manifest and handle state.  
	*Why*: Reset operations must be deterministic; automated assertions prove the service never strands partial state.
- Uninitialize cleanup to prevent stale handles or leaked state.  
	*Why*: Closing sequences are easy to overlook manually; automation enforces leak-free teardown every run.
- Multi-user and multi-profile session boundaries, PFLocalUser persistence, and wrong-account guardrails preventing data bleed.  
	*Why*: Automated multi-profile permutations uncover data crossover issues faster than manual spot checks.
- Save descriptor, thumbnail, and metadata parity including Unicode, boundary lengths, and UI exposure.  
	*Why*: Automation can iterate through boundary inputs to ensure metadata parity without human fatigue.
- Custom UI pathways, dispatcher guarantees, user-response timing, XAsync cancellation, and invalid callback usage handling across stock versus custom experiences.  
	*Why*: Automated harnesses validate dispatcher guarantees and callback ordering with millisecond precision.
- Complex atomic-unit boundaries, mixed rename/delete permutations, and cross-unit dependencies that extend beyond Scenario 8’s single-decision validation.  
	*Why*: Automation can explore large combinatorial sets of atomic unit changes that are infeasible manually.
- Guard and error-code validation for initialization order, re-entry, concurrent API calls, download/upload overlap, and AddUser retry paths.  
	*Why*: Automated guard-rail tests stop subtle contract violations from slipping into production.
- Offline mode API surfaces, deferred upload continuations, and long-running offline reconnect expectations that exceed Scenario 3’s single reconnect coverage.  
	*Why*: Repeated offline loops in automation ensure deferred uploads survive lengthy outages.
- Local and remote corruption injection beyond Scenario 20’s manifest validation, including archive tampering and recovery UX coverage for streaming blobs.  
	*Why*: Automated corruption injection confirms recovery logic catches every tampered payload variant.
- Folder size accuracy, background queue customization, and shutdown robustness to ensure telemetry and operations respect caller-provided dispatchers.  
	*Why*: Automation can systematically vary queue settings and shutdown sequences to guard against regressions.
- Extended manifest serialization, skip-flag handling, empty-folder recreation, and forward-compat versioning so schema changes never orphan payloads.  
	*Why*: Automated schema validation keeps future versioning safe without manual diffing.
- Local state metadata persistence that tracks last-sync file size/timestamps and survives crashes to flag unsent changes accurately.  
	*Why*: Crash-loop automation proves metadata persists correctly after repeated failures.
- Manifest compaction heuristics and zip-reuse thresholds to prevent unbounded archive growth while respecting bandwidth trade-offs.  
	*Why*: Automated long-run tests measure compaction effectiveness, impossible to maintain manually.
- Cross-runtime identity handoff for out-of-proc uploads when titles authenticate with Custom ID or OpenID providers.  
	*Why*: Automation exercises the full identity matrix to ensure background uploads succeed regardless of auth provider.

## Additional Noteworthy Manual Test Gaps
In addition to the automation backlog above, these are human-only checks where qualitative judgement, visual fidelity, or policy compliance prevents us from relying on automation, and no current automated scenario covers them.

- UI-first regression sweeps covering accessibility (narration, contrast, focus order) across GDK shell dialogs, PlayFab custom UI, and Steam overlay variants.  
	*Why*: Automation cannot reliably judge screen-reader narration, contrast ratios, or focus order across storefront shells; manual sweeps keep us compliant with platform accessibility bars and avoid blocking cert.
- Long-form UX validation for sync conflict messaging, including localization fidelity, clarity of call-to-action text, and storefront-specific terminology alignment.  
	*Why*: Only human review catches confusing phrasing, mistranslated strings, or misleading button labels that would erode player trust during conflicts.
- Controller versus mouse/keyboard UX parity for progress dialogs, retry/cancel affordances, and background upload prompts.  
	*Why*: Input feel, focus handling, and haptic feedback require hands-on validation; automation cannot assert parity across control schemes.
- Visual confirmation of background upload indicators, toast notifications, and resume flows during power/suspend transitions across target hardware.  
	*Why*: These indicators are platform-rendered and timing sensitive, making them impractical to script; manual checks ensure players see accurate status and avoid duped uploads.
- End-to-end customer support tooling walkthroughs (support portal rollback, forced manifest reset) to confirm operator instructions remain accurate and intuitive.  
	*Why*: Support flows depend on multi-step human tooling and documentation; manual runbooks prove that partners can execute recovery without engineering help.
- Sync failure and offline fallback dialogs (Retry/Use Offline copy, iconography, focus order) across supported locales to confirm the messaging matches the offline support contract.  
	*Why*: Copy tone, iconography, and layout vary by locale and storefront; manual review ensures the UX guides players correctly when automation injects the error.
- Active-device contention prompts (Try Again vs Sync Last Saved Data) including post-upload release prompts to ensure instructions align with the spec and prevent accidental data loss.  
	*Why*: Determining whether the prompts communicate risk and respect user timing needs human judgment; automation only verifies the API path.
- Custom upload progress overlays on non-Steam 3PP platforms validating developer-supplied UX blocks the session appropriately, exposes accurate progress, and satisfies storefront-specific requirements.  
	*Why*: These overlays are title-authored and vary widely, so manual vetting ensures certification readiness and avoids regressions in bespoke UI code.
- Rollback menu surfacing for PFGameSaveFilesAddUserOptions::Rollback, including warnings, confirmation flows, and recovery education for players before destructive restoration.  
	*Why*: Manual review confirms the warnings set expectations, reduces support calls, and prevents accidental data loss when players initiate recovery.
- Cross-network identity linking guidance (OpenID handoff, XUser binding fallbacks) ensuring manual flows, error surfaces, and recovery instructions remain intuitive when tokens expire or linking fails.  
	*Why*: Identity edge cases hinge on visual guidance and support messaging that automation can’t grade; manual walkthroughs prove players can recover without churn.

## Test Automation Framework Features
To execute the scenario backlog and close the automation gaps, the framework should expose the following capabilities at the feature level:

- **Coordinated multi-device orchestration** to assign scenario roles, manage session lifecycle, and enforce keep-alives across Xbox, Windows, Steam, and other runtimes.
- **Pluggable identity/auth drivers** that automate sign-in, token refresh, and account handoff flows without hardcoding platform-specific stacks.
- **Deterministic payload tooling** for seeding save data, hashing results, snapshotting manifests, and diffing incremental uploads or deletions.
- **UI callback automation hooks** able to drive conflict, out-of-storage, sync-failed, and progress prompts with scripted responses and timing controls.
- **Fault and environment injection** covering network disruption, disk pressure, power-state transitions, and service throttling to validate recovery paths.
- **Service and storage mock integration** so HTTP and blob endpoints can be intercepted, delayed, or failed in a repeatable way during offline/rollback drills.
- **Centralized telemetry and log aggregation** that timestamps device events, captures HRESULTs, and packages evidence for pipeline triage.
- **Scenario scheduler with step-level assertions** enabling repeatable execution, branching, and early failure when guard rails trip.
- **Resource and state probes** to observe quota usage, handle counts, and active-device ownership without attaching debuggers to every run.
- **Extensible command surface** (CLI/API) that lets pipelines, engineers, or future tools kick off scenarios, configure permutations, and retrieve results headlessly.

## Scenario Details

### Common Account Reset
Run this reset flow before each scenario so the test account starts with a known-empty cloud manifest and local container.

1. Follow Scenario 1 steps 1–8 to initialize the runtime, authenticate, and add the user so the device holds the active lock.
2. Query the local save root via `PFGameSaveFilesGetFolder` (Scenario 1 step 9) and recursively delete its contents using the harness file helpers; this clears all local manifests, metadata files, and payload folders.
3. Call `PFGameSaveFilesResetCloudAsync` with the acquired `PFLocalUserHandle` and wait for the `PFGameSaveFilesResetCloudResult`; assert the HRESULT is success.
4. Immediately invoke `PFGameSaveFilesAddUserWithUiAsync` again to confirm the cloud now returns an empty manifest and that `PFGameSaveFilesIsConnectedToCloud` remains `true`.
5. Perform Scenario 1 step 13 cleanup (close handles, uninitialize, release the task queue) so subsequent scenario runs start from a cold state.

### 1. Single-Device Golden Path Sync Across Platforms
1. Run the Common Account Reset to clear the account, then `XGameRuntimeInitialize` (Windows/GDK) followed by `PFInitialize`
2. `PFServicesInitialize`
3. `PFServiceConfigCreateHandle`
4. `XTaskQueueCreate` for async dispatch (retain handle for reuse)
5. `PFGameSaveFilesInitialize`
6. (Optional on Windows, required on Steam Deck) `PFGameSaveFilesSetUiCallbacks` and `PFGameSaveFilesSetActiveDeviceChangedCallback` to hook progress, conflict, out-of-storage, and active-device notifications.
7. Obtain a `PFLocalUserHandle` using the platform path:  
	a. **GDK**: `PFLocalUserCreateHandleWithXboxUser` after `XUserAddAsync`.  
	b. **Steam**: `PFLocalUserCreateHandleWithSteamUser` after resolving Steam identity (`PFAuthenticationLoginWithSteamAsync`).  
	c. **Custom ID**: `PFLocalUserCreateHandleWithPersistedLocalId` (invokes `PFAuthenticationLoginWithCustomIDAsync`).
8. `PFGameSaveFilesAddUserWithUiAsync` → `PFGameSaveFilesAddUserWithUiResult`
9. Retrieve the save root via `PFGameSaveFilesGetFolderSize` → `PFGameSaveFilesGetFolder` (or direct fixed-buffer call); optionally query `PFGameSaveFilesGetRemainingQuota` for diagnostics.
10. Write `payload.bin` (~10 KB) inside the returned folder.
11. `PFGameSaveFilesUploadWithUiAsync`
12. Terminate, delete the local container folder (files plus metadata manifest), and relaunch the title; repeat steps 1–9 to pull the payload back down and compare hashes.
13. Cleanup: `PFLocalUserCloseHandle`, `PFGameSaveFilesUninitializeAsync` → `PFGameSaveFilesUninitializeResult`, `PFServicesUninitializeAsync`, `PFUninitializeAsync`, `PFServiceConfigCloseHandle`, `XTaskQueueCloseHandle`, and `XUserCloseHandle` (or Steam equivalent).

### 2. Two-Device Golden Path Sync
1. Run the Common Account Reset to clear the account, then on Device A perform Scenario 1 steps 1–11 to publish `payload.bin` and confirm the upload succeeded.
2. On Device A immediately call `PFGameSaveFilesUploadWithUiAsync` (or the equivalent upload path) with `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`, wait for completion, and then terminate the title so Device A relinquishes the active lock before Device B signs in.
3. On Device B, repeat Scenario 1 steps 1–8 to initialize, authenticate, and add the same user; confirm the download yields the hash from Device A’s payload.
4. On Device B, modify or replace `payload.bin`, execute Scenario 1 steps 10–11 to upload the new version, then immediately call `PFGameSaveFilesUploadWithUiAsync` (or the equivalent upload path) with `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`, wait for completion, and terminate the title so Device B releases the active lock.
5. Return to Device A and execute Scenario 1 steps 1, 8–11 to download Device B’s update and verify hashes match the latest payload.

### 3. Basic Offline-to-Online Reconnect
1. Run the Common Account Reset to clear the account, then perform Scenario 1 steps 1–7 to initialize the runtime, register UI callbacks, and obtain a `PFLocalUserHandle`; configure an `HttpMock` (see `Test/PlayFabServicesManualTestApp/TestFramework/HttpMock.cpp`) or equivalent LHC mock to intercept the `PFGameSaveFilesAddUserWithUiAsync` traffic—specifically `POST https://<titleId>.playfabapi.com/GameSave/ListManifests` and `POST https://<titleId>.playfabapi.com/GameSave/GetManifestDownloadDetails` (add `Client/LoginWithCustomID` if the scenario uses Custom ID auth`)—and force retryable connectivity failures (e.g., return HTTP 503 or `E_FAIL`) before invoking the API.
2. With the failure mocks active, call `PFGameSaveFilesAddUserWithUiAsync`; when the sync-failed callback fires, respond once with `PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::Retry)` and assert the operation surfaces the expected connectivity HRESULT (match sample logs) while the mock hit counters confirm the underlying PlayFab routes were intercepted.
3. Reissue `PFGameSaveFilesAddUserWithUiAsync` without clearing the failure mocks; respond with `PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::UseOffline)`, wait for completion, and verify the result flags offline mode while still granting access to the save root (Scenario 1 step 9) and that the mocks captured the second request.
4. Immediately query `PFGameSaveFilesIsConnectedToCloud` (expect `false`) and issue `PFGameSaveFilesGetRemainingQuota`; confirm the quota call returns the documented offline HRESULT and that both APIs continue to point at the same save root while the mocks remain in failure mode.
5. Create `payload_offline.bin` (or mutate existing content) under the save root per Scenario 1 step 10 and record its hash for later comparison.
6. Keep the failure mocks in place (extend them to `POST https://<titleId>.playfabapi.com/GameSave/InitiateUpload` and `POST https://<titleId>.playfabapi.com/GameSave/FinalizeManifest` as needed) and invoke `PFGameSaveFilesUploadWithUiAsync`; confirm the returned result indicates a deferred/offline upload with the expected offline HRESULT (follow the sample’s diagnostics or `PFGameSaveFilesUploadWithUiResult` fields), verify `PFGameSaveFilesIsConnectedToCloud` remains `false`, and ensure no real network call succeeds because the mocks consume the requests.
7. While still offline, reissue `PFGameSaveFilesGetRemainingQuota` and `PFGameSaveFilesGetFolderSize` to ensure they continue surfacing the offline status without corrupting local bookkeeping.
8. Switch the LHC mocks to success responses (e.g., update status to HTTP 200, clear `SetNetworkResult`, and ensure matching storage `PUT` mocks point to valid content) before calling `PFGameSaveFilesAddUserWithUiAsync` again; if a sync-failed prompt still appears, respond with `PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::Retry)`, and verify the call now succeeds while `PFGameSaveFilesIsConnectedToCloud` flips to `true` and the mocks log the successful transition.
9. Execute `PFGameSaveFilesUploadWithUiAsync` once more with the success mocks active—covering `GameSave/InitiateUpload`, `FinalizeManifest`, and any blob `PUT` URLs surfaced in the response—expect a successful upload that pushes the previously staged offline delta.
10. Terminate the title, clear the local container, relaunch, and repeat Scenario 1 steps 1–9 to download the save; confirm the hash of `payload_offline.bin` matches the offline version created in step 5, proving the reconnect path preserved progress.
11. Perform cleanup per Scenario 1 step 13 to release handles, close the task queue, and dispose of any active LHC mocks (including unregistering the specific PlayFab and blob URLs configured above).

### 4. Active Device Release and Reacquire
1. Run the Common Account Reset to clear the account, then on Device A execute Scenario 1 steps 1–10 to initialize, authenticate, obtain the save root, and stage a small payload while remaining in an active session.
2. Call `PFGameSaveFilesUploadWithUiAsync` on Device A without specifying `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`, confirm the upload succeeds, and keep the title running so the active lock stays with Device A.
3. Query `PFGameSaveFilesIsConnectedToCloud` on Device A (expect `true`) and keep the registered callbacks active so the upcoming contention notification can be observed.
4. On Device B, repeat Scenario 1 steps 1–7 to initialize the runtime, register UI callbacks (including `PFGameSaveFilesSetActiveDeviceChangedCallback`), and obtain a `PFLocalUserHandle` for the same account.
5. Invoke `PFGameSaveFilesAddUserWithUiAsync` on Device B; when the active-device contention UI callback fires, respond with `PFGameSaveFilesSetUiActiveDeviceContentionResponse(..., PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData)` and wait for the operation to complete.
6. Verify Device A received the active-device-changed callback, transitioned to offline mode (`PFGameSaveFilesIsConnectedToCloud` returns `false`), and persisted any pending local state without crashing.
7. Modify the payload on Device B (reuse Scenario 1 step 10) and call `PFGameSaveFilesUploadWithUiAsync` with `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`, ensuring the upload succeeds and the completion result indicates the active lock was released.
8. Terminate Device B or explicitly uninitialize per Scenario 1 step 13 so it cleanly exits the session after releasing the lock.
9. Back on Device A, rerun `PFGameSaveFilesAddUserWithUiAsync`; confirm the call succeeds immediately, the manifest reflects Device B’s latest payload, and `PFGameSaveFilesIsConnectedToCloud` returns `true`.
10. Perform cleanup on both devices following Scenario 1 step 13 to close handles, task queues, and any custom logging hooks.

### 5. Large Cloud Dataset Initialization
1. Run the Common Account Reset to clear the account, then on Device A (seeding device) execute Scenario 1 steps 1–11 to initialize, generate a deterministic payload totaling ~100 MB across ~1000 files (use the payload helper to create the file set), upload via `PFGameSaveFilesUploadWithUiAsync` while specifying `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`, and capture the resulting manifest snapshot (file list, sizes, hashes) for later verification before terminating to release the active lock.
2. On the target device (Device B), execute Scenario 1 steps 1–8 to initialize the runtime, authenticate with the same account, and invoke `PFGameSaveFilesAddUserWithUiAsync` against the seeded manifest.
3. Monitor progress callbacks during the bulk download, recording total transfer duration and ensuring no unexpected UI prompts (conflict, active-device contention) appear.
4. After the call completes, invoke `PFGameSaveFilesGetFolderSize`, `PFGameSaveFilesGetRemainingQuota`, and enumerate the save root to confirm the expected file count materialized locally without stalls or throttling.
5. Hash the downloaded files (reuse the payload tooling described earlier) and compare against the manifest snapshot from step 1 to validate parity across counts, hashes, and directory structure.
6. Query `PFGameSaveFilesIsConnectedToCloud` to confirm the device remained online throughout the download and that no fallback to offline mode occurred.
7. Trigger a second `PFGameSaveFilesAddUserWithUiAsync` immediately to verify the subsequent sync is incremental and finishes quickly, indicating caches and metadata hydrated correctly.
8. Upload a small mutation (Scenario 1 step 10 followed by step 11) to prove the large dataset does not block regular writes and confirm the upload completes without throttling or timeout.
9. Perform cleanup using Scenario 1 step 13 to close handles and dispose of any collected diagnostics artifacts.

### 6. Conflict Resolution – Local Wins
1. Run the Common Account Reset to clear the account, then on Device A execute Scenario 1 steps 1–11 while creating a deterministic folder structure (e.g., `slotA/character.sav`, `settings/options.json`) and upload with `PFGameSaveFilesUploadWithUiAsync` plus `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive` so the baseline propagates and the active lock is released.
2. On Device B, execute Scenario 1 steps 1–8 to initialize, register conflict UI callbacks, and call `PFGameSaveFilesAddUserWithUiAsync` to hydrate the same account, verifying the baseline folders arrive intact.
3. Still on Device B, duplicate the hydrated `slotA` folder for later comparison, then disconnect network traffic by enabling the LHC mock to return retryable failures for `GameSave/InitiateUpload` and `FinalizeManifest`; this keeps Device B in an offline-capable state while edits are staged.
4. Modify `slotA/character.sav` on Device B (e.g., bump a save timestamp or counter) to represent the player’s desired offline change and record its hash for post-conflict validation; avoid touching other folders so only `slotA` diverges.
5. On Device A, reinitialize per Scenario 1 steps 1–8, pull the latest manifest, mutate the same `slotA/character.sav` with different content, and upload via `PFGameSaveFilesUploadWithUiAsync` (no release option) to publish version N+1 while reclaiming the active lock; terminate afterward to free the lock.
6. Restore Device B’s connectivity mocks to success, then call `PFGameSaveFilesAddUserWithUiAsync`; when the conflict UI callback fires, respond with `PFGameSaveFilesSetUiConflictResolutionResponse(..., PFGameSaveFilesUiConflictResolutionUserAction::UseLocal)` ensuring the handler returns success.
7. Wait for the async to complete and assert the HRESULT matches the documented conflict-success path; verify the conflict callback provided the expected list of divergent folders (only `slotA`) and that no additional prompts appeared.
8. Enumerate the save root on Device B and compare hashes: `slotA` should match the offline edit from step 4, while unaffected folders (`settings`) should mirror Device A’s previously uploaded state. Confirm the manifest version increments and that the conflict resolution decision is captured in the scenario evidence.
9. Re-run `PFGameSaveFilesAddUserWithUiAsync` on Device A to confirm it immediately downloads Device B’s winning content and that subsequent diffs show only the `slotA` data changing.
10. Perform Scenario 1 step 13 cleanup on both devices and archive the conflict-handling logs for regression tracking.

### 7. Conflict Resolution – Cloud Wins
1. Run the Common Account Reset to clear the account, then repeat Scenario 6 steps 1–5 verbatim: seed the deterministic folder set on Device A, hydrate Device B, enable the LHC failure mocks, stage Device B’s offline edit (tag it with an unmistakable `LOCAL_ONLY` marker), and publish the conflicting cloud change from Device A so the manifest advances.
2. Restore normal network behaviour on Device B as described in Scenario 6 step 6, call `PFGameSaveFilesAddUserWithUiAsync`, and when the conflict callback triggers respond with `PFGameSaveFilesSetUiConflictResolutionResponse(..., PFGameSaveFilesUiConflictResolutionUserAction::UseCloud)` instead of `UseLocal`.
3. Follow Scenario 6 step 7 to verify the async completes with the conflict-resolved HRESULT and that the callback payload lists only the expected folders; confirm the `UseCloud` selection is reflected in the resulting manifest state.
4. Reuse Scenario 6 validation flow but invert the expectation: enumerate Device B’s save root and assert `slotA/character.sav` now matches Device A’s authoritative hash (the `LOCAL_ONLY` marker must be gone) while untouched folders such as `settings` remain unchanged.
5. Issue `PFGameSaveFilesAddUserWithUiAsync` on Device A (or a third device) to prove the cloud-resolved manifest propagates everywhere, then complete teardown per Scenario 6 step 10 and archive the hash comparisons showing the local branch was discarded.

### 8. Multi-Atomic Conflict Decision Application
1. Run the Common Account Reset to clear the account, then on Device A execute Scenario 6 step 1 but expand the deterministic payload to include three atomic units (for example `slotA/character.sav`, `slotB/companion.sav`, and `settings/options.json`). Upload with `PFGameSaveFilesUploadWithUiAsync` plus `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive` and record per-folder hashes for later comparison.
2. On Device B, execute Scenario 1 steps 1–8 to initialize, register the conflict callback, and hydrate the account with `PFGameSaveFilesAddUserWithUiAsync`; capture baseline hashes for each folder and duplicate them for diffing.
3. Keep Device B active and enable the LHC mocks (same endpoints as Scenario 6 step 3) so subsequent edits remain offline. Modify `slotA/character.sav` and `slotB/companion.sav` with distinct local content while leaving `settings/options.json` untouched; record hashes for both modified folders.
4. On Device A, mirror the dual-folder divergence by editing `slotA/character.sav` and `slotB/companion.sav` with alternative data (ensuring the changes differ from Device B’s edits) and upload via `PFGameSaveFilesUploadWithUiAsync` without the release flag so the service now holds conflicting versions for both atomic units; terminate afterward to release the lock.
5. Restore Device B’s network to normal, call `PFGameSaveFilesAddUserWithUiAsync`, and when the conflict callback fires ensure it enumerates both `slotA` and `slotB` in the same decision. Respond with `PFGameSaveFilesSetUiConflictResolutionResponse(..., PFGameSaveFilesUiConflictResolutionUserAction::UseLocal)` to select the local branch for all conflicts.
6. Await completion, confirm the HRESULT matches the conflict-success path, and verify a single conflict resolution covers both folders. Enumerate Device B’s save root: both `slotA` and `slotB` should now match the local hashes from step 3, while `settings/options.json` retains Device A’s unmodified baseline value.
7. Repeat steps 1–6 but invert the decision to `UseCloud` and confirm both conflicting folders resolve to Device A’s changes. Capture before/after hashes for each run to demonstrate the atomic decision applies consistently across all listed folders.
8. Perform Scenario 1 step 13 cleanup on all participating devices and archive the hash tables from both `UseLocal` and `UseCloud` passes to document consistent multi-unit behaviour.

### 9. Rollback to Last Known Good Recovery
1. Run the Common Account Reset to clear the account, then on Device A execute Scenario 1 steps 1–11 to seed a non-empty baseline payload (e.g., write `slotA/character.sav` with `BASELINE_START`). Upload with `PFGameSaveFilesUploadWithUiAsync` plus `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive` so version N is finalized and the active lock is released; record the manifest version and hash for later comparison.
2. Without resetting state, run Scenario 1 steps 1–8 again on Device A to rehydrate the freshly uploaded baseline and confirm the downloaded files match `BASELINE_START`; this sets version N as the active baseline for subsequent uploads.
3. Modify the payload on Device A with a deliberate but valid improvement (e.g., add a `GOOD_PATCH` marker inside `slotA/character.sav`) and call `PFGameSaveFilesUploadWithUiAsync` using `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive` to finalize version N+1. Capture the returned manifest version showing progression N → N+1; per the rollback spec promotion rules, note that version N (the previously active baseline) now transitions to `IsKnownGood = true` automatically.
4. Rehydrate the account on Device A by invoking `PFGameSaveFilesAddUserWithUiAsync` again with default options; assert the downloaded files now include the `GOOD_PATCH` content and record the manifest version (N+1). This call makes N+1 the new active baseline required for its own promotion on the next finalized successor.
5. Intentionally corrupt or regress the payload on Device A (e.g., overwrite `slotA/character.sav` with `CORRUPTED_PAYLOAD`) and upload again via `PFGameSaveFilesUploadWithUiAsync`, still supplying `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`, to produce version N+2. Verify the version history shows N+1 → N+2 progression and confirms that version N+1 is now automatically promoted to `IsKnownGood = true`, while version N remains a prior known-good anchor.
6. On Device B, execute Scenario 1 steps 1–8 to initialize and sync normally; confirm it receives the corrupted N+2 payload (hash matches the `CORRUPTED_PAYLOAD` content). Capture the manifest version to prove the bad data is currently live.
7. Inspect the `ListManifestsResponse.TitleConfiguration` exposed through diagnostics to ensure `DisableClientRollbackToLastKnownGood` is false (if true, document the gating behavior and expect rollback to be ignored). Configure a `PFGameSaveFilesAddUserOptions` struct with `rollbackOption = PFGameSaveFilesRollbackOption::RollbackToLastKnownGood`, then call `PFGameSaveFilesAddUserWithUiAsync` again on Device B using those options.
8. Assert the rollback-enabled call succeeds (or returns the documented gating HRESULT if disabled), and validate that the downloaded files now match the `GOOD_PATCH` hash set from step 4 while the `CORRUPTED_PAYLOAD` marker is absent. Record the manifest version reported in the result payload and confirm the rollback path targets version N+1.
9. Execute Scenario 1 step 13 cleanup on all participating devices and archive the before/after manifest metadata, including `IsKnownGood` status, for regression evidence.

### 10. Rollback to Last Conflict Recovery
1. Run the Common Account Reset to clear the account, then on Device A execute Scenario 6 step 1 to seed a deterministic payload (`slotA/character.sav = CLOUD_BASELINE`, `settings/options.json` untouched) and upload with `PFGameSaveFilesUploadWithUiAsync` plus `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`; capture manifest version N and hashes for later comparison.
2. On Device B, follow Scenario 6 steps 2–3 to hydrate the account, duplicate the hydrated files for diffing, enable the LHC failure mocks, and stage an offline edit in `slotA/character.sav` that clearly identifies the local branch (e.g., `LOCAL_BRANCH_WINNER`). Record the local hash and note that no new manifest is created yet.
3. On Device A, repeat Scenario 6 step 5: reinitialize, download the latest manifest (now version N), mutate `slotA/character.sav` with a distinct `CLOUD_BRANCH_LOSER` marker, and upload via `PFGameSaveFilesUploadWithUiAsync` without the release flag so the service now holds conflicting branches; terminate the title afterward to release the active lock. Confirm the upload produces version N+1.
4. Restore Device B’s connectivity mocks (Scenario 6 step 6) and call `PFGameSaveFilesAddUserWithUiAsync`; when the conflict callback fires, respond with `PFGameSaveFilesSetUiConflictResolutionResponse(..., PFGameSaveFilesUiConflictResolutionUserAction::UseLocal)` to keep the `LOCAL_BRANCH_WINNER` content. Wait for completion, assert the HRESULT indicates conflict success, and capture the returned manifest version (winner, N+2) noting that the opposing cloud branch is preserved as a conflict loser (`IsWinner = false`, `ConflictingVersion = N+2`).
5. Still on Device B, immediately enumerate the save root and persist hashes so you can distinguish winner versus loser content later. Optionally reissue `PFGameSaveFilesAddUserWithUiAsync` (default options) to confirm no further conflict fires and that the live manifest remains the winner content.
6. On Device A (fresh session) or a clean Device C, run Scenario 1 steps 1–8 to sync normally; verify the downloaded files match the `LOCAL_BRANCH_WINNER` data and record the manifest version (should align with N+2). This establishes the default behavior before invoking rollback.
7. Inspect the latest `ListManifestsResponse.TitleConfiguration` via diagnostics to confirm `DisableClientRollbackToLastConflictLoser` is false; if true, document the gating outcome and expect the rollback request to be ignored. Construct a `PFGameSaveFilesAddUserOptions` struct with `rollbackOption = PFGameSaveFilesRollbackOption::RollbackToLastConflict`, then call `PFGameSaveFilesAddUserWithUiAsync` again on the same device using those options.
8. Assert the rollback-enabled call succeeds (or surfaces the documented gating HRESULT) and that the hydrated files now match the `CLOUD_BRANCH_LOSER` content captured from Device A’s conflict upload. Verify via manifest metadata that the returned version corresponds to the preserved loser (version N+1) with `IsWinner = false` and `ConflictingVersion = N+2`, and that the rollback event is recorded in scenario evidence.
9. Execute Scenario 1 step 13 cleanup on all participating devices and archive the hash comparisons and manifest version history (N, N+1, N+2) showing selection of the conflict loser per the rollback spec.

### 11. Upload Interruption Recovery
1. Run the Common Account Reset to clear the account, then on Device A execute Scenario 1 steps 1–10 to initialize, obtain the save root, and stage a deterministic payload (e.g., `slotA/character.sav` plus a secondary file) sized large enough to observe partial transfers.
2. Register UI callbacks as needed (Scenario 1 step 6) and prepare LHC mocks or crash instrumentation capable of intercepting `GameSave/FinalizeManifest` responses and the associated blob `PUT` calls so the upload can be aborted between initiate and finalize.
3. Kick off `PFGameSaveFilesUploadWithUiAsync` with the staged payload, wait until the initiate response returns the upload session (or observe progress callbacks reaching at least one chunk), then trigger the interruption: either terminate the process immediately, forcibly tear down connectivity via the HTTP mock, or crash the task queue thread to ensure `FinalizeManifest` never executes. Do not release the active device lock via `ReleaseDeviceAsActive` inside this partial attempt.
4. Relaunch the title (or restart the harness), rerun Scenario 1 steps 1–8 to reinitialize and call `PFGameSaveFilesAddUserWithUiAsync`, and confirm the manifest version still matches the pre-interruption baseline (no new version, hashes equal the last committed state). Record that any partially staged upload artifacts were removed locally.
5. With connectivity restored, stage a fresh payload mutation (e.g., bump a counter inside `slotA/character.sav`) and call `PFGameSaveFilesUploadWithUiAsync` again; allow the operation to complete normally and, if desired, include `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive` so the lock is cleanly released after success.
6. Re-run `PFGameSaveFilesAddUserWithUiAsync` to verify the new payload downloads correctly, demonstrating that the system recovered from the interrupted upload without manual cleanup.
7. Execute Scenario 1 step 13 cleanup to close handles, release the task queue, and dispose of any remaining HTTP mocks or crash hooks.

### 12. Out-of-Storage UI Handling
1. Run the Common Account Reset to clear the account, then on Device A perform Scenario 1 steps 1–10 to initialize the runtime, register UI callbacks (ensure the out-of-storage handler is wired), and stage a baseline payload under the save root.
2. Use a harness helper to consume free disk space on the device (e.g., create a large temporary file outside the save root or exercise a debug API that mocks the internal file system) until available capacity is lower than the pending upload size; log the remaining space so the test can restore it later.
3. Invoke `PFGameSaveFilesUploadWithUiAsync` and, when the out-of-storage UI callback fires, respond with `PFGameSaveFilesSetUiOutOfStorageResponse(..., PFGameSaveFilesUiOutOfStorageUserAction::Retry)` after freeing enough disk space to satisfy the upload. Confirm the async resumes and completes successfully, and that the saved payload hashes match the expected state.
4. Stage a second payload mutation (e.g., append data to `slotA/character.sav`), re-consume disk space so the device is again below the required threshold, and start another `PFGameSaveFilesUploadWithUiAsync`.
5. When the out-of-storage callback fires for the second attempt, respond with `PFGameSaveFilesSetUiOutOfStorageResponse(..., PFGameSaveFilesUiOutOfStorageUserAction::Cancel)` without freeing disk space; capture the HRESULT returned by the async (`E_PF_GAMESAVE_USER_CANCELLED`) and confirm the manifest version is unchanged while the local delta remains staged for a future retry.
6. Release disk space back to normal, call `PFGameSaveFilesUploadWithUiAsync` again (optionally with `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`), and ensure the previously staged changes upload successfully once capacity is available. Confirm a subsequent `PFGameSaveFilesAddUserWithUiAsync` on a clean session pulls down the updated payload.
7. Delete any temporary filler files, verify disk space returns to baseline, and execute Scenario 1 step 13 cleanup to close handles and reset the environment.

### 13. User-Canceled Sync/Upload Recovery
1. Run the Common Account Reset to clear the account, then on Device A execute Scenario 1 steps 1–8 to initialize, register UI callbacks (including the progress handler), and hydrate an empty manifest.
2. Stage a payload large enough to exercise progress callbacks (e.g., multiple files totaling ~50 MB) and create a second device (Device B) or harness role reserved for download validation later.
3. Invoke `PFGameSaveFilesAddUserWithUiAsync` to start a download; once the progress callback reports a mid-transfer state (e.g., 40–60%), call `PFGameSaveFilesSetUiProgressResponse(..., PFGameSaveFilesUiProgressUserAction::Cancel)` to simulate the player canceling. Confirm the async returns `E_PF_GAMESAVE_USER_CANCELLED`, that no partial manifest is materialized under the save root, and that `PFGameSaveFilesIsConnectedToCloud` remains true.
4. Immediately reissue `PFGameSaveFilesAddUserWithUiAsync` without canceling to prove the download resumes cleanly and the payload can be hydrated end-to-end (compare hashes against the staged content).
5. Modify the payload locally, kick off `PFGameSaveFilesUploadWithUiAsync`, and when the progress callback reaches mid-transfer call `PFGameSaveFilesSetUiProgressResponse(..., PFGameSaveFilesUiProgressUserAction::Cancel)` again. Verify the async returns `E_PF_GAMESAVE_USER_CANCELLED`, the cloud manifest version is unchanged, and the local staged changes persist for retry.
6. Retry `PFGameSaveFilesUploadWithUiAsync` without canceling; allow it to complete successfully, optionally releasing the active device lock. Then run `PFGameSaveFilesAddUserWithUiAsync` on Device B to confirm the new payload propagates correctly.
7. Execute Scenario 1 step 13 cleanup on all participating devices, removing any temporary payloads and ensuring callbacks are deregistered.

### 14. Concurrent Upload Arbitration
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–11 to seed a baseline payload and upload using `PFGameSaveFilesUploadOption::KeepDeviceActive` so the manifest is published while Device A explicitly retains the active lock for subsequent contention steps.
2. Without tearing down, keep Device A initialized: run Scenario 1 steps 1–8 again to rehydrate the same account, ensure the active-device changed callback remains registered, and stay online so Device A now holds the active lock.
3. On Device B run Scenario 1 steps 1–7 to initialize, register the active-device contention callback, and call `PFGameSaveFilesAddUserWithUiAsync`. When the contention prompt fires, respond with `PFGameSaveFilesSetUiActiveDeviceContentionResponse(..., PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry)` and assert the async fails with `E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE`, proving uploads are blocked while Device A holds the lock.
4. Stage a new payload delta on Device A (e.g., tag `slotA/character.sav` with `A_LOCKED_UPLOAD`), configure an HTTP mock or throttling hook to delay `GameSave/FinalizeManifest` and blob `PUT` calls, and start `PFGameSaveFilesUploadWithUiAsync` using `PFGameSaveFilesUploadOption::KeepDeviceActive` so the title stays active while the upload is in flight.
5. While Device A’s upload is pending, have Device B call `PFGameSaveFilesAddUserWithUiAsync` again; this time respond to the contention callback with `PFGameSaveFilesSetUiActiveDeviceContentionResponse(..., PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData)`. Verify the call succeeds, Device B becomes the active device, Device A receives the active-device changed callback, and `PFGameSaveFilesIsConnectedToCloud` on Device A flips to `false`. Once the HTTP mock releases, assert Device A’s in-flight upload fails with `E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE` because it lost the lock mid-transfer.
6. With Device B now active, stage a distinct payload delta (e.g., write `B_ACTIVE_UPLOAD` into `slotA/character.sav`) and call `PFGameSaveFilesUploadWithUiAsync` without the release option. Confirm the upload completes successfully, the manifest advances, and Device B retains the lock.
7. Attempt `PFGameSaveFilesUploadWithUiAsync` on Device A while still inactive; expect the operation to fail immediately with `E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE`, demonstrating writes are blocked when the lock is owned elsewhere.
8. Invoke `PFGameSaveFilesAddUserWithUiAsync` on Device A, respond to the contention callback with `SyncLastSavedData`, and confirm the call succeeds, transferring the lock back to Device A and delivering the active-device changed callback to Device B. Perform a final upload from Device A (optionally using `PFGameSaveFilesUploadOption::KeepDeviceActive` to retain the lock) with a new marker such as `A_FINAL_UPLOAD`, and ensure it completes successfully.
9. Execute Scenario 1 step 13 cleanup on both devices, remove any active HTTP mocks, and delete temporary payload markers so the account returns to a neutral state.

### 15. Mid-Operation Disk Exhaustion Recovery
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–10 to initialize, register UI callbacks (progress, sync-failed, out-of-storage), and stage a deterministic payload sized to span multiple upload chunks (e.g., 200 MB split across several files).
2. Prepare a disk pressure helper (reuse the debug filesystem mock API referenced in Scenario 12 step 2 or an equivalent harness shim) capable of lowering free space below a specified threshold while the test runs; record baseline free space before starting the operation.
3. Start `PFGameSaveFilesUploadWithUiAsync` using `PFGameSaveFilesUploadOption::KeepDeviceActive`. Allow progress to reach the second chunk (monitor via progress callback or blob `PUT` telemetry), then consume disk space mid-transfer so remaining free space undercuts the required buffer by at least 10%.
4. When the out-of-storage callback fires, respond with `PFGameSaveFilesSetUiOutOfStorageResponse(..., PFGameSaveFilesUiOutOfStorageUserAction::Retry)` without freeing space. Assert the async returns `E_PF_GAMESAVE_DISK_FULL`, partial temp artifacts (chunks, staging zips) are removed automatically, and `PFGameSaveFilesIsConnectedToCloud` still reports `true`.
5. Free enough disk space to exceed the payload size, reissue `PFGameSaveFilesUploadWithUiAsync`, and confirm the upload restarts from the beginning (verify new session IDs or chunk counters). Allow completion, then stage a new local delta to exercise the download path next.
6. Call `PFGameSaveFilesAddUserWithUiAsync` on the same device (or Device B) to download the fresh payload; once progress begins, consume disk space mid-transfer until the download fails. Assert the operation returns `E_PF_GAMESAVE_DISK_FULL`, no partial files remain in the save root, and the device stays connected to the cloud.
7. Restore disk space, rerun `PFGameSaveFilesAddUserWithUiAsync`, and confirm the download succeeds end-to-end with hashes matching the uploaded payload. Optionally run a third cycle using `PFGameSaveFilesUploadWithUiAsync` followed by `PFGameSaveFilesAddUserWithUiAsync` to prove repeated exhaustion/recovery works without manual cleanup.
8. Verify all temporary filler files are removed, compare final free space against the baseline captured in step 2, and execute Scenario 1 step 13 cleanup to close handles and release the task queue.

### 16. Large Payload Incremental Sync
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–11 to initialize, register progress callbacks, and stage a deterministic payload totaling ~220 MB across at least 500 files (use the payload tooling to generate repeatable file names and contents). Upload via `PFGameSaveFilesUploadWithUiAsync` using `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive` so other devices can hydrate the baseline.
2. Capture the returned manifest version, per-file hashes, and quota diagnostics (via `PFGameSaveFilesGetRemainingQuota`) as the baseline snapshot; persist these artifacts for later diff comparisons.
3. On Device B (clean environment) run Scenario 1 steps 1–8 to hydrate the account and verify the download succeeds without unexpected callbacks. Record overall transfer duration and bandwidth to establish a baseline for future incremental runs.
4. Return to Device A, rehydrate the manifest (Scenario 1 steps 1–8), and mutate a targeted subset of the payload—e.g., modify 5% of files spread across multiple folders plus add a new file and delete another—to simulate realistic incremental edits. Keep the total delta under 10 MB.
5. Invoke `PFGameSaveFilesUploadWithUiAsync` with `PFGameSaveFilesUploadOption::KeepDeviceActive` so Device A retains the lock. Instrument the run via debug telemetry (e.g., harness hook that logs chunk uploads or inspects `PFGameSaveFilesUploadTelemetry` if available) to capture the list of transferred files/chunks.
6. After completion, compare quota usage and manifest metadata against the baseline: confirm only changed files show new hashes, the manifest version increments once, and recorded chunk telemetry proves the SDK skipped untouched files. Optionally query the debug diff API (same tooling used by payload helpers) to confirm byte counts align with the expected delta size.
7. On Device B run `PFGameSaveFilesAddUserWithUiAsync` again to retrieve the updated payload. Verify the download size roughly matches the delta (not the full payload), the new/deleted files materialize correctly, and hashes for untouched files remain identical to the baseline snapshot.
8. Repeat steps 4–7 with an additional mutation pass (e.g., modify a different subset plus rename a folder) to ensure successive incremental uploads continue to transfer only the diffs. Track cumulative manifest size to confirm it stays within quota limits.
9. Execute Scenario 1 step 13 cleanup on all participating devices, deleting temporary payload copies and archiving the baseline-versus-delta telemetry for regression evidence.

### 17. Quota Limit Handling
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–10 to initialize, register UI callbacks (especially the sync-failed handler), and obtain the save root.
2. Keep the title configuration untouched and call `PFGameSaveFilesGetRemainingQuota` to log the default quota (≈250 MB for most titles); capture the service-reported limit and the currently consumed space for later comparison.
3. Stage a deterministic payload just under the default quota (e.g., 235–240 MB split across several files) and call `PFGameSaveFilesUploadWithUiAsync` with `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`. Confirm the upload succeeds, producing manifest version N and leaving the expected cushion (roughly 10–15 MB) in the remaining quota diagnostics.
4. Rehydrate the manifest (Scenario 1 steps 1–8) and stage an additional delta that intentionally pushes the total size beyond the default limit—append enough data to exceed the recorded quota by at least 10 MB (for example, target ~260 MB overall). Invoke `PFGameSaveFilesUploadWithUiAsync` using `PFGameSaveFilesUploadOption::KeepDeviceActive` so the device retains the lock for inspection.
5. When the sync-failed callback triggers, respond with `PFGameSaveFilesSetUiSyncFailedResponse(..., PFGameSaveFilesUiSyncFailedUserAction::Retry)` once to prove the service continues rejecting the oversized upload. Assert the async ultimately returns `E_PF_GAMESAVE_QUOTA_EXCEEDED`, the manifest version remains N, and the local staged files persist for correction.
6. Demonstrate recovery by trimming the payload back below the limit (delete or shrink files to fall at least 5% under the default quota) and invoking `PFGameSaveFilesUploadWithUiAsync` again with `KeepDeviceActive`. Confirm the upload now succeeds, producing manifest version N+1 and restoring the remaining-quota buffer reported by `PFGameSaveFilesGetRemainingQuota`.
7. Optionally repeat steps 4–6 but intercept `GameSave/InitiateUpload` to return a bandwidth-throttling error (e.g., HTTP 429 mapped to `E_PF_GAMESAVE_BANDWIDTH_LIMIT_EXCEEDED`) to validate the harness captures both storage and bandwidth quota responses without modifying service settings.
8. On Device B execute Scenario 1 steps 1–8 to download the latest manifest and verify the payload reflects the post-trim state while no oversized files leak through. Record hashes and remaining-quota diagnostics for evidence.
9. Execute Scenario 1 step 13 cleanup on all participating devices and remove any temporary payload artifacts used to exceed the quota.

### 18. High File Count Upload
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–10 to initialize, register UI callbacks, and obtain the save root handle.
2. Use the payload helper to generate a deterministic corpus of roughly 1000 files totaling ~1 MB. Distribute files across multiple folder depths (for example, 10 top-level folders with 100 leaf files each) and persist the produced manifest metadata (filenames, relative paths, hashes, sizes) for later enumeration checks.
3. Invoke `PFGameSaveFilesGetFolder` (or the equivalent deterministic enumeration helper) to assert the local file count matches the expected 1000 entries before uploading. Record the elapsed time for enumeration and the aggregate size via `PFGameSaveFilesGetFolderSize` to establish a baseline.
4. Call `PFGameSaveFilesUploadWithUiAsync` with `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`. After completion, capture the returned manifest version, upload duration, and any log that reports manifest file count to confirm the service accepted the full corpus without throttling.
5. On Device B (clean environment) run Scenario 1 steps 1–8 to hydrate the same account. Measure the `PFGameSaveFilesAddUserWithUiAsync` duration, verify no timeouts or contention callbacks fire, and enumerates the hydrated save root to confirm all 1000 files materialized with matching hashes.
6. Still on Device B, call `PFGameSaveFilesGetRemainingQuota` and `PFGameSaveFilesGetFolderSize` to ensure metadata queries remain responsive even with the large file set. Persist the enumeration logs so future regressions can compare timing and counts.
7. Return to Device A, rehydrate the manifest (Scenario 1 steps 1–8), and mutate a targeted subset: update the contents of ~50 existing files, delete another ~25, and add ~25 new files spread across the existing folder hierarchy. Regenerate the expected manifest metadata for the modified set only.
8. Execute `PFGameSaveFilesUploadWithUiAsync` using `PFGameSaveFilesUploadOption::KeepDeviceActive` so the device retains the lock for inspection. Instrument the upload (e.g., capture `PFGameSaveFilesUploadTelemetry`, chunk counters, or HTTP mock diagnostics) to verify only the touched files transfer and that manifest enumeration time remains stable despite the high file count.
9. On Device B rerun `PFGameSaveFilesAddUserWithUiAsync` to download the update. Confirm the overall transfer size aligns with the incremental delta (not the full corpus), the deleted files are absent locally, and only the mutated files report new hashes. Finish by executing Scenario 1 step 13 cleanup on all participating devices.

### 19. Filesystem Boundary Validation Across Platforms
1. Run the Common Account Reset on each target device class (Windows MSIXVC, Steam PC, Steam Deck, Xbox GDK) so every platform starts with the same clean account state. Record the OS build, drive format (NTFS/ext4/XVD), and any platform-imposed path limits for traceability.
2. On a content-authoring host generate a deterministic dataset that intentionally stresses filesystem boundaries: include reserved Windows filenames (`con.txt`, `nul.dat`), near-260-character relative paths, mixed normalization variants (precomposed and decomposed Unicode for the same logical name), folders with trailing spaces/dots (skip on platforms that disallow them), and several thousand cumulative files to push manifest metadata limits. Store the manifest blueprint (full path, UTF-8 code point sequence, byte size) in the test artifacts.
3. Using platform-aware payload tooling, materialize the dataset on each device with guardrails that automatically skip combinations the OS refuses (e.g., strip trailing spaces on Steam Deck ext4). Log any exclusions so the scenario can assert they were intentional and not silent failures.
4. Before uploading, enumerate the local save root via `PFGameSaveFilesGetFolder` and compare against the blueprint to ensure every legal entry exists with the expected normalization. For entries intentionally skipped due to platform policy, record the rationale and confirm the harness flagged them as expected.
5. On each platform, call `PFGameSaveFilesUploadWithUiAsync` (using `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`) to publish the dataset. Capture manifest version, upload duration, and logs that reports file count, longest path, and any normalization adjustments performed by the SDK.
6. After upload, immediately enumerate the returned manifest metadata (List Manifests diagnostics) and diff against the blueprint: verify filenames that required normalization now exist in a consistent canonical form, no entries were truncated, and the manifest count matches the on-disk enumeration. Archive the manifest JSON per platform for regression review.
7. On a secondary device per platform family (e.g., second Windows PC, alternate Steam Deck) execute Scenario 1 steps 1–8 to hydrate the account. Confirm `PFGameSaveFilesAddUserWithUiAsync` completes without path-related HRESULTs, assert that the hydrated filesystem reproduces the canonical names exactly, and verify Unicode normalization stability by comparing hashes/text encoding of the downloaded files.
8. Introduce cross-platform diff checks: upload a platform-specific-only naming pattern (e.g., emoji filename allowed on Steam but not Xbox) from Device A, then hydrate on a platform that rejects it. Ensure the SDK surfaces the documented error (`E_PF_GAMESAVE_PATH_NOT_ALLOWED` or equivalent), leaves the rest of the payload intact, and records logs indicating a policy violation.
9. Perform round-trip edits: pick representative files from each edge-case category, modify content on one platform, upload, and download on the others to ensure edits propagate without name mangling or loss. Include at least one rename that shortens a near-limit path to validate the manifest compaction path.
10. Execute Scenario 1 step 13 cleanup on all devices, resetting any platform-specific payload tooling overrides. Preserve the cross-platform manifest and failure-log artifacts so future runs can detect regressions in path handling.

### 20. Manifest Corruption Detection
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–11 to seed a deterministic baseline payload (multiple files with unique hashes) and record the manifest version N alongside the raw manifest JSON returned by `PFGameSaveFilesAddUserWithUiAsync`.
2. Copy the downloaded manifest file(s) from the local cache to a safe location. Using the harness corruption helper, create several tampered variants: flip bits in the manifest JSON header, truncate the file, inject invalid UTF-8 sequences, and modify checksum fields while leaving content untouched. Catalog each corruption type with an ID and expected HRESULT.
3. Replace the on-disk manifest in the save root with one corrupted variant while keeping payload files intact. Attempt `PFGameSaveFilesAddUserWithUiAsync` to trigger download validation. Assert the call fails with the documented corruption HRESULT (e.g., `E_PF_GAMESAVE_MANIFEST_INVALID`) and that the SDK deletes the corrupted manifest copy while preserving the last known good payload.
4. Immediately follow up with `PFGameSaveFilesGetFolder` and hash verification to confirm the local payload still matches the pre-corruption state. Reissue `PFGameSaveFilesAddUserWithUiAsync` after restoring the pristine manifest to ensure the device recovers automatically.
5. Repeat steps 3–4 for each corruption variant, including a scenario where the manifest references a blob that is intact but whose hash is mismatched. Use storage mocks or payload tooling to alter an individual blob while leaving metadata untouched, then run `PFGameSaveFilesAddUserWithUiAsync` to confirm the download detects the mismatch and rolls back.
6. Test upload-time corruption by intercepting `GameSave/FinalizeManifest` with an HTTP mock that injects malformed manifest content into the response. Verify the client rejects the bad response, surfaces the corruption HRESULT, and retries cleanly once the mock is cleared.
7. Validate crash resilience: corrupt the manifest, begin `PFGameSaveFilesAddUserWithUiAsync`, crash the title before the failure surfaces, relaunch, and ensure the first sync still detects the corruption and recovers deterministically.
8. On Device B run Scenario 1 steps 1–8 to pull the baseline payload after each corruption test, confirming the cloud copy remains unaltered and that corruption never leaks to other devices.
9. Finish by executing Scenario 1 step 13 cleanup on all devices and archiving the corruption-id-to-HRESULT mapping for regression tracking.

### 21. Interrupted Download Resume
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–11 to publish a payload large enough to span multiple download chunks (e.g., 150 MB across dozens of files) and release the active lock.
2. On Device B configure HTTP mocks (or network fault injection) targeting `GameSave/GetManifestDownloadDetails`, the resulting storage `GET` URLs, and any CDN endpoints so you can deterministically drop connectivity mid-download. Record the chunk size and retry policy from diagnostics for expected behavior.
3. Execute `PFGameSaveFilesAddUserWithUiAsync` on Device B. After the transfer begins (observe via progress callback or mock hit counters), sever connectivity by forcing the storage `GET` calls to time out or return transient errors. Ensure the interruption occurs after at least one chunk downloads successfully to validate partial progress handling.
4. When the sync-failed callback fires, respond with `PFGameSaveFilesSetUiSyncFailedResponse(..., PFGameSaveFilesUiSyncFailedUserAction::Retry)` once. Confirm the async eventually surfaces the documented retryable error (e.g., `E_PF_GAMESAVE_NETWORK_ERROR`) and that partial files remain quarantined (no visible partial data under the save root).
5. Without restoring connectivity, immediately reissue `PFGameSaveFilesAddUserWithUiAsync` and respond with `UseOffline` to prove the device transitions to offline mode cleanly. Verify `PFGameSaveFilesIsConnectedToCloud` returns `false` and that the save root remains empty (or contains only prior baseline data).
6. Restore connectivity by clearing the mocks or enabling success responses. Invoke `PFGameSaveFilesAddUserWithUiAsync` again, respond with `Retry`, and confirm the download resumes from the beginning or the documented checkpoint (compare chunk logs). Assert the final payload hashes match Device A’s source files and that the completion result returns success.
7. Repeat steps 3–6 with a second interruption strategy: drop the connection just before the final chunk completes to ensure the SDK can revalidate the entire payload on retry without double-writing.
8. Validate persistence across app restarts by interrupting a download, terminating the title before handling the failure, relaunching, and confirming that the next `AddUser` call resumes/restarts cleanly without orphaned temp files.
9. Execute Scenario 1 step 13 cleanup on all participating devices, remove any active network mocks, and archive logs showing initial failure, offline transition, and successful resume.

### 22. File/Folder Deletion Propagation
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–11 to publish a deterministic payload containing multiple folders (e.g., `slotA`, `slotB`, `settings`) and both shared and unique files. Capture manifest version N and hash listings for each folder.
2. On Device B run Scenario 1 steps 1–8 to hydrate the account with version N, verifying all files and folders materialize locally. Duplicate the hydrated payload to a comparison directory for later diffing and keep an offline copy that can be restored to simulate a device that missed subsequent deletions.
3. Return to Device A, rehydrate (Scenario 1 steps 1–8), and delete a selected subset of content: remove an entire folder (`slotB`) plus a handful of individual files inside `slotA`. Leave other folders untouched. Log the exact paths to be deleted so assertions can reference them by name.
4. Call `PFGameSaveFilesUploadWithUiAsync` with `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive` to publish version N+1 containing the deletions. Capture the returned manifest metadata or List Manifests response to confirm the removed entries no longer appear and that the manifest version advanced.
5. On Device B run `PFGameSaveFilesAddUserWithUiAsync` to download version N+1. After completion, enumerate the save root and compare against the baseline copy from step 2: confirm the deleted folder and files are absent, remaining content is unchanged, and no orphaned empty directories persist. Assert `PFGameSaveFilesGetFolderSize` reflects the reduced footprint.
6. Restore the duplicated baseline snapshot on Device B (or use a harness helper to roll the local save root back to version N) to simulate a device that remained offline after the original hydration. Call `PFGameSaveFilesAddUserWithUiAsync` without manual cleanup and verify the deletion applies immediately upon reconnection, with no conflict prompts and no resurgence of removed files.
7. Validate deletion logs: ensure the upload path recorded a deletion event and that downstream devices log the manifest shrink. If available, capture chunk logs showing no data transfer for deleted files beyond metadata updates.
8. Repeat the deletion workflow with a rename-to-delete scenario: rename `slotA` locally so the service interprets it as delete + add. Confirm the manifest reflects the rename correctly and that other devices observe the deletion before the new folder appears.
9. Execute Scenario 1 step 13 cleanup on all devices, restoring the account to a neutral state and archiving before/after directory listings for regression evidence.

### 23. Mutating During Upload Guard
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–11 to seed a deterministic payload large enough to keep `PFGameSaveFilesUploadWithUiAsync` busy for several seconds (e.g., ~150 MB split across folders). Record manifest version N and capture baseline hashes for all files.
2. Register UI callbacks (progress, sync-failed) and enable the harness file-system watcher so the test can record attempted writes while an upload is active. Configure the payload helper to queue background mutations (create/modify/delete) on demand and log HRESULTs or Win32 errors returned by each file operation.
3. Invoke `PFGameSaveFilesUploadWithUiAsync` with `PFGameSaveFilesUploadOption::KeepDeviceActive` so the device retains the active lock while the transfer progresses. Once the progress callback reports at least 25% completion (or the first chunk upload finishes), trigger the background mutation workflow to overwrite an existing file and add a new file in the save root.
4. Assert the harness receives the documented guard failure—either the SDK’s file helper returns `E_PF_GAMESAVE_UPLOAD_IN_PROGRESS` (preferred) or the platform layer surfaces a sharing violation. Record the precise error code and verify the mutation did not succeed (file timestamps and hashes remain unchanged, new file absent).
5. Repeat the mutation attempt using asynchronous writes on a worker thread to mimic a title-side race. Confirm the guard still blocks the operation and that no partial files appear under the save root or temporary upload staging directory.
6. Allow the original upload to complete. Validate that the resulting manifest version increments to N+1 and matches the pre-upload payload (no unintended changes). Capture telemetry/log output proving no mid-upload mutations slipped through.
7. After the successful upload, intentionally mutate the save root again (same changes as step 3) and immediately queue another `PFGameSaveFilesUploadWithUiAsync` without waiting for the first to finish (simulating back-to-back sends). Assert the second call fails fast with `E_PF_GAMESAVE_UPLOAD_ALREADY_PENDING` (or equivalent), demonstrating API-level guarding in addition to file-system protection.
8. Clear all background mutation hooks, reissue a clean `PFGameSaveFilesUploadWithUiAsync`, and confirm the upload succeeds when no concurrent writes occur. Follow with `PFGameSaveFilesAddUserWithUiAsync` on Device B to verify the cloud manifest matches the clean sequence (no ghost files from prior blocked writes).
9. Execute Scenario 1 step 13 cleanup on all participating devices, reset the file-system watcher state, and archive the mutation-attempt logs for regression triage.

### 24. Online-to-Offline Recovery
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–11 to publish a moderate payload (~50 MB) and release the active lock. Record manifest version N and baseline telemetry.
2. On Device A maintain a live session (do not uninitialize) and configure HTTP mocks or fault injectors to intermittently fail PlayFab API calls with retryable errors: target `GameSave/InitiateUpload`, `FinalizeManifest`, `ListManifests`, and `GetManifestDownloadDetails`. Use a rotating pattern (e.g., fail three consecutive calls, then succeed) to emulate flaky connectivity.
3. With the failure pattern active, call `PFGameSaveFilesUploadWithUiAsync` to stage a small delta. When the sync-failed UI callback fires, respond with `PFGameSaveFilesSetUiSyncFailedResponse(..., PFGameSaveFilesUiSyncFailedUserAction::Retry)` twice, confirming each retry surfaces the same transient connectivity HRESULT and leaves the operation waiting on further user input. Ensure the title remains online at this point.
4. Reissue `PFGameSaveFilesUploadWithUiAsync`, but when the sync-failed prompt appears again, respond with `UseOffline`. Validate the call completes with the offline HRESULT, the payload is staged locally for deferred upload, and `PFGameSaveFilesIsConnectedToCloud` flips to `false`. Confirm the save root remains accessible and that offline telemetry records the state change.
5. While offline, mutate additional files and call `PFGameSaveFilesUploadWithUiAsync` once more to queue another deferred upload. Ensure the API returns immediately with the offline HRESULT and logs the pending work without attempting network calls.
6. Switch the mocks back to success responses. Call `PFGameSaveFilesAddUserWithUiAsync`, respond with `Retry`, and confirm the device reconnects ( `PFGameSaveFilesIsConnectedToCloud` returns `true`). Verify both deferred uploads flush automatically: inspect telemetry for the queued operations and confirm manifest version advances to N+2.
7. To prove resiliency across restarts, transition offline again (re-enable failures), stage another deferred upload, terminate the title without reconnecting, relaunch, and invoke `PFGameSaveFilesAddUserWithUiAsync`. Confirm the pending upload remains queued and flushes automatically once connectivity returns.
8. On Device B (clean environment) run Scenario 1 steps 1–8 to hydrate the account and verify the final manifest reflects all queued changes, demonstrating no data loss occurred during the offline period.
9. Execute Scenario 1 step 13 cleanup on all devices, remove fault injectors, and archive telemetry covering online failures, offline mode entry, deferred upload flush, and reconnect success.

### 25. Long-Running Sync Soak
1. Run the Common Account Reset to clear the account. On Device A execute Scenario 1 steps 1–11 to stage a representative payload (~40 MB across mixed folder depths, deletes, and metadata files) and upload with `PFGameSaveFilesUploadWithUiAsync` plus `PFGameSaveFilesUploadOption::ReleaseDeviceAsActive`. Capture the manifest snapshot (version, hashes, quota) as the starting baseline.
2. Configure the harness scenario scheduler to execute a repeating loop for at least 12 hours (or 500+ iterations), rotating through `AddUser`, upload, and offline flows while preserving timestamps for each iteration. Enable verbose telemetry collection: memory usage, handle counts, task queue backlog, HRESULT histogram, and network throughput.
3. Within each loop iteration, hydrate the account per Scenario 1 steps 1–9, mutate a small deterministic delta (modify 2–3 files, add one file, delete one file), and alternate upload options: odd iterations call `PFGameSaveFilesUploadWithUiAsync` with `PFGameSaveFilesUploadOption::KeepDeviceActive`, even iterations include `ReleaseDeviceAsActive`. Record the manifest version after every upload and ensure the scheduler validates hashes against the expected delta list.
4. Every fifth iteration, inject transient network failures using the HTTP fault hooks from Scenario 3 step 1, forcing the sync-failed callback to surface. Respond with `Retry` on the first prompt and `UseOffline` on the second to prove the soak transitions between online and offline modes without manual intervention. Log the duration spent offline and confirm deferred uploads flush automatically when connectivity is restored in the following iteration.
5. Every tenth iteration, simulate an active-device contention by introducing Device B per Scenario 14 step 3: trigger `PFGameSaveFilesAddUserWithUiAsync` on Device B with `SyncLastSavedData`, confirm the lock transfers, and ensure Device A detects the change via the active-device callback before continuing the next loop.
6. At hourly checkpoints, run diagnostics on Device A: invoke `PFGameSaveFilesGetRemainingQuota`, `PFGameSaveFilesGetFolderSize`, and harness resource probes (open handle counts, heap usage, thread metrics). Assert readings remain within 5% of baseline trends and no handles leak between iterations; fail the soak early if thresholds are breached.
7. After the soak concludes, perform a comprehensive validation pass on Device A: enumerate the save root, compare against the accumulated manifest history to confirm every intended delta landed exactly once, and ensure no unexpected files exist. Inspect telemetry for HRESULT spikes, stalled uploads, or prolonged offline durations beyond configured tolerances.
8. On Device B (clean environment) execute Scenario 1 steps 1–8 to hydrate the final manifest, verifying the dataset matches Device A’s final hash set. Run `PFGameSaveFilesAddUserWithUiAsync` twice more to ensure incremental downloads complete quickly and that no deferred changes remain queued.
9. Execute Scenario 1 step 13 cleanup on all participating devices, stop the scheduler, dispose of HTTP mocks, and archive the soak telemetry bundle (iteration logs, resource metrics, manifest history) for regression comparison in future runs.



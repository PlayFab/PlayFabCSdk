# Client Compliance Review: Service Endpoint Rules

Review of PFGameSave client source against the service endpoint rules documented in `specs/playfab-gamesave/design/gamesave-service-endpoint-rules.md`.

**Goal**: For each service endpoint, verify the client correctly constructs requests, handles responses/errors, and respects all service-side constraints.

**Client source**: `Source/PlayFabGameSave/Source/`
**Generated service wrappers**: `Source/PlayFabGameSave/Source/Generated/`

---

## Review Progress

| # | Endpoint | Client call sites | Status | Issues |
|---|----------|-------------------|--------|--------|
| 1 | [ListManifests](#1-listmanifests) | LockStep, UploadStep, RelockStep, ResetCloudStep | ⚠️ Issues found | NextAvailableVersion not read by UploadStep/RelockStep; DisableUnsignaledOutOfProcessUpload ignored |
| 2 | [InitializeManifest](#2-initializemanifest) | LockStep, UploadStep (TakeLock), RelockStep | ⚠️ Issues found | UploadStep/RelockStep version computation misses filtered manifests; unbounded offset retry |
| 3 | [InitiateUpload](#3-initiateupload) | UploadStep | ⚠️ Issues found | No client-side file count pre-validation; generic error handling for non-retryable errors |
| 4 | [UpdateManifest](#4-updatemanifest) | UploadStep (PromoteIfNeeded), SetSaveDescriptionStep | ⚠️ Issues found | PromoteIfNeeded blocks on quarantined baseline instead of skipping |
| 5 | [FinalizeManifest](#5-finalizemanifest) | UploadStep | ⚠️ Issues found | No client-side ConflictingVersion!=Version guard; generic error handling |
| 6 | [GetManifestDownloadDetails](#6-getmanifestdownloaddetails) | CompareStep | ❌ Non-compliant | No handling for ManifestVersionNotFinalized or ManifestVersionQuarantined — pointless retry loops |
| 7 | [DeleteManifest](#7-deletemanifest) | ResetCloudStep, UploadStep (ReleaseDeviceAsActive) | ✅ Compliant | Minor: DeletePendingManifest silently swallows errors |
| 8 | [RollbackToManifest](#8-rollbacktomanifest) | LockStep (SelectBaselineManifest) | ❌ Non-compliant | LastConflictLoser uses wrong sort key (pairHigh vs loser version); filters Finalized only |

Legend: ⬜ Not started · 🔄 In progress · ✅ Compliant · ⚠️ Issues found · ❌ Non-compliant

---

## 1. ListManifests

**Status**: ⚠️ Issues found

### Service rules to verify
- [x] Client reads `NextAvailableVersion` and uses it for new manifest version computation
- [x] Client reads `TitleConfiguration` fields (PerPlayerQuotaBytes, rollback disable flags, etc.)
- [x] Client handles filtering correctly (understands PendingDeletion/Quarantined may be absent)
- [x] Client handles `ManifestNotFound` and other error responses

### Findings

| Check | Status | Details |
|-------|--------|---------|
| NextAvailableVersion used (LockStep) | ✅ | `LockStep.cpp:222` reads `GetNextAvailableVersion()` into `m_nextAvailableVersion`, used as primary source for version computation |
| NextAvailableVersion used (UploadStep) | ❌ | `UploadStep.cpp:925` reads only `GetManifests()`, never reads `GetNextAvailableVersion()`. TakeLock computes `max(visible) + 1` which misses filtered-out PendingDeletion/Quarantined manifests → avoidable 409 retries |
| NextAvailableVersion used (RelockStep) | ❌ | `RelockStep.cpp:181` reads only `GetManifests()`. CreatePendingManifest uses `base + 1` which can be much lower than actual next available → requires offset retries to converge |
| PerPlayerQuotaBytes | ✅ | `LockStep.cpp:224-290` parses as uint64, defaults to 256MB if 0, clamps to int64_max |
| DisableClientRollbackToLastKnownGood | ✅ | `LockStep.cpp:260,266-269` gates rollback option |
| DisableClientRollbackToLastConflictLoser | ✅ | `LockStep.cpp:259,271-274` gates rollback option |
| DisableUnsignaledOutOfProcessUpload | ❌ | Never read by any call site. Field exists in generated types but is ignored |
| Manifest filtering (losers skipped) | ✅ | `TryGetLatestFinalizedManifest` correctly skips conflict losers in primary pass, falls back to highest loser only if no winners exist |
| Error handling | ✅ | All call sites route failures to error UI or step failure. Adequate |

---

## 2. InitializeManifest

**Status**: ⚠️ Issues found

### Service rules to verify
- [x] `BaseVersion ≤ Version` always holds
- [x] `BaseVersion == Version` only when no base dependency (first manifest or self-referencing)
- [x] When `BaseVersion < Version`, the base manifest must be finalized (client must know this)
- [x] `BaseVersion` points to the correct data ancestor (winner, not loser, after conflicts)
- [x] Client handles `ManifestVersionAlreadyExists` (409) — retries with version offset
- [x] Client handles `BaseVersionNotAvailable` — retries or fails gracefully
- [x] Client populates Metadata (DeviceId, DeviceName, DeviceType) correctly

### Findings

| Check | Status | Details |
|-------|--------|---------|
| BaseVersion ≤ Version | ✅ | `CreateInitManifestRequest` (LockStep.cpp:767-794) applies offset consistently. When base==version, offset applied to both |
| BaseVersion points to winner | ✅ | LockStep uses `m_baselineFinalizedManifest` (winner/rollback target). UploadStep uses `m_postUploadLatestFinalizedPFManifest` (set by `TryGetLatestFinalizedManifest`). RelockStep uses `TryGetLatestFinalizedManifest`. All skip conflict losers |
| Metadata populated | ✅ | `CreateInitManifestRequest` sets DeviceId, DeviceName, DeviceType, DeviceVersion |
| 409 retry (version offset) | ✅ | All three call sites (LockStep:492, UploadStep:1035, RelockStep:75) increment `m_manifestVersionOffset` and retry |
| 409 retry unbounded | ⚠️ | `m_manifestVersionOffset` has no upper bound — theoretical infinite loop on pathological concurrent-client races |
| BaseVersionNotAvailable retry | ✅ | All three call sites re-list manifests and retry up to 3 times with offset reset |
| Version from NextAvailableVersion (LockStep) | ✅ | Uses `m_nextAvailableVersion` as primary source |
| Version computation (UploadStep) | ❌ | Uses `max(visible manifests) + 1` — misses filtered manifests (same issue as ListManifests) |
| Version computation (RelockStep) | ❌ | Uses `effectiveBase + 1` — can be far below actual next available |
| Idempotency on lost response | ⚠️ | Always increments version on 409 instead of re-trying same version — creates orphaned Initialized manifests on network timeouts |

---

## 3. InitiateUpload

**Status**: ⚠️ Issues found

### Service rules to verify
- [x] Only called when manifest is in Initialized or Uploading state
- [x] Files don't collide with existing FileMap entries from other versions
- [x] Total file count ≤ 100
- [x] Client does not set TestOnlyCustomTokenExpirationSeconds in production

### Findings

| Check | Status | Details |
|-------|--------|---------|
| Manifest state precondition | ✅ | State machine enforces ordering: CompressFiles → InitiateUpload. Manifest is always Initialized or Uploading at this point |
| File list populated | ✅ | `UploadStep.cpp:551-558` iterates compressed files and creates `FileToUploadWrap` per file |
| TestOnlyCustomTokenExpirationSeconds | ✅ | Never set by client code |
| File count ≤ 100 (client-side) | ❌ | No pre-validation before calling InitiateUpload. Server enforces at FinalizeManifest time; client has retry logic (re-compress into fewer batches) at lines 772-796 |
| Error-specific handling | ❌ | `UploadStep.cpp:580-591` treats all failures identically — ManifestUpdatesNotAllowed, FileAlreadyExists, and transient errors all get the same retry UI |

---

## 4. UpdateManifest

**Status**: ⚠️ Issues found

### Service rules to verify
- [x] UploadProgress only sent when manifest is Initialized/Uploading
- [x] ManifestDescription only sent when manifest is Initialized/Uploading
- [x] MarkAsKnownGood only sent when manifest is Finalized (or PendingDeletion with FinalizationTimestamp)
- [x] At least one field is always set in the request
- [x] Client handles `NotFinalizedManifestNotEligibleAsKnownGood` error

### Findings

| Check | Status | Details |
|-------|--------|---------|
| MarkAsKnownGood targets Finalized | ✅ | `UploadStep.cpp:1232-1234` `EvaluateKnownGoodPromotionEligibility` only targets Finalized manifests. Short-circuits if already KnownGood |
| ManifestDescription state | ✅ | `SetSaveDescriptionStep.cpp:74` targets pending manifest (Initialized/Uploading). Correct |
| UploadProgress field | ✅ | Not sent by SyncManager — client never calls UpdateManifest for progress. Field unused |
| At least one field set | ✅ | MarkAsKnownGood and ManifestDescription are always set when their respective calls are made |
| PromoteIfNeeded error handling | ❌ | `UploadStep.cpp:956-964` treats all failures generically. If baseline is quarantined (NotFinalizedManifestNotEligibleAsKnownGood), user stuck in retry loop. Promotion is non-critical — should skip and continue to TakeLock |
| SetSaveDescription error handling | ⚠️ | `SetSaveDescriptionStep.cpp:88-93` propagates raw error, no retry mechanism. Description updates are non-critical — acceptable but differs from other steps |
| Promotes even when baseline pruned | ⚠️ | `UploadStep.cpp:1250-1251` attempts promotion even if baseline not in manifest list. Service will reject — acceptable defensive behavior |

---

## 5. FinalizeManifest

**Status**: ⚠️ Issues found

### Service rules to verify
- [x] Only called when manifest is in Uploading state
- [x] `Conflict.ConflictingVersion != Version` (no self-conflict)
- [x] `MarkBaseAsKnownGood` not set when `BaseVersion == Version`
- [x] `Conflict.ConflictingVersion` should not equal `BaseVersion` (disabled server-side but client should still aim to comply)
- [x] All files in FilesToFinalize were previously uploaded (exist in FileMap)
- [x] When not Force and not conflict-loser, manifest must be latest version
- [x] Client correctly sets `IsWinner` based on user conflict choice
- [x] Client handles `NewerManifestExists` (409) error

### Findings

| Check | Status | Details |
|-------|--------|---------|
| Version field | ✅ | `UploadStep.cpp:696` correctly set from pending manifest version |
| FilesToFinalize | ✅ | Correctly builds array of new + kept remote files with names and sizes |
| Conflict.IsWinner | ✅ | TakeLocal → `true`, TakeRemote → `false` (FolderSyncManager.cpp:217-226). Correct |
| Conflict.ConflictingVersion | ✅ | Set to `baselineVer` (the cloud version that was conflicting). Correct |
| No self-conflict guard | ⚠️ | No client-side check that `ConflictingVersion != Version`. Service enforces this. Low risk — pending version is always newer than baseline |
| force field | ✅ | Never set — intentional. Client uses offline fallback instead of forcing |
| markBaseAsKnownGood on Finalize | ✅ | Never set on FinalizeManifest. Done via separate UpdateManifest (avoids BaseVersion==Version restriction). Correct design |
| NewerManifestExists (409) | ✅ | `UploadStep.cpp:801` transitions to ForceDisconnectFromCloud. Correct |
| InvalidParams (>100 files) | ✅ | `UploadStep.cpp:775` triggers re-compression with retry cap |
| Default conflict choice | ⚠️ | Unknown `TakeUIChoice` silently drops conflict metadata (FolderSyncManager.cpp:227-229). Warning trace only. Low risk — enum always resolved by UI |
| DataStorageQuotaExceeded | ⚠️ | Falls through to generic error UI. Could benefit from specific user message |

---

## 6. GetManifestDownloadDetails

**Status**: ❌ Non-compliant (specific error handling)

### Service rules to verify
- [x] Only called for finalized manifests
- [x] Client handles `ManifestVersionNotFinalized` error
- [x] Client handles `ManifestVersionQuarantined` error
- [x] Client uses SAS download URLs before expiration (60 min)

### Findings

| Check | Status | Details |
|-------|--------|---------|
| Only for finalized manifests | ✅ | Called with version from `TryGetLatestFinalizedManifest` output. Correct |
| Request construction | ✅ | `CompareStep.cpp:65` correctly sets version |
| Extended manifest download | ✅ | Locates `extended-<ver>-manifest.json` in download details, downloads and parses. Graceful skip if not found |
| ManifestVersionNotFinalized | ❌ | No specific handling. Generic retry UI — retrying a not-finalized manifest loops pointlessly. Should re-list manifests or go offline |
| ManifestVersionQuarantined | ❌ | No specific handling. Generic retry UI — quarantined manifests never become downloadable. Should fall back to KnownGood or notify user |
| Parse failure | ✅ | Extended manifest parse failure → immediate CompareStepFailure (no retry). Correct — parsing errors won't fix on retry |
| SAS token usage | ✅ | URLs used immediately for download. No concern about 60-min expiry |

---

## 7. DeleteManifest

**Status**: ✅ Compliant (minor note)

### Service rules to verify
- [x] Client understands deletion is soft (PendingDeletion)
- [x] Client handles idempotent success for already-deleted manifests

### Findings

| Check | Status | Details |
|-------|--------|---------|
| Request construction (ResetCloudStep) | ✅ | `ResetCloudStep.cpp:137-138` sets version per manifest. Correct |
| Delete loop tolerates failures | ✅ | `ResetCloudStep.cpp:151-154` logs warning and continues to next manifest |
| Skips PendingDeletion | ✅ | `ResetCloudStep.cpp:123-126` excludes already-PendingDeletion manifests |
| Request construction (UploadStep) | ✅ | `UploadStep.cpp:1310-1311` version from pending manifest |
| DeletePendingManifest error handling | ⚠️ | `UploadStep.cpp:1313-1320` silently swallows failures — always transitions to DeleteDone. Benign given idempotency but a transient failure leaves stale pending manifest with no indication |

---

## 8. RollbackToManifest

**Status**: ❌ Non-compliant (conflict loser selection)

### Service rules to verify
- [x] Client only requests rollback to finalized manifests
- [x] Client correctly identifies Last Known Good (highest KnownGood version, any status)
- [x] Client correctly identifies Last Conflict Loser (highest version with !IsWinner)
- [x] Client respects disable flags
- [x] Mutual exclusivity: only one rollback flag at a time

### Findings

| Check | Status | Details |
|-------|--------|---------|
| Mutual exclusivity | ✅ | `PFGameSaveFilesAPI.cpp:145-152` validates at API entry point. Returns `E_INVALIDARG` if both flags set |
| DisableClientRollbackToLastKnownGood | ✅ | `LockStep.cpp:260,266-269` masks off bit when disabled |
| DisableClientRollbackToLastConflictLoser | ✅ | `LockStep.cpp:259,271-274` masks off bit when disabled |
| LastKnownGood selection | ✅ | `LockStep.cpp:691-701` finds highest version with KnownGood=true |
| LastKnownGood ignores status | ⚠️ | `LockStep.cpp:687` filters Finalized only. Service rule says "any status" — PendingDeletion KnownGood would be missed. Unlikely edge case |
| LastConflictLoser = highest !IsWinner version | ❌ | `LockStep.cpp:742-750` uses `pairHigh = max(manifestVersion, conflictingVersion)` as sort key. Service uses the loser's own version. Diverges when low-version loser conflicts with high-version winner |
| LastConflictLoser ignores status | ⚠️ | `LockStep.cpp:722` filters Finalized only. Same deviation as KnownGood |
| Graceful fallback | ✅ | Both fall back to latest finalized `L` when target not found |
| No conflict state flipping | ✅ | Not applicable — client does local baseline selection, not a direct RollbackToManifest service call |

---

## Cross-Cutting Concerns

| Concern | Status | Notes |
|---------|--------|-------|
| Error code mapping (service → HRESULT) | ✅ | `GameSave.cpp` uses `ServiceErrorToHR()` which maps all service error codes. `PFErrors.h:918-936` defines all GameSave HRESULTs |
| Retry logic for ConflictUpdatingManifest | ✅ | Service retries 2x server-side. Client does not see these — transparent. Client-side retries are for 409 ManifestVersionAlreadyExists (different) |
| BaseVersion chain after conflicts | ✅ | Fixed in current branch — winner used as base, max(all) + 1 for version number |
| Quota computation | ✅ | Uses `PerPlayerQuotaBytes` from TitleConfiguration, defaults to 256MB |
| Telemetry alignment | ⬜ | Not reviewed — out of scope |

---

## Issue Summary (Severity-Ordered)

| # | Severity | Endpoint | Issue |
|---|----------|----------|-------|
| 1 | **High** | RollbackToManifest | LastConflictLoser selection uses `pairHigh` sort key instead of loser's own version — produces wrong result in edge cases |
| 2 | **Medium** | GetManifestDownloadDetails | No handling for `ManifestVersionNotFinalized` — pointless retry loop |
| 3 | **Medium** | GetManifestDownloadDetails | No handling for `ManifestVersionQuarantined` — pointless retry loop |
| 4 | **Medium** | ListManifests | UploadStep and RelockStep don't read `NextAvailableVersion` — avoidable 409 retries |
| 5 | **Medium** | UpdateManifest | PromoteIfNeeded blocks on quarantined baseline instead of skipping |
| 6 | **Low** | InitiateUpload | No client-side file count pre-validation (mitigated by FinalizeManifest retry) |
| 7 | **Low** | InitiateUpload | Generic error handling for non-retryable errors like ManifestUpdatesNotAllowed |
| 8 | **Low** | ListManifests | `DisableUnsignaledOutOfProcessUpload` never read |
| 9 | **Low** | InitializeManifest | Version offset retry has no upper bound |
| 10 | **Low** | RollbackToManifest | LastKnownGood and LastConflictLoser filter Finalized only (service says "any status") |
| 11 | **Info** | DeleteManifest | DeletePendingManifest silently swallows errors |
| 12 | **Info** | FinalizeManifest | No client-side ConflictingVersion != Version guard |
| 13 | **Info** | InitializeManifest | 409 retry creates orphaned manifests instead of exploiting idempotency |

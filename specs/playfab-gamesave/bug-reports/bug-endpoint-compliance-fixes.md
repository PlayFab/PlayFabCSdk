# Fixes: Endpoint Compliance Issues (1–5)

Five issues found during client compliance review against service endpoint rules (`specs/playfab-gamesave/design/gamesave-service-endpoint-rules.md`). Full review at `specs/playfab-gamesave/ai-code-review/endpoint-compliance-review.md`.

---

## Fix 1 (High): LastConflictLoser Selection Uses Wrong Sort Key

**File:** `LockStep.cpp` — `SelectBaselineManifest`, line ~742

**Problem:**
When `RollbackToLastConflict` is requested, the client finds the "last conflict loser" to use as the rollback baseline. The service defines this as the conflict loser manifest with the **highest own version number**. The client was instead using `pairHigh = max(loserVersion, conflictingVersion)` — the maximum of the loser's version and the version it conflicted with.

These produce different results when a low-version loser conflicts with a high-version winner:

| Loser | Conflicted With | Client (pairHigh) | Service (loser ver) |
|-------|----------------|--------------------|---------------------|
| v1 (loser) | v5 (winner) | pairHigh = 5 | loser ver = 1 |
| v3 (loser) | v2 (winner) | pairHigh = 3 | loser ver = 3 |
| **Client picks** | | **v1** (pairHigh 5 > 3) | |
| **Service picks** | | | **v3** (loser ver 3 > 1) |

The client would roll back to v1 while the service considers v3 the valid rollback target, causing `ManifestNotEligibleForRollback`.

**Fix:**
Changed the ranking key from `pairHigh` to the loser's own version (`thisVer`):

```cpp
// Before
uint64_t pairHigh = (thisVer > conflictVer) ? thisVer : conflictVer;
if (pairHigh > bestPairVersion)

// After
if (thisVer > bestPairVersion)
```

---

## Fix 2 & 3 (Medium): GetManifestDownloadDetails — Pointless Retry on Permanent Errors

**File:** `CompareStep.cpp` — `GetManifestDownloadDetails` callback, line ~74

**Problem:**
When `GetManifestDownloadDetails` fails, ALL errors were routed to the sync-failed retry UI. Two error codes represent **permanent** failures for a given manifest version:

- `E_PF_GAME_SAVE_MANIFEST_VERSION_NOT_FINALIZED` — the requested manifest was rolled back or is no longer finalized. Retrying the same version will always fail.
- `E_PF_GAME_SAVE_MANIFEST_VERSION_QUARANTINED` — the manifest was quarantined by file validation. It will never become downloadable again.

The user would be stuck choosing "Retry" (which loops forever) or "Use Offline" (which may not be obvious).

**Fix:**
Added specific detection for these two HRESULTs. When either is returned, the client forces disconnect-from-cloud and proceeds to `CompareDone` — the same path as choosing "Use Offline" but without requiring user interaction for a situation the user cannot resolve:

```cpp
if (result.hr == E_PF_GAME_SAVE_MANIFEST_VERSION_NOT_FINALIZED ||
    result.hr == E_PF_GAME_SAVE_MANIFEST_VERSION_QUARANTINED)
{
    m_forceDisconnectFromCloud = true;
    m_stage = CompareStage::CompareDone;
    task.ScheduleNow();
}
else
{
    // Transient errors still get the retry UI
    ...
}
```

---

## Fix 4 (Medium): UploadStep and RelockStep Don't Use NextAvailableVersion

**Files:** `UploadStep.cpp` (TakeLock), `UploadStep.h`, `RelockStep.cpp`, `RelockStep.h`

**Problem:**
After an upload completes, `ListManifestsAfterUpload` refreshes the manifest list. The service response includes `NextAvailableVersion` (`max(all versions) + 1`) which accounts for **all** manifests including those filtered out by `IncludeUnavailable=false` (PendingDeletion, Quarantined).

UploadStep's `TakeLock` was computing `newManifestVersion` from `max(visible manifests) + 1`, which misses filtered-out manifests. This caused avoidable `ManifestVersionAlreadyExists` (409) errors when a PendingDeletion or Quarantined manifest existed at a version higher than any visible manifest.

RelockStep's `CreatePendingManifest` was computing `newVersion = effectiveBase + 1`, which could be far below the actual next available version if many manifests existed between the base and the current head.

Both relied on the `m_manifestVersionOffset` retry mechanism to converge, wasting network round-trips.

**Fix:**
Both now read and store `GetNextAvailableVersion()` from the ListManifests response and use it as the primary source for new version computation, with fallback to manual computation:

**UploadStep:**
```cpp
// Store in ListManifestsAfterUpload callback:
m_nextAvailableVersion = result.Payload().GetNextAvailableVersion();

// Use in TakeLock:
uint64_t newManifestVersion = 0;
if (!m_nextAvailableVersion.empty())
    newManifestVersion = StringToUint64(m_nextAvailableVersion);
if (newManifestVersion == 0)
    // fallback to max(visible) + 1
```

**RelockStep:**
```cpp
// Store in RefreshManifests callback:
m_nextAvailableVersion = result.Payload().GetNextAvailableVersion();

// Use in CreatePendingManifest:
uint64_t newVersion = 0;
if (!m_nextAvailableVersion.empty())
    newVersion = StringToUint64(m_nextAvailableVersion);
if (newVersion == 0)
    newVersion = effectiveBase + 1;
```

Note: `LockStep` already read `NextAvailableVersion` correctly — only UploadStep and RelockStep were missing it.

---

## Fix 5 (Medium): PromoteIfNeeded Blocks on Quarantined/Ineligible Baseline

**File:** `UploadStep.cpp` — `PromoteIfNeeded` UpdateManifest callback, line ~956

**Problem:**
After a successful upload, the `PromoteIfNeeded` stage attempts to mark the original activation baseline as Known Good via `UpdateManifest(MarkAsKnownGood=true)`. If this call fails (e.g., the baseline was quarantined or deleted between upload and promotion), **all** errors were routed to the sync-failed retry UI.

Two error codes are permanent failures for promotion:
- `E_PF_GAME_SAVE_NOT_FINALIZED_MANIFEST_NOT_ELIGIBLE_AS_KNOWN_GOOD` — the baseline is quarantined or no longer finalized.
- `E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED` — the manifest is in a state that doesn't allow updates.

The user would be stuck in a retry loop for a non-critical operation (the upload already succeeded — promotion is an optimization for future rollback).

**Fix:**
Added specific detection for these non-retryable codes. When either is returned, promotion is silently skipped and the state machine continues to `TakeLock`:

```cpp
if (result.hr == E_PF_GAME_SAVE_NOT_FINALIZED_MANIFEST_NOT_ELIGIBLE_AS_KNOWN_GOOD ||
    result.hr == E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED)
{
    TRACE_WARNING("...skipping promotion");
    m_stage = UploadStage::TakeLock;
    task.ScheduleNow();
}
else
{
    // Transient errors still get the retry UI
    ...
}
```

---

## Testing

All fixes are compile-verified (build succeeded, 0 warnings, 0 errors). Fixes 2–5 are error-handling improvements for rare edge cases that are difficult to trigger in the standard test harness without mock/fault injection. Fix 1 requires a scenario with multiple conflict resolutions at different version numbers to observe the divergent selection — consider adding a dedicated rollback test scenario.

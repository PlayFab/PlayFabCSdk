# Fix: TakeLock Uses Wrong BaseVersion After Conflict Resolution

## Bug Summary

After conflict resolution during `AddUserWithUiAsync`, the `TakeLock` step (and other lock-acquisition paths) set the new manifest's `BaseVersion` to the wrong value. The `BaseVersion` was being derived from `m_postUploadLatestFinalizedPFManifest` — which is the data-selection "latest finalized" manifest and skips conflict losers. After a conflict the loser often has the highest version number, so using the winner's version as the new manifest version caused collisions with the already-existing loser version (`MANIFEST_VERSION_ALREADY_EXISTS`).

More fundamentally, the `BaseVersion` field should reflect the manifest that the game's data is derived from — the **winner** of the conflict (or the non-conflict data-selection baseline). A conflict loser is a preserved snapshot of discarded data; it should never be in the base version chain of future manifests.

### Concrete example (Take Remote)

| Step | Version | Status | BaseVersion | Notes |
|------|---------|--------|-------------|-------|
| Cloud baseline | v5 | Finalized | — | The data the game is using |
| Pending lock | v6 | Initialized | v5 | Created during LockStep |
| Conflict detected | — | — | — | Local changed + cloud changed |
| User chooses "Take Remote" | — | — | — | Cloud data wins |
| Conflict upload | v6 | Finalized (loser) | v5 | Local branch preserved for rollback |
| **TakeLock (before fix)** | v7 | Initialized | **v6 ← WRONG** | Based on loser (highest version) |
| **TakeLock (after fix)** | v7 | Initialized | **v5 ← CORRECT** | Based on winner (data ancestor) |

Before the fix, `newManifestVersion` was also derived from the winner's version (`v5 + 1 = v6`), which collided with the loser. The retry mechanism with `m_manifestVersionOffset` would eventually work around this, but the base version chain was still wrong.

### Why wrong BaseVersion matters

1. **MarkBaseAsKnownGood**: During `FinalizeManifest`, the client can request `MarkBaseAsKnownGood=true`. If the base points to the loser, the wrong manifest gets promoted to known good.
2. **Disabled service validation**: The service has a check that `ConflictingVersion != BaseVersion` — a manifest shouldn't conflict with its own base. This check is currently disabled (Bug 60132083) because the client was getting this wrong. Fixing the base version is a prerequisite to re-enabling that validation.
3. **Version chain integrity**: The base version chain should represent data lineage. Pointing to a discarded conflict loser breaks that semantic.

## Fix

Three files changed, all following the same principle: **use the data-selection baseline (winner/non-conflict) as `BaseVersion`, and compute the new manifest version from the highest existing version of any status to avoid collisions.**

### UploadStep.cpp — TakeLock after upload

The `TakeLock` stage creates a new pending manifest after a successful upload (to retain active device).

**Before:** Both `baseManifestVersion` and `newManifestVersion` were derived from `m_postUploadLatestFinalizedPFManifest.GetVersion()`. After a conflict, this is the winner — correct for base, but `version + 1` may collide with the loser.

**After:** `baseManifestVersion` is the winner (from `m_postUploadLatestFinalizedPFManifest`, which is set by `TryGetLatestFinalizedManifest` — skips losers). `newManifestVersion` is `max(all manifest versions) + 1`, which safely avoids collisions with any existing manifest including losers.

### LockStep.cpp — CreatePendingManifest during AddUser

Two code paths create a new pending manifest during `AcquireActiveDevice`: one when a stale/foreign pending exists, and one when no pending exists at all.

**Before:** `baseManifestVersion` was set from `m_baselineFinalizedManifest` (data-selection baseline). This was already correct — `m_baselineFinalizedManifest` is set by `TryGetLatestFinalizedManifest` or `SelectBaselineManifest`, both of which skip conflict losers. No code change was needed for the base value itself. Comments added for clarity.

**No functional change** — `newManifestVersion` already came from `m_nextAvailableVersion` (service-provided `max + 1`) or `pendingVersion + 1`, both of which are safe.

### RelockStep.cpp — RefreshManifests during relock

After an upload, `RelockStep` refreshes the manifest list and picks a base for the new lock.

**Before:** Used `TryGetLatestFinalizedManifest` — already correct (skips losers). Variable renamed for clarity.

**No functional change** to the base version value.

## What did NOT change

These call sites use `TryGetLatestFinalizedManifest` for **data selection** (choosing which manifest's files to show the user). Skip-losers is correct here:

| File | Call site | Purpose |
|------|-----------|---------|
| `LockStep.cpp` | `SelectBaselineAndCheckContention` | Picks which finalized data to sync during AddUser |
| `ActiveDevicePollWorker.cpp` | Active device polling | Detects if another device changed the visible data |
| `UploadStep.cpp` | `PromoteIfNeeded` | Evaluates Known Good promotion of the winner manifest |

## Test scenario

`gamesave-40-takelock-stale-manifest-after-conflict.yml` exercises this fix:

1. DeviceA uploads baseline data (v1 finalized, released).
2. DeviceB downloads baseline, modifies `slotA/`, uploads (v2 finalized, released).
3. DeviceA modifies `slotA/` locally, then calls `AddUserWithUiAsync` → conflict detected (local + cloud both changed `slotA/`).
4. Auto-response: Take Remote. Conflict upload finalizes local data as loser (v3). TakeLock creates v4 with `base=v2` (the winner).
5. DeviceA writes new data and uploads with `KeepDeviceActive`.
6. DeviceB downloads and verifies it sees DeviceA's post-conflict data — confirming the version chain is correct and data flows through cleanly.

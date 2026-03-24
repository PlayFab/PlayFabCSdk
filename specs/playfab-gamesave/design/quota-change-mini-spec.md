# PFGameSave Quota Retrieval Change - Mini Spec
Date: 2025-09-15

## Problem Statement
The current PFGameSave implementation calls a service quota endpoint at user add / lock acquisition time to obtain remaining quota (bytes) for the title/user. That service endpoint is being deprecated/removed. We must stop calling it and instead use the per-player quota value already present in the Title Configuration ("perPlayerQuotaBytes").

Goal: PFGameSaveFilesGetRemainingQuota should return:

   remaining = perPlayerQuotaBytes - totalLocalGameSaveBytes

Where totalLocalGameSaveBytes is the aggregate size (in bytes) of all files currently present in the local game save root as tracked by the in‑memory `SharedPtr<FileFolderSet> localFileFolderSet`. If the set is empty (e.g. very first activation with no files) remaining = perPlayerQuotaBytes. We eliminate any direct network call dedicated solely to quota.

## Current Behavior (Summary)
1. During LockStep acquisition a stage `GetRemainingQuota` performs a network call to fetch remaining quota; value stored in `LockStep::m_remainingQuotaFromService`.
2. `FolderSyncManager` copies that into `m_remainingQuotaFromService` and increments/decrements it as uploads occur (`m_totalUploadedSinceQuotaSet`).
3. API provider `GetRemainingQuota` returns internally cached value for in-proc; GRTS path defers to platform (`PFXGameSaveFilesGetRemainingQuota`).

## Desired Behavior
1. Remove (or bypass) the LockStep stage "GetRemainingQuota" network call.
2. Retrieve `perPlayerQuotaBytes` from existing service responses:
   - Primary source: `ListManifestsResponse.TitleConfiguration.PerPlayerQuotaBytes` (already returned with the initial manifest list). This is a string field; parse with `std::stoull` (base 10) into `uint64_t`.
   - Caching: Store the parsed quota in global state (e.g. `GameSaveGlobalState` or maintained within `LockStep`). Subsequent quota computations use the cached numeric value; no further calls unless both primary and fallback failed previously and we retry after a reconnect.
   - Validation & overflow: If parsing throws or value exceeds `UINT64_MAX` (should not) treat as missing and fall back. If the parsed value is 0, interpret this as unset and substitute a default quota of 256MB (268,435,456 bytes) to preserve backwards-compatible reasonable capacity (zero will NOT mean "no storage").
   - Thread-safety: Retrieval/parsing occurs during activation path before making quota available to API callers; no additional synchronization required beyond existing LockStep mutex.
3. Compute current usage (totalLocalGameSaveBytes):
   - Iterate all files recorded in `localFileFolderSet` summing their on-disk (uncompressed) sizes.
   - Rationale: After compare/download the local set reflects authoritative local content; using uncompressed size is transparent to developers and avoids ambiguity now that service accounting is removed.
4. Do NOT persist a cached "remaining" value; `PFGameSaveFilesGetRemainingQuota` will compute on demand each time it is called by:
   - Reading the cached parsed `perPlayerQuotaBytes` value (constant until re-init) and enumerating current `localFileFolderSet` file sizes.
   - Returning `perPlayerQuotaBytes - liveSumLocalFileBytes` (may be negative if over quota).
5. Upload path adjustments: no need to update a stored remaining value; finalize just updates `localFileFolderSet` and future calls naturally reflect new state.
6. API unchanged; doc updated to reflect new data source.

## Edge Cases
- Title config missing or `perPlayerQuotaBytes` absent: Treat as unlimited: return INT64_MAX (or a large sentinel). Document behavior.
- Offline mode: Continue returning cached last-known remaining; if offline activation (UseOffline) path, return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD as today.
- Negative remaining: allowed (signals over-quota) and upload logic should already block on negative values (verify). If not, add check.
- Multi-device contention: On device switch we recompute based on freshly rebuilt `localFileFolderSet` after download/compare. If user chose TakeLocal during conflict, remaining reflects only kept local data (authoritative going forward).
- Local deletions reduce totalLocalGameSaveBytes after next sync cycle that refreshes the set (post compare step).

## Telemetry Changes
- Emit new field `quotaSource = TitleConfig` instead of `ServiceEndpoint`.
- Add telemetry when title config fetch fails -> `quotaFallback = Unlimited`.

## API / Header Changes
No public API signature changes.
Update comment in `PFGameSaveFilesGetRemainingQuota` doc string to: "Returns remaining bytes derived from the title configuration perPlayerQuotaBytes field minus current stored compressed data size." and remove reference to service endpoint.

## Implementation Plan (High-Level)
1. Introduce helper `QuotaManager` or extend `LockStep`:
   - Method: `InitializeQuotaFromTitleConfiguration(const ListManifestsResponse&)` which attempts to parse `TitleConfiguration.PerPlayerQuotaBytes`.
   - If missing, call existing generated wrapper to issue `GetConfigForTitle` and parse `GetConfigForTitleResponse.PerPlayerQuotaBytes`.
   - Store resulting `uint64_t m_effectiveQuotaBytes` (or sentinel when unavailable).
2. Skip/Remove LockStage::GetRemainingQuota state machine usage.
   - Replace with inline computation after manifests are fetched & before compare stage.
3. Remove dependency on `m_remainingQuotaFromService`; eliminate any variables tracking incremental remaining quota. (retain parsed `perPlayerQuotaBytes` only.)
4. Modify provider implementation of `PFGameSaveFilesGetRemainingQuota` to enumerate `localFileFolderSet` each call (guard with mutex already protecting the structure).
5. Documentation: update `specs/PlayFabGameSave/docs/game-saves/limits.md` to describe new quota source and on-demand computation.

## Validation
- Unit tests: Title config quota 100MB, `localFileFolderSet` totals 30MB -> remaining 70MB.
- Add a new 10MB file, run upload/finalize (refresh set) -> remaining 60MB.
- Over-quota: quota 50MB, local total 60MB -> remaining -10MB.
- Missing title config entry -> sentinel (INT64_MAX) returned.
- Delete 5MB locally, sync -> remaining increases by 5MB.

## Migration Notes
Titles see no code change; `PFGameSaveFilesGetRemainingQuota` semantics shift but value generally similar. Potential slight differences; document in release notes.

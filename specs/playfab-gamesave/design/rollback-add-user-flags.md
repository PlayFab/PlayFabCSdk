<!-- Mini Spec: PFGameSave Rollback AddUser Flags -->
# PFGameSave Rollback Support via `PFGameSaveFilesAddUserOptions`

Author: TBD  
Date: 2025-09-22  
Status: Draft (review)  
Target Release: TBD  
Related Components: Client SDK (PFGameSave), GRTS (PFXGameSaveConfigRequest), Win32/Inproc provider  
Related Documents: [PFGameSaves_ConflictResolution_Flow_Spec.md](PFGameSaves_ConflictResolution_Flow_Spec.md)

## 1. Problem / Motivation

Developers need a way to roll a user back to a previous (known good or conflict-point) cloud game save manifest **at the time they add the user to the Game Saves system**. The client exposes explicit rollback intent via `PFGameSaveFilesAddUserOptions` flags.

Two rollback scenarios are supported:

1. Roll back to the last known good finalized manifest (most recent manifest that is not flagged as conflict / corruption source)
2. Roll back to the manifest version present at the time of the last conflict (to allow users to retry a branching progression decision)

## 2. Goals

* Expose new additive flags on `PFGameSaveFilesAddUserOptions` to request rollback semantics.
* Map those flags to GRTS (`GameSaveAPIProviderGRTS`) by passing through to existing `PFXGameSaveConfigRequest.InitFlagRollbackToLastKnownGood` / `InitFlagRollbackToLastConflict`.
* Implement equivalent behavior for the Win32/in-proc (`GameSaveAPIProviderWin32`) provider by selecting the correct target manifest before normal download logic runs.
* Preserve current default behavior (sync to latest) when no new flags are set.
* Be non-breaking for existing titles (enum is flag-style; old binaries still compile; behavior is opt-in).
* Provide deterministic outcomes & clear error handling when the requested rollback version does not exist.

## 3. Non-Goals

* Adding UI to let the player pick rollback targets post-initialization.
* Mid-session rollback (after `AddUserWithUiAsync` succeeds) — requires full uninitialize/reinitialize cycle and is out of scope.
* Extending upload APIs for rollback; rollback applies only at initial Add User sync path.

## 4. Public API Changes

### 4.1 `PFGameSaveFilesAddUserOptions` (C++ enum flags)

Add two flag values:

```
    enum class PFGameSaveFilesAddUserOptions : uint32_t
    {
        None = 0x00,
        RollbackToLastKnownGood = 0x01,      // Sync against last known good finalized manifest
        RollbackToLastConflict = 0x02,       // Sync against manifest at time of last conflict
        // Future flags reserved (0x04, 0x08, ...)
    };
```

Validation rule: At most one rollback flag may be specified per call. If both are provided simultaneously return `E_INVALIDARG` (fail fast before async begins) — see §6.5.

### 4.2 Title Configuration Gating

The service can disable either rollback path for a title via the Title Configuration object returned inline with the initial manifest enumeration:

Source: `ListManifestsResponse.TitleConfiguration`

Fields used:
* `DisableClientRollbackToLastKnownGood`
* `DisableClientRollbackToLastConflictLoser`
Behavior (Win32 & GRTS providers):
1. Public API validation (mutual exclusivity) runs first; if both rollback flags are set the call fails with `E_INVALIDARG` (gating flags are not consulted).
2. After `ListManifests` succeeds, the title configuration in that response is inspected inline (no rollback-specific global cache is required).
3. Before manifest selection / provider-specific initialization, requested rollback option bits are masked out if the corresponding disable flag is true:
    * If `DisableClientRollbackToLastKnownGood` is true, clear `RollbackToLastKnownGood`.
    * If `DisableClientRollbackToLastConflictLoser` is true, clear `RollbackToLastConflict`.
4. Clearing a bit is silent (trace-level informational log only) and treated exactly as if the caller never requested that rollback path.
5. If both bits are cleared (either by caller omission or by gating) normal latest-manifest (L) behavior proceeds.

Rationale: Configuration in `ListManifestsResponse` appears early enough in the Add User flow to support inline gating without maintaining additional rollback-specific global state.

Edge Cases:
* Missing / null `TitleConfiguration`: treat as all features enabled (no flags cleared).
* Repeated AddUser calls: gating is re-evaluated per call using the freshly returned `ListManifestsResponse`.
* Service returns both disable flags true: all rollback intents neutralized.
* Service returns conflicting / malformed values: only boolean interpretation applied; no failure surfaces to caller.

Testing Notes:
* Inject mock `ListManifestsResponse` with combinations of disable flags and assert (a) global state updates, (b) Win32 provider clears option bits, (c) selection falls back to latest when fully gated.
* Ensure mutual exclusivity failure still triggers prior to any attempt to read or apply gating flags.

### 4.2 Documentation Updates

`PFGameSaveFilesAddUserWithUiAsync` docs will gain a subsection “Rollback Options” describing semantics, precedence, and errors.

## 5. Behavior: GRTS Provider (`GameSaveAPIProviderGRTS`)

| AddUser Option | PFXGameSaveConfigRequest Flag Set | Net Effect |
|----------------|-----------------------------------|-----------|
| None | (no rollback flags) | Current behavior: target latest finalized manifest (or pending if reconnect) |
| RollbackToLastKnownGood | InitFlagRollbackToLastKnownGood | GRTS selects last good finalized manifest and treats it as the baseline for download/merge |
| RollbackToLastConflict | InitFlagRollbackToLastConflict | GRTS selects manifest at time of last recorded conflict |

Edge case mapping and final selection is performed by GRTS. The client simply forwards the flag bit(s). If both flags are (incorrectly) set client rejects before issuing request.

## 6. Behavior: Win32 / In-Process Provider (`GameSaveAPIProviderWin32`)

Since the Win32 provider currently enumerates manifests and picks “latest finalized” (then optionally pending) we must interpose logic early in `AddUserWithUiAsync` flow (before `PreparingForDownload` transitions). Pseudocode insertion point: right after verifying `syncState == NotStarted` and before setting `PreparingForDownload`.

### 6.1 Manifest Selection Strategy (Win32/In-Process) – With Title Config Gating

Metadata Inputs:
* `IsKnownGood` (bool)
* `IsWinner` (bool) – exactly one manifest in a conflict pair is winner; the other is loser (`IsWinner == false`).
* `ConflictingVersion` (version id) – set when a manifest participated in a conflict; both winner and loser have this field referencing the opposing version.

Definitions:
* Latest Finalized (L): Highest finalized manifest (simple max-version over all finalized manifests; excludes conflict losers when computing L).
* Last Known Good (KG): Highest finalized manifest with `IsKnownGood == true`. If none exist, fallback to L.
* Conflict Loser (CL): A finalized manifest marked `IsWinner == false`.
* Last Conflict (LC): Loser associated with the most recent conflict pair. Recency is determined by computing `pairHigh = max(loser.Version, loser.ConflictingVersion)` for each loser; the loser whose pair yields the largest `pairHigh` is selected. If no loser exists, fallback to L.

Selection algorithm (after removing any user bits gated off by title config):
```
if (option & RollbackToLastKnownGood)
    target = (KG found) ? KG : L;
else if (option & RollbackToLastConflict)
    target = (most recent conflict loser found) ? that loser : L;
else
    target = L;
```

Implementation note: Current code performs independent scans:
* L: scan for highest finalized manifest version
* KG: scan for highest finalized with `IsKnownGood`
* LC: scan finalized losers computing `pairHigh = max(loser.Version, loser.ConflictingVersion)` and select the loser with greatest `pairHigh`
If the requested rollback target (KG or LC) is not found the logic traces a fallback message and proceeds with L.

Validation: only one rollback flag may be specified (see §4.1 / §6.3). If both present return `E_INVALIDARG` before any manifest enumeration work.

### 6.2 Data Sources Needed
* Manifest enumeration including per-manifest fields: `Version`, `IsKnownGood`, `IsWinner`, and `ConflictingVersion`.

Notes:
* A rollback choice is not persisted; sessions without flags always start from L.
* If pruning removes older manifests (including losers) the logic naturally falls back to L.

### 6.3 Conflict Resolution Flow

Purpose
For detailed information about the conflict resolution flow implementation, including the deterministic winner/loser assignment process, conflict detection, upload strategies, and service persistence behavior, refer to the comprehensive specification in **[PFGameSaves_ConflictResolution_Flow_Spec.md](PFGameSaves_ConflictResolution_Flow_Spec.md)**.

Key points relevant to rollback functionality:

* **Winner/Loser Assignment**: Exactly one manifest in each conflict pair is marked as winner (`IsWinner = true`), with the other marked as loser (`IsWinner = false`)
* **Bidirectional Pairing**: Both manifests set `ConflictingVersion` to reference each other, enabling conflict pair reconstruction
* **Rollback Target Selection**: `RollbackToLastConflict` selects the most recent conflict loser (highest-version manifest with `IsWinner = false`)
* **Fallback Behavior**: When no conflict losers exist, rollback falls back to latest finalized manifest (L)

The conflict resolution flow ensures that both branches of a conflict are preserved regardless of user choice, enabling future rollback to either the winning or losing branch as needed.

### 6.4 Setting `IsKnownGood`

Objective
Provide a deterministic client rule for when a manifest is marked `IsKnownGood`, yielding a dependable rollback anchor without extra service calls.

Core Principle
Only a manifest that has: (1) been fully downloaded & loaded by the title, and then (2) successfully produced at least one newer finalized successor upload, becomes Known Good. Promotion happens at the time that successor finalizes, marking the loaded baseline from which it was derived (even if multiple additional finalized manifests were produced later in the same session before any reload occurred).

Terminology
* Active Baseline: The manifest the game has currently loaded (result of AddUser sync).
* Successor (V_new): The newly finalized manifest produced from modifications to the Active Baseline.
* Candidate: An Active Baseline that could be promoted when its successor finalizes.
* Known Good (KG): A manifest with `IsKnownGood = true`.

Promotion Algorithm
1. Start of session / after conflict resolution: set Active Baseline = V_cloud (or winner).
2. Title uploads changes producing finalized successor V_new.
3. On successful finalization of V_new: if Active Baseline (call it V_prev) is not already known good, set `V_prev.IsKnownGood = true`.
    * Note: V_prev may have been the loser of an earlier conflict pair if the session explicitly chose/loaded it (e.g., user-selected rollback or loser branch load). Conflict loser status does not disqualify promotion once it has been the Active Baseline and a successor finalized.
4. Do NOT mark V_new yet. It must first become the Active Baseline in a future load cycle and itself produce a further successor.
5. Next session (or reconnect) that downloads V_new sets Active Baseline = V_new; repeat from step 2.

State Diagram (conceptual)
UNVERIFIED (just uploaded, not yet loaded) -> ACTIVE BASELINE (loaded) -> PROMOTED (after a successor upload succeeds).

Why This Works
* Ensures every Known Good manifest has survived a full round trip: download -> game logic execution -> successor derivation.
* Avoids labeling an unexercised manifest (fresh upload never loaded) as safe.
* Allows multiple uploads in one session without intermediate downloads: only the originally loaded baseline is promoted; later rapid-fire uploads remain unverified until loaded in a future session.

Edge / Special Cases
* First manifest ever: no predecessor to promote.
* Multiple successive uploads without session restart: only the first session baseline may be promoted; intermediate successors wait until they are later loaded and produce their own successor.
* Failed or canceled upload: no promotion.
* Session ends after creating V_new but before loading it later: V_new stays unverified.
* Pruned predecessor before promotion opportunity: no retroactive marking; chain simply shortens.

Rollback Implications
* Last Known Good may lag the latest version by multiple steps if many uploads occurred without reload.
* RollbackToLastKnownGood provides a stable, exercised anchor rather than merely the immediate predecessor.

### 6.5 Error Handling

* If both rollback flags: return `E_INVALIDARG` immediately.
* If requested target version resolves to same as L (no earlier viable version) proceed normally (idempotent).

## 7. UX & UI Callbacks

No new mandatory UI. Existing AddUser progress / conflict dialogs function unchanged. A rollback that selects an older manifest may still surface subsequent conflicts if local files diverged from the chosen baseline.

## 8. Edge Cases Summary

| Scenario | Expected Result |
|----------|-----------------|
| Both flags set | Fail fast `E_INVALIDARG` |
| RollbackToLastConflict set but no loser manifest exists | Fallback to latest (L) |
| RollbackToLastKnownGood set but no manifest has `IsKnownGood` | Fallback to latest (L) |
| Target resolves to same as latest | Proceed (idempotent) |
| Title config disables selected rollback mode | Option bit ignored; proceed with remaining logic |
| Reconnect pending manifest exists | Ignore pending; chosen rollback target used |
| Inconsistent conflict pairing (one side missing or missing reciprocal `ConflictingVersion`) | Treat present manifest as winner (eligible for L); log diagnostic; loser selection ignores orphan |
| All manifests marked losers (should not occur) | Pick highest version as L; log error trace |

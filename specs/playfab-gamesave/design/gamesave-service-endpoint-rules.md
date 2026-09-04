# GameSave Service Endpoint Rules

Comprehensive reference for the validation rules and business logic enforced by each GameSave service endpoint. Derived from `src/FrontDoor/ServiceLogic/ManifestServiceLogic.cs` and related contracts.

---

## Manifest Lifecycle

```
[Initialized] ──InitiateUpload──▶ [Uploading] ──FinalizeManifest──▶ [Finalized]
                                                                        │
                                                     ┌─────────────────┤
                                                     ▼                  ▼
                                              DeleteManifest     UpdateManifest
                                                     │          (MarkAsKnownGood)
                                                     ▼
                                             [PendingDeletion] ──cleanup──▶ DELETED

[Finalized] ──RollbackToManifest──▶ NEW [Finalized] (version = max + 1)
[Any]       ──FileValidation──▶ [Quarantined]
```

---

## ListManifests

**Returns** all manifests for a player/title/workspace, plus `NextAvailableVersion` and `TitleConfiguration`.

### Filtering
- Always excludes manifests in `PendingDeletion` state where `CleanupStartTime < now` (near-cleanup).
- If `IncludeUnavailable = false` (default): also excludes `PendingDeletion` and `Quarantined`.

### TitleConfiguration fields returned
| Field | Purpose |
|-------|---------|
| `PerPlayerQuotaBytes` | Cloud storage quota per player |
| `DisableClientRollbackToLastKnownGood` | Prevents client rollback to last known good |
| `DisableClientRollbackToLastConflictLoser` | Prevents client rollback to last conflict loser |
| `DisableUnsignaledOutOfProcessUpload` | Controls out-of-process upload behavior |

### NextAvailableVersion
- `max(all manifest versions) + 1`, or `0` if no manifests exist.

---

## InitializeManifest

Creates a new manifest in `Initialized` state. This represents acquiring the active device lock.

### Request fields
| Field | Required | Description |
|-------|----------|-------------|
| `Version` | Yes | Version number for the new manifest |
| `BaseVersion` | Yes | Version this manifest is based on |
| `Metadata` | Yes | Device metadata (DeviceId, DeviceName, DeviceType, DeviceVersion) |
| `GeoLocation` | No | Used for storage account region selection |
| `PlayerIdentityProvider` | No | Identity provider name |

### BaseVersion rules
1. If `BaseVersion == Version` or `BaseVersion == 0`: no base version validation is performed. This means the manifest has no dependency on a previous manifest.
2. If `BaseVersion != Version` and `BaseVersion != 0`:
   - `BaseVersion` **must be less than** `Version` → otherwise returns `BaseVersionNotAvailable`
   - The base manifest **must exist** → otherwise returns `BaseVersionNotAvailable`
   - The base manifest **must be in `Finalized` status** → otherwise returns `BaseVersionNotAvailable`

> **Note:** There is a TODO to remove the `BaseVersion != 0` special case after the mainserver client is updated to send the current version instead of 0.

### Other rules
- If a manifest with the same version already exists → returns `ManifestVersionAlreadyExists` (409).
- **Idempotency**: If the existing manifest is in `Initialized` state with matching metadata, the duplicate request returns `Success`.
- A storage account is assigned based on geo-location.

---

## InitiateUpload

Transitions a manifest from `Initialized` to `Uploading` and returns SAS upload URLs for each file.

### Rules
- Manifest **must be in `Initialized` or `Uploading` state** → otherwise `ManifestUpdatesNotAllowed`.
- Files must be new to this workspace. If a file already exists in the FileMap with a **different** `OriginatingVersion` → `FileAlreadyExists`. Re-uploading the same file for the same version is allowed (returns a new SAS token).
- Total files across all uploads for this manifest must not exceed **100** → `ManifestFilesLimitExceeded`.
- `TestOnlyCustomTokenExpirationSeconds` is only allowed for test titles → `OperationNotAllowedForTitle`.

### Side effects
- Manifest status set to `Uploading`.
- FileMap updated with new file entries (storage path, expiration time, originating version).
- File validation processing enqueued (delayed until after SAS token expiration + 3 min buffer).
- SAS tokens valid for **60 minutes** (default).

---

## UpdateManifest

Updates specific fields on an existing manifest. At least one update must be requested.

### Allowed updates by manifest state

| Field | Allowed in `Initialized`/`Uploading` | Allowed in `Finalized` | Allowed in `PendingDeletion` (if finalized) | Blocked in `Quarantined` |
|-------|--------------------------------------|----------------------|---------------------------------------------|------------------------|
| `UploadProgress` | ✅ | ❌ `ManifestUploadProgressUpdateNotAllowed` | ❌ | ❌ |
| `ManifestDescription` | ✅ | ❌ `ManifestDescriptionUpdateNotAllowed` | ❌ | ❌ |
| `MarkAsKnownGood` | ❌ `NotFinalizedManifestNotEligibleAsKnownGood` | ✅ | ✅ (if `FinalizationTimestamp` exists) | ❌ |

### MarkAsKnownGood behavior
- When set to `true`, manifest status is reset to `Finalized` (even from `PendingDeletion`).
- Emits a `VersionMarkedKnownGood` PlayFab event.
- Idempotent: no-op if the value matches the current state.

---

## FinalizeManifest

Transitions a manifest from `Uploading` to `Finalized`. This completes an upload.

### Request fields
| Field | Required | Description |
|-------|----------|-------------|
| `FilesToFinalize` | Yes | Array of files (1–100) with name and expected size |
| `Force` | No | Skip "latest version" validation |
| `MarkBaseAsKnownGood` | No | Mark the base manifest as known good |
| `ManifestDescription` | No | Short description (max 1024 chars) |
| `Conflict` | No | Conflict metadata (`IsWinner`, `ConflictingVersion`) |

### Validation rules

1. **Self-conflict check**: `Conflict.ConflictingVersion` cannot equal `Version` → `ManifestNotEligibleAsConflictingVersion`.

2. **Base self-reference + KnownGood**: If `BaseVersion == Version`, cannot set `MarkBaseAsKnownGood = true` → `NotFinalizedManifestNotEligibleAsKnownGood`.

3. **Manifest status**: Must be `Uploading` → `ManifestUpdatesNotAllowed`. (Already-finalized manifests return `Success` if the request matches the existing finalization — idempotency.)

4. **Latest version check**: Unless `Force` is set OR the manifest is a conflict loser (`Conflict != null && !IsWinner`), the manifest must be the latest non-`PendingDeletion` version → `NewerManifestExists` (409).

5. **File validation**: Every file in `FilesToFinalize` must exist in the FileMap (previously uploaded via InitiateUpload, possibly for a different version) → `UnknownFileInManifest`.

6. **Quota enforcement**: Total file size must be within per-player quota → `DataStorageQuotaExceeded`.

### MarkBaseAsKnownGood behavior
- Only applied if the base manifest exists and is not already known good.
- If the base is not `Quarantined`, its status is reset to `Finalized`.
- Sets `KnownGood = true` on the base manifest.
- Emits a `VersionMarkedKnownGood` PlayFab event.
- If the base manifest is not found, a metric is emitted but no error is returned.

### Conflict handling
When `Conflict` is provided and the conflicting manifest exists:
- **Current manifest** gets: `Conflict = { IsWinner, ConflictingVersion, timestamps }`.
- **Conflicting manifest** gets: inverse `IsWinner`, with `ConflictingVersion` pointing back. Both are retained for **7 days** (`RetainedUntilTimestamp`).
- Both manifests are updated atomically in a transaction.
- If the conflicting manifest is **not found**: the finalization still proceeds (logged but not blocked). *(See disabled checks below.)*

### Disabled validation checks (Bug 60132083)

Three checks are currently **commented out** due to a client bug where the client creates a new version with `BaseVersion == ConflictingVersion` after a conflict:

| Check | Intended behavior | Current behavior |
|-------|-------------------|-----------------|
| Conflict with base manifest | `ConflictingVersion` cannot equal `BaseVersion` | ⚠️ **Logged but allowed** |
| Conflicting manifest must be finalized | A conflict with an unfinalized manifest is meaningless | ⚠️ **Logged but allowed** |
| Losing manifest must find winner | If the losing manifest can't find the winner, fail | ⚠️ **Logged but allowed** |

> Tracked by: Bug 60132083, Deliverable 60133284

---

## GetManifestDownloadDetails

Returns SAS download URLs for all files in a finalized manifest.

### Rules
- Manifest must be in `Finalized` state.
  - If `Quarantined` → `ManifestVersionQuarantined`.
  - If not finalized → `ManifestVersionNotFinalized`.
- All files listed in the manifest must exist in the FileMap → `InternalServerError` (500) if any are missing (indicates a service bug).

### Side effects
- SAS tokens valid for **60 minutes**.
- `LastDownloadRequestTimestamp` updated (best-effort, errors ignored).

---

## DeleteManifest

Marks a manifest for deletion (soft delete). Actual cleanup is handled by the background cleanup processor.

### Rules
- Sets status to `PendingDeletion`.
- Idempotent: already-deleted manifests return `Success`.

---

## RollbackToManifest

Creates a new `Finalized` manifest that is a copy of a specified older manifest.

### Request fields
| Field | Required | Description |
|-------|----------|-------------|
| `RollbackVersion` | Yes | Version of the manifest to rollback to |
| `RollbackReason` | No | Free-text reason (max 100 chars) |
| `CallingEntity` | No | Entity making the request (`{type}!{id}`) |

### Eligibility rules

1. **Target must exist** → `ManifestNotFound`.
2. **Target must have been finalized** (`FinalizationTimestamp != null`) → `ManifestNotEligibleForRollback`.
3. **Permission check based on caller**:
   - **Title entities** (server-to-server): Can rollback to **any** finalized manifest.
   - **Non-title entities** (player clients): Can only rollback to:
     - **Last Known Good** (if `DisableClientRollbackToLastKnownGood` is `false`), OR
     - **Last Conflict Loser** (if `DisableClientRollbackToLastConflictLoser` is `false`)
     - If neither applies → `ManifestNotEligibleForRollback`.

### "Last Known Good" definition
The manifest with `KnownGood == true` that has the **highest version** among all known good manifests. Status is intentionally **ignored** — a `PendingDeletion` manifest can still be the last known good (prevents gaming the system by deleting manifests).

### "Last Conflict Loser" definition
The manifest with `Conflict != null && !IsWinner` that has the **highest version** among all conflict losers.

### New manifest creation
| Field | Value |
|-------|-------|
| `Version` | `max(all existing versions) + 1` |
| `BaseVersion` | `RollbackVersion` (the source manifest) |
| `Status` | `Finalized` (immediately) |
| `Files` | Copied from old manifest |
| `Conflict` | `null` (cleared) |
| `GeneratedByRollback` | `true` |
| `RollbackReason` | From request |
| `CreationTimestamp` | Now |
| `FinalizationTimestamp` | Now |

### Conflict state flipping
If rolling back to a **conflict loser** (the old manifest has `Conflict.IsWinner == false`):
- The old (rolled-back-to) manifest is marked `IsWinner = true`.
- The manifest it conflicted with: if its `Conflict.ConflictingVersion` still points to the old manifest AND it is currently the winner, it is flipped to `IsWinner = false`.
- All updates (new manifest creation + conflict flips) happen atomically in a transaction.

---

## Common Validation (All Endpoints)

- **Calling permissions**: Every endpoint checks title-level permissions via `EnforceCallingPermissions`. Disabled titles are blocked.
- **Conflict retries**: `InitiateUpload`, `UpdateManifest`, `FinalizeManifest`, and `RollbackToManifest` automatically retry up to **2 times** on `ConflictUpdatingManifest` (Cosmos DB 409 conflicts).
- **Near-cleanup filtering**: Any manifest in `PendingDeletion` with `CleanupStartTime < now` is treated as non-existent by read and list operations.

---

## File Constraints

| Constraint | Value |
|------------|-------|
| Max files per manifest | 100 |
| Max file size (reported) | 1 GB (1,000,000,000 bytes) |
| File name pattern | `^[a-zA-Z0-9\-_\.]+$` |
| File name length | 1–50 characters |
| Manifest description length | Max 1024 characters |
| SAS token duration | 60 minutes |
| Conflict retention | 7 days |

---

## Status Code Quick Reference

| Status Code | HTTP | Meaning |
|-------------|------|---------|
| `Success` | 200 | Operation succeeded |
| `ManifestNotFound` | 404 | Manifest does not exist |
| `ManifestVersionAlreadyExists` | 409 | Version already taken |
| `ConflictUpdatingManifest` | 409 | Cosmos DB write conflict (retried automatically) |
| `NewerManifestExists` | 409 | Cannot finalize an older version |
| `ManifestUpdatesNotAllowed` | 400 | Wrong manifest state for this operation |
| `ManifestVersionNotFinalized` | 400 | Expected finalized manifest |
| `ManifestVersionQuarantined` | 400 | Manifest quarantined by file validation |
| `BaseVersionNotAvailable` | 400 | Base manifest missing or not finalized |
| `ManifestNotEligibleForRollback` | 400 | Target not eligible for rollback |
| `ManifestNotEligibleAsConflictingVersion` | 400 | Invalid conflict reference |
| `NotFinalizedManifestNotEligibleAsKnownGood` | 400 | Cannot mark unfinalized manifest as known good |
| `ManifestFilesLimitExceeded` | 400 | More than 100 files |
| `UnknownFileInManifest` | 400 | File not found in FileMap |
| `FileAlreadyExists` | 400 | File belongs to different version |
| `DataStorageQuotaExceeded` | 400 | Over per-player quota |
| `NoUpdatesRequested` | 400 | UpdateManifest with no fields |
| `InternalServerError` | 500 | Service-side error |

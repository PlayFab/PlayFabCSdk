# PlayFab GameSave Service Internals Guide

> How the PlayFab GameSave service works under the hood.
> For AI agents debugging SDK ↔ service integration, understanding API behavior, or tracing save flows.

**Source code**: `C:\git\GS.GameSave` (READ ONLY — team does not own this repo)
**Solution file**: `src/GameSave.sln` (17 projects)
**Runtime**: .NET / ASP.NET Core, deployed to Kubernetes

## What the Service Is

The PlayFab GameSave service is the **cloud backend** that stores, versions, and manages game save data for players. When a game (via the SDK or GRTS) uploads save data, the service:

- Tracks versioned **manifests** (metadata about each save version) in Cosmos DB
- Stores actual **game save files** in Azure Blob Storage (one storage account per title per region)
- Enforces **quotas** (default 256 MB per player)
- Detects **conflicts** when multiple devices upload concurrently
- Supports **rollback** to previous versions (last known good, last conflict loser)
- Manages **title onboarding** and storage account lifecycle
- Processes **billing** from Azure resource usage events
- Handles async **cleanup** of deleted manifests, players, and storage accounts

The SDK (PlayFab.C) and GRTS are **separate codebases** that call this service's REST API. The service has no knowledge of the SDK's internal state machines — it provides a stateless API and enforces invariants server-side.

## Architecture

```
                        Clients (SDK / GRTS)
                              │
                              │ HTTPS (all POST, JSON body)
                              ▼
┌──────────────────────────────────────────────────────────────────┐
│                        FrontDoor (API)                           │
│  ManifestController  QuotaController  TitleManagementController  │
│  AdminController     PlayerManagementController                  │
│         │                                                        │
│         ▼                                                        │
│  ManifestServiceLogic  QuotaServiceLogic  TitleMgmtServiceLogic  │
│  AdminServiceLogic     PlayerMgmtServiceLogic                    │
│  TitleConfigEnforcer   StorageMapper                             │
│  TitleConfigCache      AccountMapCache                           │
└─────────┬──────────────────┬──────────────────┬─────────────────┘
          │                  │                  │
          ▼                  ▼                  ▼
   ┌─────────────┐   ┌────────────┐   ┌──────────────────┐
   │ManifestMgr  │   │FileMapMgr  │   │GameSaveBlobMgr   │
   │TitleConfigMgr│   │TitleMapMgr │   │(SAS token gen)   │
   └──────┬──────┘   └─────┬──────┘   └────────┬─────────┘
          │                 │                   │
          ▼                 ▼                   ▼
   ┌────────────────────────────┐     ┌──────────────────┐
   │  Cosmos DB (gamesave-db)   │     │ Azure Blob Storage│
   │  Containers:               │     │ Container: gamesave│
   │  - manifests               │     │ Path: {pid}/{tid}/ │
   │  - file-maps               │     │   {wid}/{filename} │
   │  - storage-account-maps    │     └──────────────────┘
   │  - title-configs           │
   │  - change-feed-leases      │
   └─────────┬──────────────────┘
             │ Change Feed
             ▼
   ┌───────────────────────────────────────────────────────┐
   │  ManifestChangeFeedProcessor    StorageAccountCFP     │
   │  (trim versions, queue cleanup)  (assign SAs, delete) │
   └──────────────────┬────────────────────────────────────┘
                      │ Queue Messages
                      ▼
   ┌────────────────────────────────────────────────────────┐
   │                  QueueProcessor                        │
   │  ValidateUploadedFiles  CleanupManifest  CleanupPlayer │
   │  CompleteStorageAccountAssignment  CreateNewStorageAccts│
   │  DeleteStorageAccounts  CleanupPlayerDataTransfer      │
   └────────────────────────────────────────────────────────┘

   ┌────────────────────────────────────────────────────────┐
   │  AzureBillingProcessor (Event Hub → billing meters)    │
   └────────────────────────────────────────────────────────┘

   ┌────────────────────────────────────────────────────────┐
   │  TitleOnboardingManager (CLI tool → FrontDoor API)     │
   └────────────────────────────────────────────────────────┘
```

### Key Source Directories

| Directory | Contents |
|-----------|----------|
| `src/FrontDoor/` | API surface: controllers, service logic, caches, geo-location, startup |
| `src/FrontDoor/Controllers/` | 5 controllers + health check (all `[HttpPost]`, `[Authorize]`) |
| `src/FrontDoor/ServiceLogic/` | Business logic: `ManifestServiceLogic`, `QuotaServiceLogic`, etc. |
| `src/FrontDoor/ServiceLogic/Caches/` | `TitleConfigCache` (5-min TTL), `AccountMapCache` (24-hr TTL) |
| `src/FrontDoor.Contracts/` | Request/response DTOs, `GameSaveStatusCode`, validation attributes |
| `src/FrontDoor.Contracts/Common/` | Base classes, `GameSaveResult<T>`, `GameSaveStatusCode` enum |
| `src/FrontDoor.Contracts/TitleConfig/` | Title onboarding/config contracts |
| `src/DataAccess/` | Cosmos DB repositories: `ManifestManager`, `FileMapManager`, `TitleConfigManager`, `TitleMapManager` |
| `src/DataAccess.Contracts/` | Data models: `ManifestDocument`, `FileMapDocument`, `TitleConfigDocument`, `StorageAccountDocument` |
| `src/AzureBlobStorage/` | `AzureBlobManager`, `AzureBlobManagerFactory`, User Delegation SAS generation |
| `src/Common/` | Constants, `AzureRegion` enum, billing meters, metrics, logging helpers |
| `src/QueueProcessor/` | Async message processing: file validation, manifest cleanup, player deletion, storage account ops |
| `src/ChangeFeedProcessor/` | Shared library: `ManifestDocumentProcessingLogic`, `StorageAccountMapsProcessingLogic` |
| `src/ManifestChangeFeedProcessor/` | Deployable service: monitors `manifests` container change feed |
| `src/StorageAccountChangeFeedProcessor/` | Deployable service: monitors `storage-account-maps` change feed |
| `src/AzureBillingProcessor/` | PAv2 Event Hub → PlayFab billing meters |
| `src/StorageAccountManagement/` | ARM client for creating/deleting/tagging Azure storage accounts |
| `src/TitleOnboardingManager/` | CLI tool for approving/rejecting title onboarding |
| `src/E2ETests/` | End-to-end tests |
| `src/Test.Common/` | Shared test utilities |

## API Surface (FrontDoor)

All endpoints are **`[HttpPost]`** with **JSON body** — RPC-style over REST. All controllers use `[Authorize]` (certain dev environments allow anonymous). All return `GameSaveResult<T>` which maps `GameSaveStatusCode` to HTTP status codes via `[GameSaveStatusCode]` attributes.

### ManifestController — Core Save Operations

| Route | Request | Response | Purpose |
|-------|---------|----------|---------|
| `POST /Manifest/InitializeManifest` | `InitializeManifestRequest` | `InitializeManifestResponse` | Create a new manifest version (phase 1 of save) |
| `POST /Manifest/InitiateUpload` | `InitiateUploadRequest` | `InitiateUploadResponse` | Get SAS upload URLs for files (phase 2 of save) |
| `POST /Manifest/FinalizeManifest` | `FinalizeManifestRequest` | `FinalizeManifestResponse` | Commit the upload, conflict detection (phase 3 of save) |
| `POST /Manifest/ListManifests` | `ListManifestsRequest` | `ListManifestsResponse` | List all versions for a player + title config |
| `POST /Manifest/GetManifestDownloadDetails` | `GetManifestDownloadDetailsRequest` | `GetManifestDownloadDetailsResponse` | Get SAS download URLs for files |
| `POST /Manifest/DeleteManifest` | `DeleteManifestRequest` | `DeleteManifestResponse` | Mark manifest for async deletion |
| `POST /Manifest/UpdateManifest` | `UpdateManifestRequest` | `UpdateManifestResponse` | Update progress, description, or KnownGood flag |
| `POST /Manifest/RollbackToManifest` | `RollbackToManifestRequest` | `RollbackToManifestResponse` | Create new version from an old finalized manifest |

### QuotaController

| Route | Request | Response | Purpose |
|-------|---------|----------|---------|
| `POST /Quota/GetQuotaForPlayer` | `GetQuotaForPlayerRequest` | `GetQuotaForPlayerResponse` | Query remaining storage quota (TotalBytes, AvailableBytes) |

### TitleManagementController

| Route | Request | Response | Purpose |
|-------|---------|----------|---------|
| `POST /TitleManagement/GetConfigForTitle` | `GetConfigForTitleRequest` | `GetConfigForTitleResponse` | Read title configuration (quota, rollback flags, etc.) |
| `POST /TitleManagement/RequestOnboarding` | `RequestOnboardingRequest` | `RequestOnboardingResponse` | Start onboarding (sets status to Pending) |
| `POST /TitleManagement/ApproveOnboarding` | `ApproveOnboardingRequest` | `ApproveOnboardingResponse` | Admin approval for title |
| `POST /TitleManagement/RejectOnboarding` | `RejectOnboardingRequest` | `RejectOnboardingResponse` | Admin rejection |
| `POST /TitleManagement/UpdateConfigForTitle` | `UpdateConfigForTitleRequest` | `UpdateConfigForTitleResponse` | Update per-title feature flags |
| `POST /TitleManagement/DeleteTitle` | `DeleteTitleRequest` | `DeleteTitleResponse` | Mark title for TTL-based deletion (7 days) |

### AdminController

| Route | Request | Response | Purpose |
|-------|---------|----------|---------|
| `POST /Admin/CreateStorageAccounts` | `CreateStorageAccountsRequest` | `CreateStorageAccountsResponse` | Enqueue batch storage account creation |
| `POST /Admin/UpdateInternalConfigForTitle` | `UpdateInternalConfigForTitleRequest` | `UpdateInternalConfigForTitleResponse` | Update internal config (quota, retention, validation %) |

### PlayerManagementController

| Route | Request | Response | Purpose |
|-------|---------|----------|---------|
| `POST /PlayerManagement/DeletePlayer` | `DeletePlayerRequest` | `DeletePlayerResponse` | Enqueue async player data deletion |
| `POST /PlayerManagement/TransferDataForPlayer` | `TransferDataForPlayerRequest` | `TransferDataForPlayerResponse` | Transfer save data between player entities |

## Data Flow: Save Operation (3-Phase Pipeline)

The service enforces a strict **Initialize → Upload → Finalize** pipeline for every save version.

```
Client                            Service (FrontDoor)                    Storage
  │                                    │                                    │
  │ 1. ListManifests ────────────────► │                                    │
  │ ◄──── manifests[] + NextVersion ── │ ← Cosmos: list manifests           │
  │                                    │                                    │
  │ 2. InitializeManifest(v=N) ──────► │                                    │
  │ ◄──── Success (or AlreadyExists) ─ │ → Cosmos: create ManifestDoc       │
  │                                    │   (Status=Initialized)             │
  │                                    │ → AssignStorageAccount (if needed)  │
  │                                    │                                    │
  │ 3. InitiateUpload(v=N, files[]) ─► │                                    │
  │ ◄──── SAS upload URLs ──────────── │ → Cosmos: update FileMap           │
  │                                    │ → Cosmos: manifest→Uploading       │
  │                                    │ → Blob: generate write SAS tokens  │
  │                                    │                                    │
  │ 4. Upload files directly ─────────────────────────────────────────────► │
  │    (PUT to SAS URLs)               │                                    │
  │                                    │                                    │
  │ 5. FinalizeManifest(v=N) ────────► │                                    │
  │ ◄──── Success (Finalized) ──────── │ → Cosmos: manifest→Finalized       │
  │                                    │   (transactional: current +        │
  │                                    │    conflicting + base manifests)   │
  │                                    │ → Queue: ValidateUploadedFiles     │
  │                                    │                                    │
  │ 6. GetManifestDownloadDetails ───► │                                    │
  │ ◄──── SAS download URLs ────────── │ → Blob: generate read SAS tokens   │
```

### Phase 1: InitializeManifest

**File**: `ManifestServiceLogic.cs` lines 141–239

1. Enforce title onboarding status
2. Validate `BaseVersion` — must exist, be `Finalized`, not `Transferred`; `BaseVersion > Version` → `BaseVersionNotAvailable`
3. Assign storage account via `AzureRegionSelector` (geo-proximity to caller)
4. Create `ManifestDocument` in Cosmos DB with `Status = Initialized`
5. **Idempotency**: If version already exists with `Initialized` status and matching metadata → return Success (handles client retries)

### Phase 2: InitiateUpload

**File**: `ManifestServiceLogic.cs` lines 327–470

1. Read manifest — must be `Initialized` or `Uploading`
2. Read/create `FileMapDocument` from Cosmos
3. Validate files: check immutability (files from older versions can't be re-uploaded under same name), check `MaxAllowedManifestFiles` limit (100)
4. Assign files to deterministic blob paths: `{playerId}/{titleId}/{workspaceId}/{fileName}`
5. Update FileMap in Cosmos with file entries (storage account, path, SAS expiration)
6. Update manifest → `Status = Uploading`, add files to file set
7. Generate **User Delegation SAS** tokens (60-min default) for each file
8. Enqueue `ValidateUploadedFiles` message (delayed until SAS expires + 3 min)

**SAS Token Details**:
- Type: User Delegation SAS (not account-key SAS)
- Default duration: 60 minutes
- Permission: `CreateAndWrite` for uploads, `Read` for downloads
- Clock skew defense: `StartsOn = UtcNow - 1 minute`
- User delegation key cached per storage account with 6-day lifetime

### Phase 3: FinalizeManifest ⭐

**File**: `ManifestServiceLogic.cs` lines 612–954 — **THE critical method**

This is where conflict detection, version validation, and ETag-based concurrency all converge.

**Step-by-step flow**:

1. **Self-conflict check**: `ConflictingVersion == Version` → `ManifestNotEligibleAsConflictingVersion`
2. **Title config + permissions check**
3. **IsWinner correction** (critical for SDK integration, lines 629–665):
   - **SDK callers** (`IsCallerPlayFabSDK == true`): If `IsWinner == false` → set `dropAsConflictLoser = true` (manifest finalized as `PendingDeletion`, never visible as "latest")
   - **Non-SDK callers** (GRTS/platform): `IsWinner` value is **flipped** in-place (`true→false`, `false→true`) because GRTS/platform uses inverted semantics
   - **`IsCallerPlayFabSDK == null`**: Legacy path — no correction
4. **List all manifests** from Cosmos
5. **Already-Finalized idempotency**: If manifest is `Finalized` and matches request → return Success
6. **Status gate**: Must be `Uploading` → `ManifestUpdatesNotAllowed`
7. **Version-is-latest validation** (conflict detection): Unless `Force` flag set OR conflict loser, checks that no newer `Finalized` manifest exists → `NewerManifestExists`
8. **Conflict metadata**: Sets `ManifestConflict` on both current and conflicting manifests (mirrored `IsWinner` flags), with 7-day `RetainedUntilTimestamp`
9. **File validation**: Each file in `FilesToFinalize` must exist in the FileMap → `UnknownFileInManifest`
10. **Quota enforcement**: Total file size vs `PerPlayerQuotaBytes` → `DataStorageQuotaExceeded`
11. **Update FileMap** in Cosmos
12. **Set final status**: `Finalized` (or `PendingDeletion` if `dropAsConflictLoser`)
13. **MarkBaseAsKnownGood**: If requested, sets `KnownGood = true` on base version
14. **Transactional write**: All manifests (current, conflicting, base) written in a single Cosmos `TransactionalBatch` with ETags
15. **Emit telemetry**: PlayStream `VersionFinalized` event

**ETag/Concurrency**: `ExecuteWithConflictRetriesAsync` wraps the operation — retries up to 2 times on Cosmos ETag conflicts (`ConflictUpdatingManifest`). If all retries exhausted, the conflict status code is returned to the caller.

## Storage Architecture

### Cosmos DB — Metadata Store

Database: `gamesave-db`

| Container | Document Type | Partition Key | Document ID Pattern | Purpose |
|-----------|--------------|---------------|---------------------|---------|
| `manifests` | `ManifestDocument` | `[playerId, titleId]` (hierarchical) | `{workspaceId}_{version}` (e.g. `default_5`) | Versioned save metadata |
| `file-maps` | `FileMapDocument` | `[playerId, titleId]` (hierarchical) | `{workspaceId}` (always `"default"`) | File-to-blob mapping, per-player |
| `title-configs` | `TitleConfigDocument` | `[titleId]` | `{titleId}` | Per-title configuration |
| `storage-account-maps` | `StorageAccountDocument`, `TitleAccountsDocument` | `"Fixed"` (all in one partition) | `s-{name}` / `t-{titleId}` | Storage account pool + title assignments |
| `change-feed-leases` | (lease docs) | — | — | Change feed processor checkpoints |

#### ManifestDocument Key Fields

```
{
  "id": "default_5",            // {workspaceId}_{version}
  "dt": "Manifest",             // DocumentType
  "tid": "E18D7",               // TitleId
  "pid": "2814640093565666",    // PlayerId
  "wid": "default",             // WorkspaceId (always "default")
  "v": 5,                       // Version (ulong, monotonically increasing)
  "bv": 4,                      // BaseVersion
  "s": "Finalized",             // Status enum
  "sa": "gsgamesave01eus",      // StorageAccount name
  "sar": "EastUs",              // StorageAccountRegion
  "kg": true,                   // KnownGood flag
  "ct": "2026-01-15T...",       // CreationTimestamp
  "ft": "2026-01-15T...",       // FinalizationTimestamp
  "Files": ["file1.bin"],       // HashSet<string> of file names
  "trfs": 10240,                // TotalReportedFileSize
  "Conflict": {                 // ManifestConflict (nullable)
    "IsWinner": true,
    "ConflictingVersion": 3,
    "RetainedUntilTimestamp": "2026-01-22T..."
  },
  "GeneratedByRollback": false,
  "RollbackReason": null,
  "_etag": "\"0000...\"",       // Cosmos ETag for optimistic concurrency
  "ttl": -1                     // -1 = no expiry; set for soft-delete
}
```

#### ManifestStatus Values

| Status | Meaning |
|--------|---------|
| `Initialized` | Version created, upload not started |
| `Uploading` | Files being uploaded via SAS URLs |
| `Finalized` | Upload complete, this is a valid version |
| `PendingDeletion` | Marked for async cleanup (soft-delete) |
| `Quarantined` | Data integrity issue detected (file size mismatch) |

#### FileMapDocument

Stores the canonical mapping of file names to blob storage locations. The `Files` list is **gzip-compressed to base64** with a `"v1:"` prefix to reduce Cosmos RU costs.

```
{
  "id": "default",
  "dt": "FileMap",
  "tid": "E18D7",
  "pid": "2814640093565666",
  "files": "v1:H4sIAAAA...",   // Compressed FileMetadata list
  // Each FileMetadata:
  //   "n": "data.bin",         // Name
  //   "sa": "gsgamesave01eus", // StorageAccountName
  //   "fp": "{pid}/{tid}/default/data.bin", // FilePath (blob)
  //   "rs": 10240,             // ReportedSize
  //   "as": 10240,             // ActualSize (set by validation)
  //   "ov": 5,                 // OriginatingVersion
  //   "ue": "2026-01-15T..."   // UploadExpirationTime (SAS expiry)
}
```

#### TitleConfigDocument

```
{
  "id": "E18D7",
  "dt": "TitleConfig",
  "OnboardingStatus": "Onboarded",
  "PerPlayerQuotaBytes": 268435456,         // 256 MB default
  "PerPlayerMaxManifestVersionsRetained": 2,
  "FileValidationSamplingPercentage": 20,
  "DisableClientRollbackToLastKnownGood": false,
  "DisableClientRollbackToLastConflictLoser": false,
  "DisableUnsignaledOutOfProcessUpload": false
}
```

### Azure Blob Storage — File Store

- **Container name**: `"gamesave"` (all storage accounts)
- **Blob path**: `{playerId}/{titleId}/{workspaceId}/{fileName}`
- **Storage account naming**: `{prefix}{random}{regionCode}` (e.g., `gsgamesave7a3eus`)
- **Account type**: Standard_LRS
- **Soft-delete retention**: 7 days
- **Access**: Via User Delegation SAS tokens (never account keys)
- **One storage account per title per region** — auto-assigned on first use

### Storage Account Lifecycle

```
Pool (available, no title) ──► AssignStorageAccount (title assigned) ──► In Use
        ▲                              │
        │                              ▼ (Change Feed)
        │                     CompleteStorageAccountAssignment
        │                       (tag ARM resource, create replacement)
        │
  CreateNewStorageAccounts ◄── Admin creates batch via API
        │
        └── Storage Account Pool Refill

Title Deleted ──► MarkForDeletion (7-day TTL) ──► DeleteStorageAccounts
                                                    (ARM delete if empty,
                                                     re-queue if not empty)
```

## Queue Processing (Async Operations)

The `QueueProcessor` is a `BackgroundService` that polls an Azure Storage Queue.

| Operation | Triggered By | What It Does |
|-----------|-------------|--------------|
| `ValidateUploadedFiles` | FrontDoor (after SAS expiry + 3 min) | Reads actual blob sizes, compares to reported sizes, quarantines oversized manifests (30-day TTL), deletes orphaned files |
| `CleanupManifest` | ManifestChangeFeedProcessor (on `PendingDeletion`) | Deletes blobs for deleted manifests, updates FileMap entries. Delay: 30-min cleanup + 10-min enqueue |
| `CleanupPlayer` | FrontDoor (`DeletePlayer` API) | Full player wipe: all blobs, manifests, and FileMap |
| `CleanupPlayerDataTransfer` | FrontDoor (after transfer) | Verifies transfer, marks origin as Transferred, deletes orphaned blobs, rolls back failed transfers |
| `CompleteStorageAccountAssignment` | StorageAccountChangeFeedProcessor | Tags ARM resource with `PlayFabTitleId`, creates replacement account for pool |
| `CreateNewStorageAccounts` | Admin API | Batch-creates storage accounts for pool refill |
| `DeleteStorageAccounts` | StorageAccountChangeFeedProcessor (on title deletion) | Validates tag ownership, waits for empty (re-queues if not), deletes ARM resource + Cosmos docs |

**Worker pattern**: `Gaming.Core.QueueProcessor` polls messages in batches of 32. Returns `true` → message deleted; exception → message stays for retry. All processing is **idempotent**.

## Change Feed Processing

### ManifestChangeFeedProcessor

**Monitors**: `manifests` container change feed (500ms poll interval)
**Parallelism**: Up to 1000 parallel partition groups, sequential within each partition

**On manifest Create**:
- `RemoveDuplicateDeviceIdPendingManifestsAsync()` — marks older Initialized/Uploading manifests from the same device for deletion (cleanup stale uploads)

**On manifest Replace (status → Finalized or KnownGood set)**:
- `TrimRetainedManifestListAndMarkForDeletionAsync()` — keeps up to `DefaultMaxFinalizedManifestVersionsRetained` (2) finalized versions; preserves latest non-loser, latest KnownGood, conflict-loser manifests within retention window

**On manifest Replace (status → PendingDeletion)**:
- `QueueManifestDeletion()` — sends `CleanupManifest` message with delay

### StorageAccountChangeFeedProcessor

**Monitors**: `storage-account-maps` container change feed (500ms poll, ordered processing)

**On StorageAccountDocument (title assigned)**: Queues `CompleteStorageAccountAssignment`
**On TitleAccountsDocument (marked for deletion)**: Queues `DeleteStorageAccounts` with 7-day delay

## Conflict Detection & Resolution

### How Conflicts Are Detected

Conflict detection happens at **FinalizeManifest** time, not during upload:

1. Client calls `ListManifests` → gets `NextAvailableVersion = N`
2. Client calls `InitializeManifest(version = N)` → allocates version
3. Client uploads files and calls `FinalizeManifest(version = N)`
4. Service checks: **Is there a newer Finalized manifest?**
   - If NO → Finalize succeeds
   - If YES → `NewerManifestExists` error (conflict detected)
5. Client must re-list manifests, resolve the conflict, and retry with `Conflict` metadata

### How Conflict Metadata Works

When finalizing with conflict data:

```json
{
  "Conflict": {
    "IsWinner": true,
    "ConflictingVersion": 3
  }
}
```

The service sets `ManifestConflict` on **both** manifests:
- **Current manifest**: `IsWinner = <value from request>`, `ConflictingVersion = <from request>`
- **Conflicting manifest**: `IsWinner = !<value>`, `ConflictingVersion = <current version>`
- Both get `RetainedUntilTimestamp = finalizationTime + 7 days`

### IsWinner Semantics — The Inversion Fix

⚠️ **Critical for SDK integration**

The service code at `ManifestServiceLogic.cs:629–665` applies **caller-type-dependent IsWinner correction**:

| Caller Type | `IsCallerPlayFabSDK` | Service Behavior |
|-------------|---------------------|------------------|
| SDK (direct) | `true` | `IsWinner=false` → `dropAsConflictLoser=true` (finalized as PendingDeletion) |
| GRTS/Platform | `false` or `null` (non-SDK) | `IsWinner` value is **flipped** (`true↔false`) because GRTS uses inverted semantics |
| Legacy | `null` | No correction (backward compat) |

**GRTS convention**: `IsWinner` describes the **referenced conflict version** ("Is the referenced version the winner?")
**Service convention**: `IsWinner` describes **this upload** ("Is this upload the winner?")

These are opposite meanings. The service flips the value for non-SDK callers to reconcile this.

### Conflict Loser Handling

When a manifest is finalized as a conflict loser (`dropAsConflictLoser = true` for SDK, or `IsWinner = false` after GRTS inversion):
- Status is set to `PendingDeletion` (not `Finalized`)
- `CleanupStartTime` = `RetainedUntilTimestamp` (7 days after finalization)
- The manifest never appears as "latest" in `ListManifests`
- Data is retained for 7 days for rollback/recovery

### Rollback

**File**: `ManifestServiceLogic.cs` lines 1002–1124

Rollback creates a **new manifest** (new version number) that copies the file references from an old finalized manifest.

**Access control**:
- **Title entity callers**: Can rollback to any finalized manifest
- **Non-title callers** (players): Can only rollback to:
  - Last known good version (unless `DisableClientRollbackToLastKnownGood` is set)
  - Last conflict loser (unless `DisableClientRollbackToLastConflictLoser` is set)

## Key Constants & Limits

| Constant | Value | Source |
|----------|-------|--------|
| `DefaultQuotaBytes` | 256 MB (268,435,456) | `Common/Constants.cs` |
| `MaxAllowedManifestFiles` | 100 | `FrontDoor.Contracts/ContractConstants.cs` |
| `DefaultSasDurationInMinutes` | 60 | `ManifestServiceLogic.cs` |
| `DefaultMaxFinalizedManifestVersionsRetained` | 2 | `Common/Constants.cs` |
| `DefaultMinimumKnownGoodManifestsRetained` | 1 | `Common/Constants.cs` |
| `DefaultFileValidationSamplingPercentage` | 20% | `Common/Constants.cs` |
| `ManifestConflictRetryLimit` | 2 | `ManifestServiceLogic.cs` |
| `ConflictRetentionTimeSpan` | 7 days | `ManifestServiceLogic.cs` |
| `DeletedTitleDocumentTtlSeconds` | 604,800 (7 days) | `Common/Constants.cs` |
| `DeleteStorageAccountDelayDays` | 7 | `Common/Constants.cs` |
| `EntityCleanupDelaySeconds` | 15 | `Common/Constants.cs` |
| `MaxVisibilityTimeoutHours` | 167 (~7 days) | `Common/Constants.cs` |
| `CleanupDelayMinutes` | 120 (default) | `ManifestCleanupOptions` |
| `CleanupEnqueueDelayMinutes` | 60 (default) | `ManifestCleanupOptions` |
| `AllowedSizeDifferenceToleranceInBytes` | 1 MB | `ValidateUploadedFilesOptions` |
| `QuarantineTTL` | 30 days | `ValidateUploadedFilesOptions` |
| Max file name length | 50 chars | `FrontDoor.Contracts/FileToUpload.cs` |
| Max file size | 1 GB | `FrontDoor.Contracts/FinalizedFileDetails.cs` |

## GameSaveStatusCode — Error Codes

The service uses a `GameSaveStatusCode` enum (30+ values) mapped to HTTP status codes via `[GameSaveStatusCode]` attribute.

### Status Codes Most Relevant to SDK Integration

| StatusCode | HTTP | When Returned |
|------------|------|---------------|
| `Success` | 200 | Operation succeeded |
| `BadRequest` | 400 | Validation failure (invalid names, missing fields) |
| `ManifestNotFound` | 404 | Requested version doesn't exist |
| `ManifestVersionAlreadyExists` | 409 | `InitializeManifest` with existing version (non-idempotent case) |
| `ConflictUpdatingManifest` | 409 | Cosmos ETag conflict (all internal retries exhausted) |
| `NewerManifestExists` | 409 | **Conflict detection** at Finalize — a newer finalized version exists |
| `ManifestUpdatesNotAllowed` | 400 | Manifest not in correct state (e.g., Finalize on non-Uploading) |
| `ManifestVersionNotFinalized` | 400 | Trying to download a non-finalized version |
| `ManifestVersionQuarantined` | 400 | Version quarantined due to data integrity issue |
| `BaseVersionNotAvailable` | 404 | Base version not found or not finalized |
| `DataStorageQuotaExceeded` | 400 | Per-player quota exceeded at Finalize |
| `UnknownFileInManifest` | 400 | Finalize references a file never uploaded |
| `FileAlreadyExists` | 409 | Re-uploading an immutable file from an older version |
| `ServiceNotEnabledForTitle` | 403 | Title not onboarded (Rejected, PendingDeletion, or NotFound) |
| `ServiceOnboardingPending` | 403 | Title onboarding still pending |
| `OperationNotAllowed` | 400 | Player data is being transferred |
| `ManifestNotEligibleAsConflictingVersion` | 400 | Self-conflict (ConflictingVersion == Version) |
| `ManifestNotEligibleForRollback` | 400 | Target version was never finalized |
| `InternalServerError` | 500 | Storage assignment failure, data integrity issue |

### DataAccessStatusCode (Internal)

| Code | Meaning |
|------|---------|
| `Success` | Operation succeeded |
| `ManifestNotFound` | Manifest document not in Cosmos |
| `ManifestVersionAlreadyExists` | Create conflict (document already exists) |
| `ConflictUpdatingManifest` | ETag mismatch on Replace |
| `FileMapNotFound` | FileMap document not in Cosmos |
| `BlobNotFound` | Blob not in Azure Storage |
| `TransientError` | Retriable Azure error (408, 429, 503) |
| `NoStorageAccountAvailable` | No unassigned accounts in requested region |

## Error Handling Patterns

### Pattern 1: Result<T> Return Type

All data access methods return `Result<DataAccessStatusCode, T>`. Business logic checks status:

```csharp
var result = await manifestManager.ReadManifestAsync(...);
if (result.StatusCode != DataAccessStatusCode.Success)
    return result.ToErroredFinalizeManifestResponse();
```

### Pattern 2: ThrowIfNotSuccess

Guard method that converts non-success results to exceptions:

```csharp
var result = await fileMapManager.UpdateFileMapAsync(...);
result.ThrowIfNotSuccess(); // throws DataAccessException
```

### Pattern 3: ExecuteWithConflictRetriesAsync

Generic retry wrapper for ETag conflicts (up to 2 retries):

```csharp
return await ExecuteWithConflictRetriesAsync(
    () => FinalizeManifestInternalAsync(request, ...),
    cancellationToken);
```

### Pattern 4: Cosmos Transactional Batch

When multiple manifests must be updated atomically (e.g., current + conflicting + base during Finalize):

```csharp
await manifestManager.UpdateManifestsAsTransactionAsync(manifestsToUpdate, ...);
// Uses Cosmos TransactionalBatch with ETag checks on all documents
```

### Pattern 5: Idempotent Operations

Both `InitializeManifest` and `FinalizeManifest` detect duplicate requests and return Success:
- Initialize: Same version + matching metadata → Success
- Finalize: Already Finalized + matching files/conflict → Success

## Concurrency Model

### ETag-Based Optimistic Concurrency

All Cosmos DB writes use ETags (optimistic concurrency):
- `CreateManifestAsync` uses `if-not-exists` semantics
- `UpdateManifestAsync` uses `ReplaceDocumentAsync` with `IfMatchEtag`
- `UpdateManifestsAsTransactionAsync` uses `TransactionalBatch` with `IfMatchEtag` on all documents

### No Distributed Locks

The service uses **no distributed locks**. All concurrency is handled through:
1. Cosmos ETag checks on writes
2. Server-side retry (2 retries on `ConflictUpdatingManifest`)
3. Client-side retry (caller retries the full operation)

### Version Monotonicity

Version numbers only increase: `NextAvailableVersion = max(all versions) + 1`. The service never reuses or decrements version numbers. Rollback creates a NEW version (higher number) pointing to old data.

## Billing & Metering

### Billing Meters

| Meter Name | Tracked By |
|------------|-----------|
| `GameSaveDataStored` | AzureBillingProcessor (from Azure PAv2 events) |
| `GameSaveStorageReads` | AzureBillingProcessor + FrontDoor (metadata reads) |
| `GameSaveStorageWrites` | AzureBillingProcessor + FrontDoor (metadata writes) |
| `GameSaveStorageEgress` | AzureBillingProcessor (blob egress) |
| `GameSaveMetadataReads` | FrontDoor (per-operation tracking in ManifestController) |
| `GameSaveMetadataWrites` | FrontDoor (per-operation tracking in ManifestController) |

### AzureBillingProcessor Flow

```
Azure PAv2 (Push Agent v2) ──Event Hub──► AzureBillingProcessorService
    │                                           │
    │                                    Parse PAv2Event
    │                                    Map MeterId → PlayFab meter
    │                                    Check PlayFabTitleId tag
    │                                    Deduplicate (10-min cache)
    │                                           │
    │                               ┌───────────┼──────────┐
    │                               ▼           ▼          ▼
    │                          IMeterLogger   Poison Hub  NonProd Hub
    │                         (billing log)  (invalid)   (dev/int subs)
```

## Caching

| Cache | TTL | What's Cached |
|-------|-----|---------------|
| `TitleConfigCache` | 5 min (valid), 1 min (not-found) | `TitleConfigDocument` per title |
| `AccountMapCache` | 24 hours | `TitleAccountsDocument` per title (storage account assignments) |
| `AzureBlobManager` instances | Singleton (per account/container) | `BlobServiceClient` + `BlobContainerClient` |
| User Delegation Key | 6 days (per storage account) | Azure AD key for SAS token generation |

## Integration Points with SDK and GRTS

### What the SDK Must Know

1. **3-phase save pipeline**: Must call `ListManifests` → `InitializeManifest` → `InitiateUpload` → (upload files) → `FinalizeManifest` in order
2. **Version numbers from service**: Use `NextAvailableVersion` from `ListManifests`, don't compute locally
3. **Conflict detection at Finalize**: `NewerManifestExists` means another device finalized while this one was uploading. Client must re-list, resolve, retry with `Conflict` data
4. **IsWinner for SDK callers**: SDK sends its own semantic (`IsWinner=true` means "my upload is the winner"). Service uses it directly for SDK callers. Conflict loser manifests are silently dropped to `PendingDeletion`
5. **SAS token expiry**: 60-minute default. If upload takes longer, SAS expires and files can't be uploaded. Must re-call `InitiateUpload` for new SAS tokens
6. **Quota enforcement at Finalize only**: Quota is checked only at `FinalizeManifest`, not during upload. SDK should pre-check to avoid wasted uploads
7. **Idempotent operations**: Safe to retry `InitializeManifest` and `FinalizeManifest` — service detects duplicates and returns Success
8. **Rollback access control**: Non-title callers restricted to LastKnownGood or LastConflictLoser targets

### What GRTS Must Know

1. **IsWinner inversion**: GRTS sends inverted `IsWinner` semantics. The service flips the value for non-SDK callers. See "IsWinner Semantics — The Inversion Fix" above
2. **Same 3-phase pipeline**: GRTS calls the same API sequence via `NtmWebService`
3. **Version consumption**: Each sync cycle consumes ~3 version numbers (init + finalize + re-sync)

### Service → SDK Error Mapping

| Service StatusCode | Expected SDK Behavior |
|-------------------|-----------------------|
| `NewerManifestExists` | Re-list manifests, trigger conflict resolution UI/callback |
| `DataStorageQuotaExceeded` | Report quota exceeded to game |
| `ManifestUpdatesNotAllowed` | State machine error — manifest not in expected state |
| `ConflictUpdatingManifest` | Service-side retries exhausted — client should retry the full operation |
| `ServiceNotEnabledForTitle` | Title not configured — fail with clear error |
| `ManifestNotFound` | Version was cleaned up — re-list and use current versions |

## Deployment & Configuration

### Deployable Services (Kubernetes)

| Service | Project | Role |
|---------|---------|------|
| FrontDoor | `src/FrontDoor/` | REST API (the main service) |
| QueueProcessor | `src/QueueProcessor/` | Async message processing |
| ManifestChangeFeedProcessor | `src/ManifestChangeFeedProcessor/` | Cosmos DB manifest change feed |
| StorageAccountChangeFeedProcessor | `src/StorageAccountChangeFeedProcessor/` | Cosmos DB storage account change feed |
| AzureBillingProcessor | `src/AzureBillingProcessor/` | Event Hub billing event processing |

### CLI Tools

| Tool | Project | Role |
|------|---------|------|
| TitleOnboardingManager | `src/TitleOnboardingManager/` | Manual title approve/reject |
| TestEnvironmentInitializer | `src/TestEnvironmentInitializer/` | Dev Cosmos DB seeding |

### Build & Test

```powershell
# Build
dotnet build src

# Run locally (all services)
.\RunLocal.ps1 -profileName <alias> -initStorageMap $true

# Run FrontDoor only
dotnet run --project src\FrontDoor\FrontDoor.csproj --launch-profile <alias>

# Run tests against mainserver (no local service needed)
# Select runsettings from src/FunctionalTests.Common/runsettings/mainserver.*.runsettings

# Run tests against localhost (service must be running)
# Select runsettings from src/FunctionalTests.Common/runsettings/localhost.*.runsettings
```

### Authentication

- **Production**: `WorkloadIdentityCredential` (Kubernetes managed identity)
- **Local dev**: `AzureCliCredential` (requires PIM into `PlayFab GameSave Dev` role)
- Both wrapped in `CachedTokenCredential` for performance
- Anonymous auth allowed for specific dev environments: `local`, `in-memory`, `caavogad`, `alzakrze`, `ravarna`, `waralp`, `rsilva`

## Test Structure

| Project | Type | What It Tests |
|---------|------|---------------|
| `FrontDoor.UnitTests` | Unit | Service logic, status code mapping, validation |
| `FrontDoor.FunctionalTests` | Functional | API endpoints against real/mocked dependencies |
| `DataAccess.UnitTests` | Unit | Cosmos DB manager methods |
| `DataAccess.FunctionalTests` | Functional | Cosmos DB operations against real DB |
| `AzureBlobStorage.UnitTests` | Unit | Blob manager, SAS generation |
| `QueueProcessor.UnitTests` | Unit | Queue message processing logic |
| `ChangeFeedProcessor.UnitTests` | Unit | Change feed processing logic |
| `AzureBillingProcessor.UnitTests` | Unit | Billing event parsing, meter mapping |
| `E2ETests` | End-to-end | Full API flows against deployed service |

---

*Generated by Lambert (Reference Analyst) — $(Get-Date -Format 'yyyy-MM-dd')*
*Source: `C:\git\GS.GameSave` (read-only analysis)*
*For corrections or updates, verify against the source code at the referenced file paths.*

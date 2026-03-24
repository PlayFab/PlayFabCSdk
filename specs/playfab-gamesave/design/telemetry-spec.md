# PlayFab Game Save Telemetry Events Specification

**Namespace:** `playfab.gamesave.internal`

All events include common fields and are sent to PlayFab's telemetry pipeline.

## Common Fields

All events automatically include these fields:

| Field | Type | Description |
|-------|------|-------------|
| `userId` | string | Platform User ID |
| `platformType` | string | Platform type (e.g., "WIN32") |

## Event Types

### 1. context_activation

Emitted when a game save context is successfully activated.

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `totalSizeBytes` | uint64 | Yes | Total bytes in the context |
| `compressedSizeBytes` | uint64 | Yes | Storage size of the context |
| `manifestState` | int | Yes | State of manifest (MS_Initialized=new, MS_Finalized=old) |
| `syncState` | int | Yes | Final sync state (SS_NotStarted if offline) |
| `conflictResolution` | int | Yes | Resolution method if conflict occurred |
| `canceled` | bool | Yes | Whether user canceled the sync |
| `contextVersion` | string | Yes | Version being activated |
| `conflictVersion` | string | No | Version of conflicting manifest (optional) |

### 2. context_activation_failure

Emitted when game save context activation fails.

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `canceled` | bool | Yes | Whether user canceled the activation |
| `aborted` | bool | Yes | Whether game aborted the activation |
| `retryAllowed` | bool | Yes | Whether retry is possible |
| `startedOnline` | bool | Yes | Whether system was online at start |
| `syncState` | int | Yes | Sync point where failure occurred |
| `callingLocation` | uint32 | Yes | State machine location of failure |
| `httpStatus` | uint32 | Yes | HTTP status of service interactions |
| `hresult` | uint32 | Yes | HRESULT of operation |
| `cv` | string | Yes | Service correlation vector |

### 3. context_sync

Emitted when a sync operation (upload/download) completes.

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `blockCount` | uint32 | Yes | Number of payload blocks synced |
| `fileCount` | uint32 | Yes | Number of files in payload blocks |
| `syncSizeBytes` | uint64 | Yes | Compressed size of payload |
| `originalSizeBytes` | uint64 | Yes | Original size of payload |
| `totalSizeBytes` | uint64 | Yes | Total size of all payload |
| `elapsedMs` | uint32 | Yes | Elapsed milliseconds since sync start |
| `hresult` | uint32 | Yes | HRESULT of the sync |
| `httpStatus` | uint32 | Yes | HTTP status code |
| `retryCount` | uint32 | Yes | Retry count across all blocks |
| `contextVersion` | string | Yes | Manifest version synced |
| `syncDownload` | bool | Yes | Direction of sync (true=download, false=upload) |

### 4. context_sync_error

Emitted when a sync operation encounters an error.

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `elapsedMs` | uint32 | Yes | Elapsed milliseconds since operation start |
| `cv` | string | Yes | Service correlation vector |
| `hresult` | uint32 | Yes | HRESULT of the operation |
| `retryCount` | uint32 | Yes | Retry count across all blocks |
| `contextVersion` | string | Yes | Manifest version |
| `blockName` | string | Yes | Name of the failing block |
| `errorSource` | int | Yes | Operation that caused the failure |

### 5. context_delete

Emitted when game save data is deleted.

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `elapsedMs` | uint32 | Yes | Elapsed milliseconds since operation start |
| `totalSizeBytes` | uint64 | Yes | Total size of data being discarded |
| `hresult` | uint32 | Yes | HRESULT of the operation |
| `type` | int | Yes | Type of removal (DeleteType enum) |
| `contextVersion` | string | No | Manifest version being deleted (optional) |
| `cv` | string | No | Service correlation vector (optional) |

## Notes

- Events are only emitted once per operation (controlled by internal flags)
- `context_delete` events are conditionally emitted based on operation type and data size
- All timing fields (`elapsedMs`) are calculated from operation start time
- HRESULT values follow Windows convention (0 = success)
- HTTP status codes follow standard HTTP conventions (200 = success, etc.)
- **Important**: These events are also defined on the service side. Any updates to event names, field names, field types, or the namespace will need to be reflected in the service-side definitions as well.

## Example Event

```json
{
  "EventNamespace": "playfab.gamesave.internal",
  "Name": "context_activation",
  "PayloadJSON": {
    "totalSizeBytes": 1024,
    "compressedSizeBytes": 512,
    "manifestState": 1,
    "syncState": 4,
    "conflictResolution": 0,
    "canceled": false,
    "contextVersion": "1",
    "userId": "testuser456",
    "platformType": "WIN32"
  }
}
```

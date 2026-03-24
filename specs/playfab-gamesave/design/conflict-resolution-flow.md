# PFGameSave Conflict Resolution Flow Specification

Author: AI Assistant  
Date: 2025-10-23  
Status: Draft  
Related: PFGameSaves_Rollback_AddUserFlags_Spec.md Section 6.3  
Components: FolderSyncManager, CompareStep, LockStep, UploadStep

## 1. Overview

This spec details the conflict resolution flow implementation for PFGameSave, describing how conflicts are detected, resolved, and how winner/loser metadata is recorded to enable future rollback functionality. The design builds upon the existing SyncManager architecture and reuses current conflict handling code where possible.

## 2. Goals

• Provide a deterministic conflict resolution mechanism that records both branches of a conflict
• Enable future rollback to either branch (winner or loser) via `RollbackToLastConflict`
• Minimize code changes by leveraging existing `CompareStep`, `UploadStep`, and manifest structures
• Ensure single upload per conflict resolution (preserve discarded branch for rollback)
• Maintain backward compatibility with existing conflict handling

## 3. Current Architecture Integration Points

### 3.1 Existing Components Reused

• **FolderSyncManager**: Main orchestrator - coordinates conflict detection and resolution
• **CompareStep**: Already handles conflict detection and UI interaction
• **UploadStep**: Performs manifest finalization and file uploads
• **LockStep**: Manages manifest selection and rollback logic
• **UICallbackManager**: Handles conflict resolution UI callbacks

### 3.2 Current Sync Flow Integration

The conflict resolution integrates into the existing `DoWorkFolderDownload` method within FolderSyncManager at these stages:

1. **LockStep.AcquireActiveDevice()** - Manifest enumeration and baseline selection
2. **CompareStep.CompareWithCloud()** - Conflict detection and user choice
3. **Conflict Upload Integration** - Single upload to preserve discarded branch
4. **Service Persistence** - Record winner/loser metadata for future rollback

## 4. Detailed Conflict Resolution Flow: "Keep Local" Scenario

This section focuses on the "Keep Local" conflict resolution path where the user chooses to keep their local files and discard the cloud version.

### 4.1 Initial State Before Conflict

**Manifest State**:
• **V_cloud (v5)**: Finalized manifest in cloud (baseline from previous session)
• **V_pending (v6)**: Empty pending manifest allocated during LockStep for device lock
• **Local files**: Modified since last sync, diverging from V_cloud content

**Key Variables**:
• `baselineVer = 5` (V_cloud version, captured at activation start)
• `newFinalizedVer = 6` (V_pending version, will become finalized after upload)

### 4.2 Phase 1: Conflict Detection

**Location**: `CompareStep::CompareWithCloud()`

**Process**:
• CompareStep detects both local and remote changes to same files/folders
• Triggers conflict UI: "Keep Local Files" vs "Keep Cloud Files"
• User selects **"Keep Local Files"**
• CompareStep records `m_takeUIChoice = TakeUIChoice::TakeLocal`
• Sets flags: `m_conflictOccurred = true`, `m_conflictRequiresUpload = true`

### 4.3 Phase 2: Upload Local Branch

**Location**: `FolderSyncManager::DoWorkFolderDownload()` conflict upload section

**Implementation**:
```cpp
// Always upload local files to preserve the divergent branch
if (m_compareStep.ConflictRequiresUpload() && !m_conflictUploadCompleted)
{
    if (!m_conflictUploadStarted)
    {
        m_uploadStep.Reset();
        uint64_t origBaseline = m_uploadStep.GetOriginalActivationBaselineVersion(); // = 5
        if (origBaseline != 0)
        {
            m_uploadStep.SetOriginalActivationBaselineVersion(origBaseline);
        }
        m_conflictUploadStarted = true;
    }
    
    // Upload local files to V_pending (v6)
    HRESULT hrUp = m_uploadStep.Upload(/* local files -> V_pending */);
}
```

**Key Behavior**:
• Local files uploaded to V_pending (v6) regardless of user choice
• This preserves both branches: V_cloud (v5) + V_pending (v6) with local changes
• Single upload operation - no additional manifest allocation needed

### 4.4 Phase 3: Service Persistence (Keep Local)

**Location**: Service-side during UploadStep.FinalizeManifest

**Client Calls FinalizeManifest on V_pending (v6)**:
• `IsWinner = true` 
• `ConflictingVersion = "5"` (points to loser)
• Status changed from "Uploading" to "Finalized"

**Service Automatically Updates V_cloud (v5)**:
• `IsWinner = false`
• `ConflictingVersion = "6"` (points to winner)
• Remains "Finalized" but now marked as conflict loser

**Key Service Behavior**:
• When FinalizeManifest includes conflict metadata, service automatically marks the `ConflictingVersion` manifest with opposite `IsWinner` status
• No separate UpdateManifest call needed from client
• Bidirectional conflict metadata created atomically by service

### 4.5 Post-Conflict State (Keep Local)

**Final Manifest State**:
• **V_cloud (v5)**: `IsWinner=false`, `ConflictingVersion="6"` (loser, available for rollback)
• **V_pending (v6)**: `IsWinner=true`, `ConflictingVersion="5"` (winner, becomes new baseline)
• **V_pending_new (v7)**: New empty pending manifest allocated for device lock continuity

**Active Baseline**: V_pending (v6) - user's local files now authoritative
**Rollback Available**: V_cloud (v5) - accessible via `RollbackToLastConflict`

### 4.6 SyncManager State Updates

**FolderSyncManager Updates**:
• `m_latestFinalizedManifest` updated to point to V_pending (v6) 
• `m_latestPendingManifest` updated to point to V_pending_new (v7)
• Sync continues with download operations against winner baseline

**Known Good Eligibility**:
• V_cloud (v5) remains eligible for Known Good promotion (was active baseline)
• V_pending (v6) not yet eligible (needs to be loaded and produce successor)

## 5. "Keep Remote" Scenario

This section details the alternative conflict resolution path where the user chooses to keep cloud files and discard local changes.

### 5.1 Initial State (Same as Keep Local)

**Manifest State**:
• **V_cloud (v5)**: Finalized manifest in cloud
• **V_pending (v6)**: Empty pending manifest allocated during LockStep
• **Local files**: Modified since last sync, conflicting with V_cloud

### 5.2 Conflict Detection (Same Process)

**User Choice**: User selects **"Keep Cloud Files"**
• CompareStep records `m_takeUIChoice = TakeUIChoice::TakeRemote`
• Sets same flags: `m_conflictOccurred = true`, `m_conflictRequiresUpload = true`

### 5.3 Upload Local Branch (Still Required)

**Key Insight**: Local files are STILL uploaded even when user chooses "Keep Cloud"

**Rationale**:
• Preserves the rejected local branch for future rollback capability
• User might later want to access their discarded local progression
• Single upload strategy - no conditional logic based on user choice

**Implementation**: Identical upload code as Keep Local scenario
• Local files uploaded to V_pending (v6)
• V_cloud (v5) content remains unchanged

### 5.4 Service Persistence (Keep Remote)

**Client Calls FinalizeManifest on V_pending (v6)**:
• `IsWinner = false` 
• `ConflictingVersion = "5"` (points to winner)
• Status changed from "Uploading" to "Finalized"

**Service Automatically Updates V_cloud (v5)**:
• `IsWinner = true`
• `ConflictingVersion = "6"` (points to loser)
• Remains "Finalized" and authoritative

**Key Service Behavior**:
• When FinalizeManifest includes conflict metadata, service automatically marks the `ConflictingVersion` manifest with opposite `IsWinner` status
• V_cloud (v5) automatically marked as winner since client marked V_pending (v6) as loser
• Service persistence creates bidirectional metadata for rollback to discarded local branch

### 5.5 Post-Conflict State (Keep Remote)

**Final Manifest State**:
• **V_cloud (v5)**: `IsWinner=true`, `ConflictingVersion="6"` (winner, remains baseline)
• **V_pending (v6)**: `IsWinner=false`, `ConflictingVersion="5"` (loser, contains discarded local changes)
• **V_pending_new (v7)**: New empty pending manifest for device lock

**Active Baseline**: V_cloud (v5) - cloud files remain authoritative
**Rollback Available**: V_pending (v6) - discarded local branch accessible via `RollbackToLastConflict`

### 5.6 SyncManager State Updates (Keep Remote)

**FolderSyncManager Updates**:
• `m_latestFinalizedManifest` remains pointing to V_cloud (v5)
• `m_latestPendingManifest` updated to V_pending_new (v7)
• Download operations proceed against V_cloud (v5) baseline

**Local File State**:
• CompareStep marks local files for download/replacement with V_cloud (v5) content
• User's local changes overwritten with cloud version (as requested)

## 6. Rollback Integration

### 6.1 RollbackToLastConflict Selection

**Location**: `LockStep::SelectBaselineManifest()`

**Algorithm** (works for both Keep Local and Keep Remote):
```cpp
// Find the most recent conflict loser
uint64_t bestPairVersion = 0; // max(manifestVersion, conflictingVersion)
ManifestWrap bestLoser;

for (auto& manifest : m_manifests)
{
    if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
        continue;
        
    auto conflictOpt = manifest.GetConflict();
    if (!conflictOpt.has_value() || conflictOpt->GetIsWinner())
        continue; // Need loser side
        
    uint64_t thisVer = StringToUint64(manifest.GetVersion());
    uint64_t conflictVer = StringToUint64(conflictOpt->GetConflictingVersion());
    uint64_t pairHigh = max(thisVer, conflictVer);
    
    if (pairHigh > bestPairVersion)
    {
        bestPairVersion = pairHigh;
        bestLoser = manifest;
        foundLoser = true;
    }
}
```

**Rollback Examples**:
• **After Keep Local**: RollbackToLastConflict selects V_cloud (v5) - the discarded cloud version
• **After Keep Remote**: RollbackToLastConflict selects V_pending (v6) - the discarded local version

### 6.2 Service-Persisted Conflict Metadata

**Direct Rollback Support**: 
• `TryGetLatestFinalizedManifest()` respects conflict metadata
• Excludes manifests marked as losers (`IsWinner = false`) from "latest" calculation
• RollbackToLastConflict directly queries service-persisted conflict metadata

## 7. Known Good Promotion Integration

### 7.1 Promotion Rules for Conflicts

**Baseline Tracking**:
• Original activation baseline version (V_cloud v5) tracked throughout conflict resolution
• Eligible for Known Good promotion when successor finalizes in future session

**Conflict Winner Eligibility**:
• **Keep Local**: V_pending (v6) becomes new baseline, eligible for promotion after producing successor
• **Keep Remote**: V_cloud (v5) remains baseline, retains promotion eligibility

**Conflict Loser Eligibility**:
• Losers CAN be promoted if explicitly loaded via rollback and successfully produce successors
• No special restrictions on conflict losers for Known Good promotion

### 7.2 Implementation Preservation

```cpp
// Preserve original baseline version throughout conflict resolution
uint64_t origBaseline = m_uploadStep.GetOriginalActivationBaselineVersion(); // 5
m_uploadStep.Reset();
if (origBaseline != 0)
{
    m_uploadStep.SetOriginalActivationBaselineVersion(origBaseline);
}
```

## 8. State Machine Summary

### 8.1 Keep Local State Flow

```
Initial: V_cloud(v5,finalized) + V_pending(v6,empty) + Local(modified)
    ↓
[Conflict Detected: Local+Remote changes]
    ↓
[User Choice: "Keep Local"]
    ↓
[Upload: Local → V_pending(v6)]
    ↓
[Winner=v6, Loser=v5]
    ↓
[Service: V_cloud(v5,loser) + V_pending(v6,winner) + V_pending_new(v7,empty)]
    ↓
Final Baseline: V_pending(v6), Rollback: V_cloud(v5)
```

### 8.2 Keep Remote State Flow

```
Initial: V_cloud(v5,finalized) + V_pending(v6,empty) + Local(modified)
    ↓
[Conflict Detected: Local+Remote changes]
    ↓
[User Choice: "Keep Remote"]
    ↓
[Upload: Local → V_pending(v6)] (still preserves local branch)
    ↓
[Winner=v5, Loser=v6]
    ↓
[Service: V_cloud(v5,winner) + V_pending(v6,loser) + V_pending_new(v7,empty)]
    ↓
Final Baseline: V_cloud(v5), Rollback: V_pending(v6)
```

## 9. Error Handling and Edge Cases

### 9.1 Upload Failure During Conflict

**Scenario**: Upload of local files to V_pending fails

**Behavior**:
• Conflict upload failure propagated to user via existing error handling
• No conflict metadata registered - conflict resolution incomplete
• User can retry AddUser operation to attempt conflict resolution again

### 9.2 Service Conflict Metadata Failure

**Scenario**: FinalizeManifest fails or conflict metadata not properly persisted

**Behavior**:
• Conflict resolution incomplete - only upload succeeded without conflict marking
• User can retry conflict resolution via new AddUser call
• Service will attempt to re-process conflict metadata on retry

**Note**: Since service automatically handles conflict metadata for both manifests during FinalizeManifest, there are no separate UpdateManifest failures to handle.

### 9.3 Rollback Validation

**Mutual Exclusivity**:
• `E_INVALIDARG` if both RollbackToLastKnownGood and RollbackToLastConflict specified
• Validation occurs before async work begins

**Fallback Behavior**:
• RollbackToLastConflict falls back to latest finalized if no conflict losers exist
• Graceful degradation rather than hard failure

## 10. Testing Strategy

### 10.1 Keep Local Test Cases

• **Conflict Detection**: Local changes + cloud changes trigger conflict UI
• **User Choice**: TakeLocal recorded correctly in CompareStep
• **Upload Preservation**: Local files uploaded to pending manifest
• **Winner Assignment**: Pending manifest becomes winner, baseline becomes loser
• **State Updates**: FolderSyncManager baseline updated to pending manifest
• **Rollback**: RollbackToLastConflict selects original cloud baseline

### 10.2 Keep Remote Test Cases

• **User Choice**: TakeRemote recorded correctly
• **Upload Still Occurs**: Local files uploaded despite remote choice
• **Winner Assignment**: Baseline remains winner, pending becomes loser
• **State Updates**: FolderSyncManager baseline remains unchanged
• **File Replacement**: Local files replaced with cloud content
• **Rollback**: RollbackToLastConflict selects uploaded local branch

### 10.3 Integration Test Scenarios

• **Round-trip**: Conflict → Keep Local → RollbackToLastConflict → Original cloud state
• **Round-trip**: Conflict → Keep Remote → RollbackToLastConflict → Discarded local state
• **Multiple Conflicts**: Ensure recency-based selection works correctly
• **Known Good Promotion**: Verify conflict participants eligible for promotion

---

*This specification details both conflict resolution paths with emphasis on preserving user choice while maintaining rollback capability through deterministic winner/loser assignment and single-upload preservation strategy.*
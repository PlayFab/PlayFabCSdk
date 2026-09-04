# Xbox Two-Device Test: Account Linking Flow

## The Problem

When running a two-device test on Xbox:
- **DeviceA** signs into Xbox Live with User A (e.g., `XboxUserA@test.com`)
- **DeviceB** signs into Xbox Live with User B (e.g., `XboxUserB@test.com`)

Each Xbox user creates a **separate PlayFab entity** (title_player_account). This means:
- DeviceA uploads game saves to **Entity A's cloud**
- DeviceB looks for game saves in **Entity B's cloud** (empty!)

The test fails because the devices don't share cloud data.

Additionally, on real Xbox consoles, signing in a second Xbox user can trigger **SPOP (Suspend/Resume)** on the first device, causing test instability.

---

## The Solution: Account Linking

We link both Xbox accounts to the **same PlayFab CustomID entity**. This way:
- Both devices share the same PlayFab entity
- Both devices see the same cloud game saves
- No SPOP issues because each device uses its own Xbox account

---

## Step-by-Step Flow

### Phase 1: DeviceA Initial Upload

```
┌─────────────────────────────────────────────────────────────────┐
│ Step 1: XUserAddAsync                                           │
│   - Signs into Xbox Live                                        │
│   - Gets XUserHandle for Xbox User A                            │
│   - Result: state->xuser = XboxUserA handle                     │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 2: PFAuthenticationLoginWithCustomIDAsync                  │
│   - customId: "[prefix]GameTestHarness"                         │
│   - createAccount: true                                         │
│   - Creates or retrieves the SHARED PlayFab entity              │
│   - Result: state->entityHandle = shared entity                 │
│                                                                 │
│   NOTE: This entity is NOT tied to Xbox User A yet!             │
│         It's just a CustomID-based entity.                      │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 3: PFAccountManagementClientLinkXboxAccountAsync           │
│   - forceLink: true                                             │
│   - Links Xbox User A to the shared CustomID entity             │
│   - forceLink=true handles re-runs (unlinks previous if needed) │
│                                                                 │
│   After this: Xbox User A ──linked──> Shared Entity             │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 4: PFLocalUserCreateHandleWithXboxUser                     │
│   - Creates PFLocalUserHandle from the Xbox user                │
│   - This handle is needed for game saves (GRTS requires Xbox)   │
│   - The local user is associated with the SHARED entity         │
│     (because we linked in step 3)                               │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 5: PFGameSaveFilesAddUserWithUiAsync                       │
│   - Adds user to game save system                               │
│   - Game saves will sync to the SHARED entity's cloud           │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 6: Upload game save data                                   │
│   - WriteGameSaveData (local files)                             │
│   - PFGameSaveFilesUploadWithUiAsync                            │
│   - Data goes to SHARED entity's cloud storage                  │
└─────────────────────────────────────────────────────────────────┘
```

### Phase 2: DeviceB Download and Upload

```
┌─────────────────────────────────────────────────────────────────┐
│ Step 1: XUserAddAsync                                           │
│   - Signs into Xbox Live                                        │
│   - Gets XUserHandle for Xbox User B (DIFFERENT user!)          │
│   - Result: state->xuser = XboxUserB handle                     │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 2: PFAuthenticationLoginWithCustomIDAsync                  │
│   - customId: "[prefix]GameTestHarness" (SAME as DeviceA!)      │
│   - createAccount: true                                         │
│   - Retrieves the SAME shared entity that DeviceA used          │
│   - Result: state->entityHandle = SAME shared entity            │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 3: PFAccountManagementClientLinkXboxAccountAsync           │
│   - forceLink: true                                             │
│   - Links Xbox User B to the shared CustomID entity             │
│   - forceLink=true unlinks Xbox User A first (if still linked)  │
│                                                                 │
│   After this: Xbox User B ──linked──> Shared Entity             │
│               (Xbox User A is now unlinked)                     │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 4: PFLocalUserCreateHandleWithXboxUser                     │
│   - Creates PFLocalUserHandle from Xbox User B                  │
│   - Associated with the SHARED entity                           │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 5: PFGameSaveFilesAddUserWithUiAsync                       │
│   - Adds user to game save system                               │
│   - Game saves sync from the SHARED entity's cloud              │
│   - DeviceB sees DeviceA's uploaded data! ✓                     │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Step 6: Modify and upload                                       │
│   - WriteGameSaveData (modify local files)                      │
│   - PFGameSaveFilesUploadWithUiAsync                            │
│   - Updated data goes to SHARED entity's cloud                  │
└─────────────────────────────────────────────────────────────────┘
```

### Phase 3: DeviceA Final Download

DeviceA repeats the same linking flow and sees DeviceB's changes.

---

## Key Points

### Why CustomID Login First?
- `PFAuthenticationLoginWithCustomIDAsync` creates/retrieves the shared entity
- Both devices use the **same customId** → same entity
- The controller's prefix ensures test isolation (e.g., `"jasonsa-GameTestHarness"`)

### Why Link Xbox Account?
- Xbox game saves (GRTS) require an Xbox user
- `PFLocalUserCreateHandleWithXboxUser` needs the Xbox account
- Linking associates the Xbox user with the shared entity
- Without linking, each Xbox user would have their own entity

### Why forceLink: true?
- If Xbox User A was previously linked to the shared entity, linking Xbox User B would fail
- `forceLink: true` tells PlayFab: "Unlink whoever is currently linked, then link me"
- This makes the test idempotent (safe to re-run)

### Entity Ownership Timeline
```
Initial state:
  Shared Entity (CustomID) ── no Xbox linked

After DeviceA links:
  Shared Entity ──linked──> Xbox User A

After DeviceB links (with forceLink):
  Shared Entity ──linked──> Xbox User B
  (Xbox User A automatically unlinked)

After DeviceA re-links:
  Shared Entity ──linked──> Xbox User A
  (Xbox User B automatically unlinked)
```

---

## YAML Command Sequence

```yaml
# 1. Sign into Xbox Live (required for game saves)
- command: XUserAddAsync

# 2. Login with shared CustomID (establishes shared PlayFab entity)
- command: PFAuthenticationLoginWithCustomIDAsync
  parameters:
    customId: "GameTestHarness"    # Controller adds prefix
    createAccount: true

# 3. Link Xbox to the shared entity (forceLink for idempotency)
- command: PFAccountManagementClientLinkXboxAccountAsync
  parameters:
    forceLink: true

# 4. Create local user from Xbox (required for game saves)
- command: PFLocalUserCreateHandleWithXboxUser

# 5. Add user to game save system (syncs with shared cloud)
- command: PFGameSaveFilesAddUserWithUiAsync
```

---

## Comparison: Old vs New Approach

| Aspect | Old (Xbox-only) | New (Linked) |
|--------|-----------------|--------------|
| Xbox Sign-in | Required | Required |
| PlayFab Entity | Per-Xbox-user | Shared via CustomID |
| Cloud Data | Separate per user | Shared |
| SPOP Risk | High (same user on two devices) | Low (different users) |
| Test Isolation | By Xbox account | By CustomID prefix |

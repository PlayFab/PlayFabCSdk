# PFGameSave GRTS Config Extensions Mini Spec

Author: (TBD)  
Last Updated: 2025-09-14  
Status: Draft / Review

## 1. Purpose

Extend the out-of-proc (GRTS) Game Save configuration to optionally:

1. Provide PlayFab Entity authentication (Entity Id + Entity Token) directly to the platform layer ONLY in the fallback scenario where an XUser handle is NOT available (i.e. `PFLocalUserTryGetXUser` fails). When an XUser is present we continue using the existing XUser-based auth path and DO NOT set the new entity auth flag.
2. Provide an explicit file system root location to the platform layer even on platforms where the path was previously implicit/ignored.

These additions are required for upcoming scenarios where the GRTS (xgameruntime / service component) needs to:

- Make PlayFab REST calls on behalf of the title after the title has suspended / terminated (continuation uploads, device contention decisions, quota checks) without requiring re-auth via XUser only flows.
- Access a non-standard or caller-specified save root path (e.g. forced test path, SteamDeck-like override reused in certain Windows test or dev tooling scenarios) instead of an implicit platform-controlled location.

## 2. Scope (In / Out)

In Scope:
- Add two new initialization flags to `PFXGameSaveConfigRequest`.
- Add three new optional fields to `PFXGameSaveConfigRequest`.
- Define population rules in `GameSaveAPIProviderGRTS` (PF layer) before calling `PFXGameSaveInitializeConfig`.
- Validation & fallback behavior when data not available.
- Telemetry & logging requirements (minimal).

Out of Scope:
- Server/service changes (service already accepts entity tokens).
- Changes to in-proc (non GRTS) provider.
- UI changes (no user-facing strings in this spec).
- Persistence of tokens beyond in-memory lifetime already managed by PFCore.

## 3. New Flags & Fields

Added to `PFXGameSaveConfigRequest` (platform / PAL side):

Flags (bitfield):
- `InitFlagUseEntityAuth = 0x4`  
  Indicates that `entityId` & `entityToken` are supplied and should be used for PlayFab calls instead of (or in addition to) XUser -> Xbox token translation.
- `InitFlagUseFileLocation = 0x8`  
  Indicates that `fileLocation` is a caller-specified absolute path to the game save root to be honored by GRTS.

Fields (UTF-8, null-terminated):
- `const char* entityId;` (PlayFab Entity Key Id, usually the Title Player Entity Id)  
- `const char* entityToken;` (PlayFab Entity Token with sufficient lifetime; short-lived)  
- `const char* fileLocation;` (Absolute path to root of game save folder)

All three are optional unless their corresponding flag is set. If a flag is set, the field MUST be non-null & non-empty or initialization fails.

## 4. Population Logic (GameSaveAPIProviderGRTS)

Executed inside `GameSaveAPIProviderGRTS::AddUserWithUiAsync` before calling `PFXGameSaveInitializeConfig`.

1. Acquire existing values (current behavior):
   - `requestingUser` (XUserHandle) via `PFLocalUserTryGetXUser`.
   - `titleId` & `apiUrl` via `PFServiceConfigGetTitleId` / `PFServiceConfigGetAPIEndpoint`.
2. Determine Entity Auth fallback (ONLY if no XUser):
   - Attempt to obtain an `XUserHandle` via `PFLocalUserTryGetXUser(localUserHandle, &xuser)` (current behavior already present in code).
   - If this call SUCCEEDS (XUser present):
     - DO NOT populate `entityId` / `entityToken`; leave them `nullptr`.
     - DO NOT set `InitFlagUseEntityAuth` (even if an entity token also exists). Auth continues through XUser->Xbox token flows as today.
   - If this call FAILS (no XUser identity available for this PFLocalUser):
     - Attempt to retrieve an entity handle with `PFLocalUserTryGetEntityHandle(localUserHandle, &entityHandle)`.
       * If this fails (e.g. user not logged into PlayFab), continue without entity auth (legacy behavior). No flag set.
     - If entity handle acquired:
       * Retrieve entity key: `PFEntityGetEntityKeySize` -> allocate buffer -> `PFEntityGetEntityKey` to access `PFEntityKey` (use `.id` as `entityId`).
       * Retrieve (cached) entity token: initiate `PFEntityGetEntityTokenAsync(entityHandle)` then `PFEntityGetEntityTokenResultSize` + `PFEntityGetEntityTokenResult` to get `PFEntityToken.token` and optional expiration.
       * If token retrieval succeeds AND token string non-empty:
         - Set `configRequest.flags |= InitFlagUseEntityAuth`.
         - Set `configRequest.entityId = <entityKey.id>` (expected to be Title Player entity id when logged in as a player).
         - Set `configRequest.entityToken = <entityToken.token>`.
       * If token retrieval fails or token empty: do not set flag (fallback to existing code paths).
     - Close duplicated `entityHandle` when done (per existing ownership rules) after constructing the config.
3. Determine File Location usage:
   - If during `PFGameSaveFilesInitialize` a non-empty `PFGameSaveInitArgs.saveFolder` was supplied AND platform = Windows/Xbox GRTS provider AND the caller explicitly requested use (TBD trigger; see Policy below), then:
     - Validate path is absolute (win32: drive letter or UNC). If invalid -> fail fast with `E_INVALIDARG` (spec open question: alternate error code?).
     - Set `configRequest.flags |= InitFlagUseFileLocation`.
     - Set `configRequest.fileLocation = saveFolder`.
   - Otherwise leave unset.

## 5. Test Plan

Unit / Component Tests (mock PFX layer):
1. XUser present, entity token also present -> NO entity flag.
2. XUser present, custom folder -> ONLY `InitFlagUseFileLocation`.
3. No XUser, entity token success -> `InitFlagUseEntityAuth` set (and values copied).
4. No XUser, entity token retrieval fails -> no flag.
5. No XUser, entity token success + custom folder -> both flags set.
6. Force invalid path with file location flag conditions -> `E_INVALIDARG`.
7. Simulate token empty string -> no flag.
8. Simulate token expire just before config build (treat as retrieval failure) -> no flag.

Functional (end-to-end) on dev kit / PC:
1. Upload after closing title continues using entity auth (observe service calls succeed without relaunch) when login method supplies entity token not derivable from XUser alone (e.g. PlayFab CustomID).
2. Override save folder; create file; verify GRTS picks up and uploads correct content.

Negative:
1. Tamper path to point to inaccessible directory; ensure failure early.

## 6. Glossary

| Term | Definition |
|------|------------|
| GRTS | Xbox Game Runtime Service (out-of-proc GameSave execution) |
| PF | PlayFab C SDK layer |
| Entity Token | PlayFab authentication token tied to entity (Title Player) |

---

End of mini spec.

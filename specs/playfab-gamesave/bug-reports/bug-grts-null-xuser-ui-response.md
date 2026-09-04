# Bug: GRTS UI Response APIs Crash on Null XUser

**Status:** Open  
**Severity:** P1 — Blocks Steam Deck / custom identity scenarios on GRTS path  
**Component:** `GameSaveAPIProviderGRTS` (PFGameSave SDK) + `PFXGameSave` (GRTS / xgameruntime)  
**Date:** 2026-04-01

---

## Summary

When `PFXGameSaveConfigRequest.requestingUser` is null (entity-auth-only path, e.g. Steam Deck Approach 2 or `LoginWithCustomID` without XUser), the GRTS UI response APIs crash or return unexpected errors because they receive a null `XUserHandle`. The PFGameSave SDK's GRTS provider calls `PFXPALGetXUserFromLocalUser()` which correctly returns `nullptr` when no XUser is associated with the `PFLocalUserHandle`, but then passes that `nullptr` directly to the `PFXGameSave*` response functions without any null check.

---

## Affected APIs

Every GRTS UI response and progress API is affected. They all follow the same pattern:

| PFGameSave SDK Function | GRTS Function Called | Header Signature |
|---|---|---|
| `SetUiProgressResponse` | `PFXGameSaveSetProgressUiResponse` | `_In_ XUserHandle requestingUser` |
| `SetUiSyncFailedResponse` | `PFXGameSaveSetSyncFailedUiResponse` | `_In_ XUserHandle requestingUser` |
| `SetUiActiveDeviceContentionResponse` | `PFXGameSaveSetActiveDeviceContentionUiResponse` | `_In_ XUserHandle requestingUser` |
| `SetUiConflictResponse` | `PFXGameSaveSetConflictUiResponse` | `_In_ XUserHandle requestingUser` |
| `SetUiOutOfStorageResponse` | `PFXGameSaveSetOutOfStorageUiResponse` | `_In_ XUserHandle requestingUser` |
| `UiProgressGetProgress` | `PFXGameSaveProgressUiGetProgress` | `_In_ XUserHandle requestingUser` |

Additionally, all GRTS UI **callback typedefs** deliver `XUserHandle requestingUser` as their first parameter (see `PFXGameSave.h` lines 108–115). When `configRequest.requestingUser` was null at initialization time, these callbacks will fire with a null `XUserHandle`, and any title code that dereferences it will crash.

---

## Root Cause

### PFGameSave SDK layer (`PFGameSaveFilesAPIProvider_GRTS.cpp`)

All response methods follow this pattern (example: `SetUiSyncFailedResponse`, line 1325):

```cpp
HRESULT GameSaveAPIProviderGRTS::SetUiSyncFailedResponse(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesUiSyncFailedUserAction action) noexcept
{
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    // ^^^ returns nullptr when no XUser is associated (entity-auth path)
    HRESULT hr = PFXGameSaveSetSyncFailedUiResponse(requestingUser, ...);
    // ^^^ passes nullptr to GRTS — crash or undefined behavior
    return hr;
}
```

`PFXPALGetXUserFromLocalUser()` (line 71) iterates the user state array and returns `nullptr` when no match is found or when the matched entry's `xUser` field is null (which it is in the entity-auth-only path where `PFLocalUserTryGetXUser` fails).

### GRTS layer (`PFXGameSave.h`)

All PFX response functions are declared with `_In_ XUserHandle requestingUser`:

```c
STDAPI PFXGameSaveSetSyncFailedUiResponse(
    _In_ XUserHandle requestingUser,
    _In_ PFXGameSaveSyncFailedUiResponse response) noexcept;
```

The `_In_` SAL annotation means the parameter must be non-null. The GRTS implementation presumably uses the `XUserHandle` to look up internal per-user state. When null is passed, the behavior is undefined — likely an access violation or `E_INVALIDARG` depending on the internal implementation.

---

## Reproduction Steps

1. Create a `PFLocalUserHandle` using a non-XUser method (e.g. `PFLocalUserCreateHandleWithPersistedLocalId` with `LoginWithCustomID`, or Steam Deck custom identity).
2. Call `PFGameSaveFilesInitialize` → set UI callbacks → `PFGameSaveFilesAddUserWithUiAsync`.
3. Force a network failure so the SyncFailed callback fires.
4. Inside the callback (or after), call `PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, UseOffline)`.
5. **Expected:** The response is forwarded to GRTS and the state machine advances.
6. **Actual:** The GRTS provider passes `nullptr` as `requestingUser` to `PFXGameSaveSetSyncFailedUiResponse`. This either crashes or returns an error, leaving the state machine stuck.

The same issue applies to all other UI response APIs if their corresponding callbacks fire.

---

## Impact

- **Steam Deck Approach 2** (custom identity without Xbox auth): Any scenario that triggers a UI callback will fail because there is no `XUserHandle`.
- **`LoginWithCustomID` on GRTS path**: Same failure — `PFLocalUserTryGetXUser` returns no XUser, so `PFXPALGetXUserFromLocalUser` returns null.
- The `InitFlagUseEntityAuth` path in `grts-config-extensions.md` was designed precisely for this scenario, but the response APIs weren't updated to handle the null-XUser case.

---

## Suggested Fix

Two layers need changes:

### 1. PFGameSave SDK (`GameSaveAPIProviderGRTS`)

Add a null check after `PFXPALGetXUserFromLocalUser` in every response method. If null, either:

- **(a)** Look up the user by an alternate key (e.g. the `PFXPALGameSaveUserState` entry's config handle) and use a GRTS API that accepts a config handle instead of XUser, **or**
- **(b)** Return a clear error like `E_PF_GAMESAVE_USER_NOT_ADDED` with a trace log explaining that UI responses are not supported on the GRTS path without an XUser.

Option (a) is preferred since it enables the feature to actually work.

### 2. GRTS layer (`PFXGameSave`)

Add overloads or modify the existing response APIs to accept `PFXGameSaveConfigHandle` as an alternative to `XUserHandle` for user lookup. This would allow the entity-auth path to identify the correct internal session without an `XUserHandle`. For example:

```c
// New overload
STDAPI PFXGameSaveSetSyncFailedUiResponseWithConfig(
    _In_ PFXGameSaveConfigHandle configHandle,
    _In_ PFXGameSaveSyncFailedUiResponse response) noexcept;
```

Similarly, the callback typedefs should either deliver a `PFXGameSaveConfigHandle` alongside the `XUserHandle`, or the GRTS should tolerate null `XUserHandle` in callbacks when entity-auth was used.

---

## Workaround

There is currently no workaround for titles using GRTS with entity-auth-only identity. The only option is to ensure an `XUserHandle` is available (Xbox ecosystem integration / Approach 1), which defeats the purpose of the custom identity path.

---

## Related Files

| File | Relevance |
|------|-----------|
| `PFXGameSave.h` (GDK 260400) | GRTS API declarations — all response/callback APIs take `XUserHandle` |
| `PFGameSaveFilesAPIProvider_GRTS.cpp` | SDK-side provider that bridges PFGameSave → GRTS; null passed without check |
| `PFGameSaveFilesAPIProvider_GRTS.h` | Provider class declaration |
| `specs/playfab-gamesave/design/grts-config-extensions.md` | Entity-auth config spec that enables the null-XUser path |
| `specs/playfab-gamesave/docs/game-saves/steam-deck-implementation.md` | Approach 2 (custom identity) documentation |

## Related Specs

- **`grts-config-extensions.md` §4**: Describes the `InitFlagUseEntityAuth` population logic where `PFLocalUserTryGetXUser` fails and entity auth is used instead. This is the exact scenario that triggers the bug.
- **`steam-deck-implementation.md` Approach 2**: Custom identity path that doesn't provide an XUser.

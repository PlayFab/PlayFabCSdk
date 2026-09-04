# Bug: XUserAddAsync Silent Login Timeout (0x89245106)

## Classification
- **Type:** Test flakiness / transient auth error
- **Severity:** Low (intermittent, not a product bug)
- **Affected tests:** 22 (All-or-Nothing Keep Cloud), potentially any test using XUserAddAsync on pc-grts device
- **First observed:** pass111 (2026-05-05)

## Symptoms

`XUserAddAsync` with `AddDefaultUserSilently` fails after ~10 seconds with error `0x89245106` on the pc-grts (GDK) device. The test never reaches PlayFab game save operations.

```
XUserAddAsync: Trying AddDefaultUserSilently...
XUserAddAsync: Silent login failed (hr=0x00000000, waitHr=0x89245106, resultHr=0x00000000).
  Pass allowUi:true to retry with sign-in UI.
[Command] << XUserAddAsync >> status=failed, hresult=0x89245106, elapsed=10385ms
```

## Root Cause

The GDK `XUserAddAsync` API attempts to sign in the default Xbox user silently (no UI). The ~10 second elapsed time and failure suggest the Xbox Live authentication service either:
1. Timed out during token refresh/validation
2. Returned a transient error for the cached user credential
3. Had a brief connectivity issue to Xbox Live endpoints

Error code `0x89245106` is in the GDK `E_GAMEUSER_*` range (facility 0x924). The exact meaning is undocumented publicly but likely indicates "user resolution required" or "silent sign-in unavailable."

## Context

- The pc-grts device uses real GDK XUser APIs (not mocked)
- A valid signed-in user with package identity is required
- Previous tests (01–21) on the same device succeeded with XUserAddAsync
- The failure is isolated to this one occurrence — test 22 typically passes

## Mitigation Options

### Option 1: Add retry logic in test handler
The `XUserAddAsync` handler could retry the silent login once after a short delay:
```cpp
if (!success) {
    Sleep(2000);
    success = tryAddUser(XUserAddOptions::AddDefaultUserSilently, false, hr, waitHr, resultHr);
}
```

### Option 2: Fall back to UI-based login
Pass `allowUi: true` in the test YAML so the handler falls through to `AddDefaultUserAllowingUI`:
```yaml
- command: XUserAddAsync
  parameters:
    allowUi: true
```

### Option 3: Accept as known flakiness
Xbox Live auth services occasionally have transient issues. Re-run to confirm.

## Impact

- Not a product bug — this is the GDK platform's XUser API timing out
- Not a game save issue — failure occurs before any game save operations
- The test correctly fails fast when auth is unavailable (games would show sign-in UI)

## Log References
- `Out/gamesave-pc-tests/pass111/22/device-DeviceA-fetched-log.txt`
- `Out/gamesave-pc-tests/pass111/22/controller.log`

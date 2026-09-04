# PlayFabGameSave Code Review — 2026-03-25

**Reviewer:** GPT-5.4
**Scope:** `Source\PlayFabGameSave\` (excluding Generated/)

## Summary

| Severity | Count |
|----------|-------|
| Critical | 1 |
| Important | 1 |
| Minor | 1 |

## Critical Issues

### CR-1: Device IDs are generated from a second-resolution global LCG, so different devices can get the same identity

**File:** `Source\PlayFabGameSave\Source\Common\Utils.cpp:19-27,48-63,305`; `Source\PlayFabGameSave\Source\SyncManager\LockStep.cpp:347-355,403-405`

When `cloudsync\info.json` does not exist, `GetLocalDeviceID()` creates a new device ID with `CreateGUID()`. That helper is not a GUID generator at all: it seeds a process-global LCG from `time(nullptr)` and emits a deterministic sequence. Two fresh devices that create `info.json` in the same second will persist the same `deviceId`, and the lock step later treats pending manifests with that ID as belonging to the local device. That suppresses active-device contention and allows one device to reuse or overwrite another device's pending save state.

```cpp
static uint64_t internal_seed = 0;
uint32_t internal_rand(void)
{
    if (internal_seed == 0)
    {
        internal_seed = static_cast<uint64_t>(std::time(nullptr));
    }
    internal_seed = internal_seed * LCG_MULTIPLIER + LCG_INCREMENT;
    return (uint32_t)(internal_seed / LCG_MODULUS_DIVISOR) % LCG_MODULUS_RANGE;
}

String CreateGUID()
{
    std::ostringstream guidStream;
    for (int i = 0; i < 32; ++i)
    {
        if (i == 8 || i == 12 || i == 16 || i == 20)
        {
            guidStream << '-';
        }
        guidStream << RandomHexDigit();
    }
    return String(guidStream.str().c_str());
}
...
data.deviceId = CreateGUID();
...
if (latestPendingManifest->GetMetadata()->GetDeviceId() != localDeviceId)
{
    showActiveDeviceContentionUI = true;
}
...
bool canReusePending = latestPendingManifest->GetMetadata().has_value() &&
    latestPendingManifest->GetMetadata()->GetDeviceId() == GetLocalDeviceID(saveFolder) &&
    pendingVersion > finalizedVersion;
```

**Fix:** Generate device IDs with a real UUID / cryptographically strong random source (for example `CoCreateGuid` or `BCryptGenRandom`) and synchronize one-time device-ID creation so concurrent syncs cannot race on shared generator state.

---

## Important Issues

### II-1: GRTS `IsConnectedToCloud` always returns `true`, even for users that were never added

**File:** `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_GRTS.cpp:890-903`

This implementation ignores `localUserHandle`, never checks whether the caller has successfully added the user, and hard-codes the result to `true`. A title can trigger this by calling `PFGameSaveFilesIsConnectedToCloud` on GRTS before `PFGameSaveFilesAddUserWithUiAsync` succeeds (or with an invalid handle): the API reports cloud connectivity even though no per-user game-save session exists. Callers that use this status to enable uploads or suppress reconnect UX will make the wrong decision.

```cpp
HRESULT GameSaveAPIProviderGRTS::IsConnectedToCloud(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ bool* isConnectedToCloud
) noexcept
{
    UNREFERENCED_PARAMETER(localUserHandle);
    // TODO: Query actual GRTS/PFX connectivity state for this user.
    // Currently hard-coded to true because the GRTS platform manages connectivity
    // at the OS level and does not expose a per-user connectivity query API.
    *isConnectedToCloud = true;
    TRACE_WARNING("GameSaveAPIProviderGRTS::IsConnectedToCloud: returning hard-coded true (no GRTS connectivity API available)");
    return S_OK;
}
```

**Fix:** Validate the user first and return `E_PF_GAMESAVE_USER_NOT_ADDED` (or `false`) when no GRTS state exists for that handle. If the platform cannot expose live connectivity, cache a real per-user connectivity state from add-user / upload failures instead of returning `true` unconditionally.

---

## Minor Issues

### MI-1: Mock cloud file operations ignore filesystem write failures and still report success

**File:** `Source\PlayFabGameSave\Source\Wrappers\GameSaveServiceMock.cpp:650,683,713,771`

When mock mode is enabled through `PFGameSaveFilesSetMockDataFolderForDebug`, several mock upload/download paths call `WriteEntireFile()` and discard its HRESULT. If the mock data folder is read-only, full, or otherwise unwritable, these APIs still return `S_OK`, so tests/debug sessions believe the cloud write succeeded even though no data was persisted. That masks real failures and can leave mock manifests and payload files out of sync.

```cpp
WriteEntireFile(manifestsPath, vData);
...
RETURN_IF_FAILED(ReadEntireFile(downloadUrl, fileData));
WriteEntireFile(filePath, fileData);
...
Vector<char> fileData(fileContent.begin(), fileContent.end());
WriteEntireFile(fullPath, fileData);
...
RETURN_IF_FAILED(ReadEntireFile(filePath, fileData));
...
WriteEntireFile(fullPath, fileData);
```

**Fix:** Check and propagate every `WriteEntireFile()` result (and fail the async op when it does not succeed) so mock mode preserves the same error semantics as the real service path.

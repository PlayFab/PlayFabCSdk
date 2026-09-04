# Xbox Automated Test Plan — Non-API Automation Actions

All non-API actions referenced across the Xbox test scenarios. These are things the test framework must do **outside** of calling PFGameSaveFiles SDK APIs — process lifecycle, network control, hardware, file I/O, etc.

Each action includes the GDK tool solution, usage examples, and the `ChangeTargetDeviceState` YAML command for test scenarios.

> **Multi-console note**: All `xb*` tools default to the console set by `xbconnect`. To target a specific console (or avoid changing your default), pass `/x:$ip` on every call. All examples below include `/x $ip` or `/x:$ip` — this is required when orchestrating tests across multiple dev kits.

> **Controller integration**: All actions are available as `ChangeTargetDeviceState` commands. The controller resolves xb* tools from `%GameDK%\bin\` (version-independent). For Xbox targets (engine: `xbox`), the controller shells out to the appropriate xb* tool with `/x:$ip` derived from the device's WebSocket endpoint. For PC targets (engine: `pc-*`), only `DisableNetwork`/`EnableNetwork` are supported; other actions log "not supported on target device". The test app PFN (`41336MicrosoftATG.XboxLiveE2E_dspnxghe87tn0`) and AUMID (`...!Game`) are hardcoded in `DeviceStateController.cs` — no need to pass `packageName` in YAML. See `DeviceStateController.cs`.

---

## Summary

| # | Action | Tag | Tool | Status |
|---|--------|-----|------|--------|
| 1 | Disable network | [DISCONNECT] | ``xbstress simulate network=broken`` | ✅ |
| 2 | Enable network | [RECONNECT] | ``xbstress stop`` | ✅ |
| 3 | Network flapping | — | ``xbstress`` toggled in a loop | ✅ |
| 4 | PLM suspend | [SUSPEND] | ``xbapp suspend`` | ✅ |
| 5 | PLM resume | [RESUME] | ``xbapp resume`` | ✅ |
| 6 | Force terminate | [TERMINATE] | ``xbapp terminate`` or ``xbrun kill.exe`` | ✅ |
| 7 | Evict via title launch | [EVICT-GAME] | ``xbapp launch $otherTitle`` | ✅ |
| 8 | Relaunch after terminate | — | ``xbapp launch`` | ✅ |
| 9 | Structured shutdown | [SHUTDOWN] | ``xbreboot /S`` | ✅ |
| 10 | Sudden power loss | [POWER-PULL] | Network-addressable PDU — cycle power via API | ⚠️ |
| 11 | Connected standby | [STANDBY] | ``xbreboot /P`` / ``xbreboot /W`` | ✅ |
| 12 | Boot after power event | — | ``xbconnect $ip /b /q /ws:120`` | ✅ |
| 13 | Sign out Xbox user | [SIGN-OUT] | ``xbuser signout`` | ✅ |
| 14 | Sign in Xbox user | [SIGN-IN] | ``xbuser signin`` | ✅ |
| 15 | Switch user | [SWITCH-USER] | ``xbuser signout`` + ``xbuser signin`` | ✅ |
| 16 | SPOP trigger | — | ETW listener via tvpp/tlc (green-signed TBD) | ⚠️ |
| 17 | Delete Everywhere | [DELETE-EVERYWHERE] | ``xbstorage delete`` / ``pfgamesaveutil reset`` | ✅ |
| 18 | Move to background | [MOVE-TO-BACKGROUND] | ``xbapp suspend`` or ``xbapp launch $stub`` | ✅ |
| 19 | Return to foreground | [RETURN-TO-FOREGROUND] | ``xbapp resume`` or ``xbapp terminate $stub`` | ✅ |
| 20 | Quick Resume save | [QR-SAVE] | ``xbapp save`` | ✅ |
| 21 | Quick Resume restore | [QR-RESTORE] | ``xbapp restore`` | ✅ |
| 22 | Simulate out-of-storage | [FILL-STORAGE] | ``xbstorage simulate`` | ✅ |
| 23 | Stop storage simulation | — | ``xbstorage simulate /stop`` | ✅ |

---

## A. Network Control

**Scenarios**: 10, 13, 16, 19, 22, 26, 28, 32, 37, 41, 45, 49, 55, 57, 68, 73, 74, 79, 80, 87, 88, 89

**Note**: In-proc tests use ``ConfigureHttpMock`` to simulate offline. On Xbox/GRTS, HTTP mocks don't intercept GRTS traffic (out-of-proc), so real network control is needed. The GDK ``xbstress`` tool solves this.

### 1. Disable network [DISCONNECT] / 2. Enable network [RECONNECT]

**Option A — "broken" mode** (title traffic fails, tools still work):
```powershell
xbstress /x $ip simulate network=broken    # Disable — title sees no network
xbstress /x $ip stop                        # Re-enable — all traffic resumes
```
From the title's perspective this is the same as an ISP failure. Tools traffic (xbrun, xbapp, etc.) is **exempted**, so the test controller can still communicate with the console.

**Option B — "disconnect" mode** (full cable-pull simulation):
```powershell
xbstress /x $ip simulate network=disconnect  # Simulates physical cable pull
xbstress /x $ip stop                          # Restore connectivity
```
This closely matches a physical network cable pull, including link status change events. **Most tools will NOT work** while this is active since all SystemOS traffic is blocked too.

**Option C — bandwidth/loss control** (fine-grained):
```powershell
xbstress /x $ip start inbwlimitkbps=192 outbwlimitkbps=192 packetloss=100  # Effectively offline
xbstress /x $ip stop
```

**Recommendation**: Use ``network=broken`` for most test scenarios (controller stays connected). Use ``network=disconnect`` when testing true link-down behavior.

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: DisableNetwork

- command: ChangeTargetDeviceState
  parameters:
    action: EnableNetwork
```

### 3. Network flapping

``xbstress`` can be toggled rapidly from the test controller:

```powershell
for ($i = 0; $i -lt 10; $i++) {
    xbstress /x $ip simulate network=broken
    Start-Sleep -Milliseconds 500
    xbstress /x $ip stop
    Start-Sleep -Milliseconds 500
}
```

Channel-based simulation can target specific addresses while leaving others unaffected:
```powershell
xbstress /x $ip set channel=0 network=broken addresses=<playfab-endpoint-ip>
xbstress /x $ip simulate network=channels
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: NetworkFlapping
    cycles: 10
    intervalMs: 500
```

---

## B. Process Lifecycle

**Scenarios**: 13–18, 37–40, 49–57

### 4. PLM Suspend / 5. PLM Resume

```powershell
xbapp /x $ip suspend $packageName   # Trigger PLM suspend
xbapp /x $ip resume  $packageName   # Resume from suspend
xbapp /x $ip query   $packageName   # Check state: Running|Suspending|Suspended|Terminated|Constrained
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: Suspend

- command: ChangeTargetDeviceState
  parameters:
    action: Resume
```

### 6. Force Terminate

```powershell
xbapp /x $ip terminate $packageName   # Clean PLM termination
```

For a hard kill (bypassing PLM), use ``xbrun`` to invoke ``kill.exe`` by PID:
```powershell
xbrun /x:$ip /x:/system /O kill.exe -f $pid
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: Terminate
```

### 7. Evict via title launch

Launch another title via ``xbapp`` — the OS evicts the current game:
```powershell
xbapp /x $ip launch $stubPackageName   # Current game gets evicted
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: EvictGame
    stubPackageName: $stubPackageName
```

### 8. Relaunch after terminate/evict

```powershell
xbapp /x $ip deploy C:\packages\GameTestAppXbox\   # First time only
xbapp /x $ip launch $packageName                    # (Re)launch
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: Relaunch
```

---

## C. Power State Control

**Scenarios**: 41–52

### 9. Structured shutdown

```powershell
xbreboot /x $ip        # Reboot
xbreboot /x $ip /S     # Full power-off (shutdown, no reboot)
xbreboot /x $ip /S /T:1   # Power off, turn back on in 1 hour (max 72 hours)
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: Reboot

- command: ChangeTargetDeviceState
  parameters:
    action: Shutdown
```

### 10. Sudden power loss [POWER-PULL]

⚠️ **Requires lab hardware.** In the past, dev kits in the lab have been connected to network-addressable power strips (e.g., APC switched rack PDU). Power can be cycled via SNMP or HTTP API, giving true cold power-pull behavior. The OS scenario tests simulate power loss by killing the Connected Storage service process (``kill.exe`` by PID), which tests the same XVD recovery path — use this as a fallback when PDU hardware is unavailable.

```powershell
# True power pull (requires network PDU with API)
Invoke-RestMethod -Method Post -Uri "http://$pduIp/outlet/$outletId/off"
Start-Sleep -Seconds 5
Invoke-RestMethod -Method Post -Uri "http://$pduIp/outlet/$outletId/on"
xbconnect $ip /b /q /ws:120   # Wait for console to come back

# Simulated power pull (no PDU needed)
xbrun /x:$ip /x:/system /O kill.exe -f $pid   # Kill ConnectedStorage service
```

### 11. Connected standby

```powershell
xbreboot /x $ip /P     # Enter connected standby (Instant On mode required)
xbreboot /x $ip /W     # Wake from connected standby (magic packet)
xbreboot /x $ip /Q     # Query standby state and power mode
```

Use ``/W:$mac`` to skip MAC lookup if known.

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: Standby

- command: ChangeTargetDeviceState
  parameters:
    action: Wake

- command: ChangeTargetDeviceState
  parameters:
    action: QueryPowerState
```

### 12. Boot after power event

```powershell
xbconnect $ip /b /q /ws:120   # Block until console responds, 120s timeout
```

After reboot, a console may briefly respond then go unresponsive during late boot — verify 2–3 times with short delays:
```powershell
$start = Get-Date
do {
    xbconnect $xboxIp /Q 2>$null
    if ($LASTEXITCODE -eq 0) { break }
    Start-Sleep -Seconds 1
} while (((Get-Date) - $start).TotalSeconds -lt $timeout)
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: WaitForBoot
    timeoutSeconds: 120
```

---

## D. User / Auth Management

**Scenarios**: 19–25, 33

### 13. Sign out Xbox user / 14. Sign in Xbox user

```powershell
xbuser /x $ip signout /e:testuser@xboxtest.com   # Sign out by email
xbuser /x $ip signout /i:$userId                  # Sign out by user ID
xbuser /x $ip signin  /e:testuser@xboxtest.com /p:password  # Sign in
xbuser /x $ip list                                 # List all users on console
xbuser /x $ip add /e:newuser@xboxtest.com          # Add a user
xbuser /x $ip delete /e:olduser@xboxtest.com       # Remove a user
xbuser /x $ip deleteall                             # Remove all users
```

Password is stored on the console after first use, so ``/p:`` is optional on subsequent sign-ins.

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: SignOut
    email: testuser@xboxtest.com

- command: ChangeTargetDeviceState
  parameters:
    action: SignIn
    email: testuser@xboxtest.com
    password: password

- command: ChangeTargetDeviceState
  parameters:
    action: ListUsers

- command: ChangeTargetDeviceState
  parameters:
    action: AddUser
    email: newuser@xboxtest.com

- command: ChangeTargetDeviceState
  parameters:
    action: DeleteUser
    email: olduser@xboxtest.com

- command: ChangeTargetDeviceState
  parameters:
    action: DeleteAllUsers
```

### 15. Switch user

Sign out one user and sign in another without closing the app:
```powershell
xbuser /x $ip signout /e:userA@xboxtest.com
xbuser /x $ip signin  /e:userB@xboxtest.com /p:password
```

The app remains running — it receives user-change notifications via the standard Xbox callback mechanism.

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: SwitchUser
    signOutEmail: userA@xboxtest.com
    signInEmail: userB@xboxtest.com
    password: password
```

### 16. SPOP trigger

⚠️ **Partial solution.** SPOP requires signing in the same account on another device to force auth expiry. Multi-device orchestration can trigger SPOP via `xbuser signin` on a second console. To detect and react to the resulting auth change, set up ETW listeners on the device via **tvpp/tlc** to watch for the auth expiry event — though it's unclear if this works on green-signed (retail-equivalent) dev kits. The resulting Xbox system UI (re-sign-in prompt) may still require manual dismissal or UI automation (FlaUI-MCP).

```powershell
# Trigger SPOP from test controller: sign same user in on Console 2
xbuser /x $console2Ip signin /e:testuser@xboxtest.com /p:password

# Set up ETW listener on Console 1 to detect auth expiry (needs investigation for green-signed)
# tvpp/tlc can capture XUser auth events on the device
```

---

## E. Dashboard / System UI Actions

**Scenarios**: 10–12, 53

### 17. Delete Everywhere

**On console** — ``xbstorage delete`` deletes a Connected Storage space for a specific user and SCID:
```powershell
# Delete per-user Connected Storage space
xbstorage /x:$ip delete /msa:testuser@xboxtest.com /scid:$scid

# Delete per-machine Connected Storage space
xbstorage /x:$ip delete /machine /scid:$scid

# Factory reset ALL Connected Storage on the console
xbstorage /x:$ip reset /force
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: DeleteStorage
    msa: testuser@xboxtest.com
    scid: $scid

- command: ChangeTargetDeviceState
  parameters:
    action: ResetAllStorage
```

**Off console (PC / CI)** — ``pfgamesaveutil reset`` (source: ``PlayFab.C\Tools\pfgamesaveutil``) deletes cloud data, local data, or both:
```powershell
# Delete cloud saves
pfgamesaveutil reset -t $titleId -k $secretKey -p $playerId --cloud

# Delete local PGS folder
pfgamesaveutil reset -t $titleId --local

# Delete both cloud and local
pfgamesaveutil reset -t $titleId -k $secretKey -p $playerId --all -f
```

### 18. Move to background / 19. Return to foreground

```powershell
xbapp /x $ip suspend $gamePackageName   # Push game to background
xbapp /x $ip resume  $gamePackageName   # Bring game back to foreground
```

For a more realistic "another app takes focus" scenario:
```powershell
xbapp /x $ip launch $stubPackageName     # game goes to background
xbapp /x $ip terminate $stubPackageName  # game returns to foreground
```

**YAML** (uses Suspend/Resume from section B, or EvictGame/Terminate for realistic scenario):
```yaml
# Push to background via suspend
- command: ChangeTargetDeviceState
  parameters:
    action: Suspend

# Return to foreground via resume
- command: ChangeTargetDeviceState
  parameters:
    action: Resume
```

### 20. Quick Resume save / 21. Quick Resume restore

``xbapp save`` captures the current Quick Resume state of a title. ``xbapp restore`` relaunches it from that saved state — this is how the test framework can deterministically trigger and verify Quick Resume behavior (used by ID63–ID66).

```powershell
# Save QR state of the running title
xbapp /x $ip save $aumid /State:pretest /Description:"Before cloud state change"

# ... do something (e.g., modify cloud state from another device) ...

# Restore from QR — title resumes with stale in-memory state
xbapp /x $ip restore $aumid /State:pretest

# Manage roaming QR states
xbapp /x $ip quickresume /List              # List all saved states
xbapp /x $ip quickresume /Delete pretest    # Clean up
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: QuickResumeSave
    state: pretest

- command: ChangeTargetDeviceState
  parameters:
    action: QuickResumeRestore
    state: pretest

- command: ChangeTargetDeviceState
  parameters:
    action: QuickResumeList

- command: ChangeTargetDeviceState
  parameters:
    action: QuickResumeDelete
    state: pretest
```

### 22. Simulate out-of-storage / 23. Stop storage simulation

``xbstorage simulate`` forces the Connected Storage service to report out-of-storage conditions. Used by ID44–ID49 (out-of-storage scenarios).

```powershell
# Reserve all remaining space — deleting CS frees space
xbstorage /x:$ip simulate /reserveremainingspace

# Force permanent out-of-storage — deleting CS does NOT free space
xbstorage /x:$ip simulate /forceoutoflocalstorage

# Stop all simulations
xbstorage /x:$ip simulate /stop
```

**YAML**:
```yaml
- command: ChangeTargetDeviceState
  parameters:
    action: SimulateOutOfStorage
    mode: reserveRemaining          # or "forceOutOfStorage" (default)

- command: ChangeTargetDeviceState
  parameters:
    action: StopStorageSimulation
```

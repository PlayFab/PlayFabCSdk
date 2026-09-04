# GameTestController CLI Mode Guide

> A hands-on guide to using the GameTestController in CLI mode for testing
> PFGameSave across local (in-proc), Xbox, and PlayStation devices.

## Table of Contents

- [Quick Start](#quick-start)
- [Architecture Overview](#architecture-overview)
- [Launching the Controller](#launching-the-controller)
- [CLI Commands Reference](#cli-commands-reference)
- [API Commands](#api-commands)
- [Batch Files & Paste](#batch-files--paste)
- [Testing Workflows](#testing-workflows)
  - [In-Proc to In-Proc (local two-device)](#workflow-1-in-proc-to-in-proc)
  - [In-Proc to Xbox](#workflow-2-in-proc-to-xbox)
  - [PlayStation to Xbox](#workflow-3-playstation-to-xbox)
- [Connecting Remote Devices](#connecting-remote-devices)
- [Troubleshooting](#troubleshooting)

---

## Quick Start

```
# 1. Build the controller and local test app
Utilities\Scripts\tests-build.ps1

# 2. Launch the controller in CLI mode
Out\x64\Debug\GameTestController\GameTestController.exe -cli

# 3. Launch two local devices
> launch
> launch

# 4. Initialize DeviceA and run commands
> batch specs\playfab-gamesave\testing\cli\batches\init-device.txt
> PFGameSaveFilesUploadWithUiAsync --mode=ReleaseDeviceAsActive
```

---

## Architecture Overview

```
┌─────────────────────────────────┐
│   GameTestController (C# CLI)   │
│   ws://localhost:15080/ws/       │
└──────┬──────────┬───────────────┘
       │ WebSocket│
  ┌────▼────┐ ┌───▼──────┐  ┌──────────────┐
  │ DeviceA │ │ DeviceB  │  │ Remote Xbox  │
  │ (local) │ │ (local)  │  │ /controller  │
  │ inproc  │ │ inproc   │  │  <ctrl-ip>   │
  └─────────┘ └──────────┘  └──────────────┘
```

The controller is a C# application that listens on `ws://localhost:15080/ws/` for
device connections. Each device (local or remote) connects over WebSocket,
announces its capabilities (engine type and supported commands), and receives
the name **DeviceA** or **DeviceB** in connection order.

**Engine types** reported by devices:

| Engine              | Device Type                                   |
|---------------------|-----------------------------------------------|
| `pc-inproc-gamesaves` | GameTestAppWindows (local PC, in-proc game saves) |
| `xbox`              | GameTestAppXbox (Xbox console dev kit)         |
| `psx`               | GameTestAppPS (PlayStation dev kit)            |

---

## Launching the Controller

```
# From the repo root — standard CLI mode
Out\x64\Debug\GameTestController\GameTestController.exe -cli
```

On startup you'll see:
```
[HH:MM:SS] WebSocket server started at http://+:15080/ws/
Game Test Controller CLI
Type 'help' for built-in commands, 'apihelp' for API commands, 'exit' to quit

>
```

> **Tip:** If you get "Access is denied", register the URL ACL once as Admin:
> `netsh http add urlacl url=http://+:15080/ws/ user=Everyone`

---

## CLI Commands Reference

All commands share a single `>` prompt. Built-in commands are handled locally;
anything else is forwarded to the first connected device as an API call.

| Command           | Aliases    | Description                                              |
|-------------------|------------|----------------------------------------------------------|
| `launch`          |            | Launch the next local test device (DeviceA, then DeviceB) |
| `list-devices`    |            | Show all connected devices and their status              |
| `run <file>`      |            | Run an automated YAML scenario                           |
| `target [device]`| | Set or show the targeted device (e.g., `target DeviceB`) |
| `batch <path>`    |            | Run commands from a text file, one per line              |
| `paste`           |            | Run commands from the clipboard                          |
| `apihelp [filter]`| `api`      | List API commands from connected device (substring filter)|
| `help [command]`  | `?`, `h`   | Show built-in commands or details for a specific one     |
| `exit`            | `quit`, `q`| Quit the CLI                                             |
| *anything else*   |            | Forwarded to the first connected device as an API command |

Tab completion works for both built-in and API commands.

---

## API Commands

Any command not recognized as a built-in is sent to the first connected device.
Use the format:

```
> CommandName --param1=value1 --param2=value2
```

Use `apihelp` to list all available API commands, or `apihelp <filter>` to
search by substring (e.g., `apihelp Upload`).

### Key PFGameSave Commands

**Lifecycle:**
- `PFGameSaveFilesInitialize --saveFolder=C:\gamesave`
- `PFGameSaveFilesAddUserWithUiAsync`
- `PFGameSaveFilesUninitializeAsync`

**Upload & Download:**
- `PFGameSaveFilesUploadWithUiAsync --mode=ReleaseDeviceAsActive`
- `PFGameSaveFilesGetFolder`
- `PFGameSaveFilesGetFolderSize`
- `PFGameSaveFilesGetRemainingQuota`

**Save Descriptions:**
- `PFGameSaveFilesSetSaveDescriptionAsync --description=My Save`
- `PFGameSaveFilesGetSaveDescription`

**UI Auto-Responses** (set before operations to avoid blocking dialogs):
- `PFGameSaveFilesSetUiConflictResponse --response=UseLocal`
- `PFGameSaveFilesSetUiOutOfStorageResponse --response=Cancel`
- `PFGameSaveFilesSetUiProgressResponse --response=Continue`
- `PFGameSaveFilesSetUiSyncFailedResponse --response=Cancel`
- `PFGameSaveFilesSetUiActiveDeviceContentionResponse --response=SyncLastSavedData`

**Debug:**
- `PFGameSaveFilesResetCloudAsync` — delete all cloud data for the current user
- `PFGameSaveFilesSetMockDeviceIdForDebug --deviceId=TestDevice1`
- `PFGameSaveFilesSetActiveDevicePollIntervalForDebug --intervalMs=5000`

**Test Utilities:**
- `WriteGameSaveData` — write test files to the save folder
- `DeleteSaveRoot` — wipe the local save folder
- `DeleteLocalFolder --folderPath=C:\gamesave\DeviceA`
- `CaptureSaveContainerSnapshot --slot=left` — snapshot for comparison
- `CompareSaveContainerSnapshots --ignoreTimestamps=true`

---

## Batch Files & Paste

### batch

Run a text file where each line is a command. Blank lines and `#` comments are
skipped. Execution stops on the first failure.

```
> batch specs\playfab-gamesave\testing\cli\batches\init-device.txt
```

### paste

Copy a block of commands to your clipboard, then run them:

```
> paste
```

The clipboard text is split on newlines and executed identically to `batch`.

### Provided Batch Files

These files live in `specs/playfab-gamesave/testing/cli/batches/`:

**Device Initialization (pick one per platform):**

| File                      | Purpose                                              |
|---------------------------|------------------------------------------------------|
| `init-device.txt`         | Local/in-proc: synthetic CustomID user (no platform identity) |
| `init-device-pc-xbox.txt` | PC with Xbox Live sign-in (triggers SPOP for MSA login) |
| `init-device-xbox.txt`    | Xbox console: creates local user from XUserHandle    |
| `init-device-psx.txt`     | PlayStation: creates local user from PSN, attempts login¹ |
| `link-psn-to-xbox.txt`    | PlayStation: links PSN to Xbox-backed PlayFab account¹ |
| `link-xbox-to-customid.txt` | Links Xbox Live identity to a CustomID-backed account |

¹ *PSN commands (`PFLocalUserCreateHandleWithPSNUser`, `PFAuthenticationLoginWithPSNAsync`,
`PFAccountManagementClientLinkPSNAccountAsync`) are assumed to exist in GameTestAppPS.
Verify exact command names against the GameTestAppPS command registry before first use.
These are marked with `[ASSUMED]` comments in the batch files.*

**Common Operations (platform-independent):**

| File                      | Purpose                                              |
|---------------------------|------------------------------------------------------|
| `teardown-device.txt`     | Clean handle close and runtime uninitialize           |
| `upload-golden-path.txt`  | Write test data, upload, capture snapshot             |
| `download-and-verify.txt` | Download from cloud, capture snapshot, compare        |
| `set-auto-responses.txt`  | Pre-set all UI auto-responses for unattended testing  |
| `reset-cloud.txt`         | Delete all cloud save data for the current user       |

**Full Workflows (multi-device, include `target` switching):**

| File                              | Purpose                                      |
|-----------------------------------|----------------------------------------------|
| `workflow-inproc-to-inproc.txt`   | Upload on DeviceA (local), download/verify on DeviceB (local) |
| `workflow-inproc-to-xbox.txt`     | Upload on DeviceA (local), download/verify on DeviceB (Xbox)  |
| `workflow-psx-to-xbox.txt`        | Upload on DeviceA (Xbox), download/verify on DeviceB (PSX)¹   |

Run a full workflow in one shot: `> batch specs\playfab-gamesave\testing\cli\batches\workflow-inproc-to-inproc.txt`

### Account Linking for Cross-Platform Testing

When testing across Xbox and PlayStation, both devices must share the same
PlayFab entity so they see the same cloud save. The Xbox identity (MSA/XUser)
serves as the **primary linking identity** per the
[Account Linking Strategies](../../../../playfab-docs-pr/playfab-docs/player-progression/game-saves/linking.md)
doc.

**First-time setup (run once per test user):**

1. On the **Xbox** device, run `init-device-xbox.txt` — this creates the
   Xbox-backed PlayFab account with `createAccount=true`.
2. On the **PlayStation** device, run `init-device-psx.txt` — this creates
   the PSN local user and attempts to login.
3. If login fails with `E_PF_ACCOUNT_NOT_FOUND` (PSN not yet linked), run
   `link-psn-to-xbox.txt` on the PlayStation device to link.

**Subsequent sessions:** `init-device-psx.txt` succeeds at login directly
because the link is already in place.

---

## Testing Workflows

### Workflow 1: In-Proc to In-Proc

Two local `GameTestAppWindows` instances on the same machine. Good for
validating the upload→download→compare cycle without needing console hardware.

```
# Launch both devices
> launch
> launch
> list-devices

# Initialize DeviceA
> batch specs\playfab-gamesave\testing\cli\batches\init-device.txt

# Reset cloud state for a clean test
> batch specs\playfab-gamesave\testing\cli\batches\reset-cloud.txt

# Upload from DeviceA
> batch specs\playfab-gamesave\testing\cli\batches\upload-golden-path.txt

# Tear down DeviceA
> batch specs\playfab-gamesave\testing\cli\batches\teardown-device.txt
```

Now switch to DeviceB:

```
# Retarget to DeviceB
> target DeviceB

# Initialize DeviceB
> batch specs\playfab-gamesave\testing\cli\batches\init-device.txt

# Download and verify
> batch specs\playfab-gamesave\testing\cli\batches\download-and-verify.txt

# Tear down DeviceB
> batch specs\playfab-gamesave\testing\cli\batches\teardown-device.txt

# Reset target back to default
> target auto
```

**Alternative — use the automated scenario:**
```
> run gamesave-02-two-device-golden-path.yml
```

---

### Workflow 2: In-Proc to Xbox

One local `GameTestAppWindows` (DeviceA) and one `GameTestAppXbox` running on
an Xbox dev kit (DeviceB). The Xbox must be able to reach the controller's IP
on port 15080.

#### 1. Start the controller

```
Out\x64\Debug\GameTestController\GameTestController.exe -cli
```

#### 2. Launch the local device

```
> launch
```

DeviceA connects as `pc-inproc-gamesaves`.

#### 3. Start GameTestAppXbox on the dev kit

Deploy `GameTestAppXbox` to the Xbox dev kit via Visual Studio or `xbapp`
tools. Pass the controller's IP address:

```
/controller 192.168.1.100
```

The Xbox device connects as `xbox` and is assigned **DeviceB**.

#### 4. Verify both devices

```
> list-devices
Connected devices (2):
  DeviceA [pc-inproc-gamesaves]
    Endpoint: 127.0.0.1:xxxxx
    Status:   Ready

  DeviceB [xbox]
    Endpoint: 192.168.1.50:xxxxx
    Status:   Ready
```

#### 5. Run a cross-device scenario

**Option A — Automated YAML scenario:**
```
> run gamesave-02-two-device-golden-path.yml
```

**Option B — Interactive with batch files:**
```
# Initialize DeviceA (local in-proc — uses CustomID, no linking needed)
> batch specs\playfab-gamesave\testing\cli\batches\init-device.txt
> batch specs\playfab-gamesave\testing\cli\batches\reset-cloud.txt
> batch specs\playfab-gamesave\testing\cli\batches\upload-golden-path.txt
> batch specs\playfab-gamesave\testing\cli\batches\teardown-device.txt

# Switch to DeviceB (Xbox) for the download side
> target DeviceB
> batch specs\playfab-gamesave\testing\cli\batches\init-device-xbox.txt
> batch specs\playfab-gamesave\testing\cli\batches\download-and-verify.txt
```

#### 6. Tear down

```
> batch specs\playfab-gamesave\testing\cli\batches\teardown-device.txt
> exit
```

> **Note:** In-proc to Xbox testing uses different auth paths: the local device
> uses a synthetic CustomID user (`init-device.txt`) while Xbox uses a real
> XUserHandle (`init-device-xbox.txt`). For the two devices to share a cloud
> save, they must resolve to the same PlayFab entity. In the test harness this
> is handled by the `GameTestHarness` persisted local ID mapping to the same
> title player account.

---

### Workflow 3: PlayStation to Xbox

One `GameTestAppPS` on a PlayStation dev kit and one `GameTestAppXbox` on an
Xbox dev kit. Both connect to the controller running on your PC.

#### 1. Start the controller

```
Out\x64\Debug\GameTestController\GameTestController.exe -cli
```

#### 2. Start GameTestAppXbox on the Xbox dev kit FIRST

Start Xbox first so it gets DeviceA — this lets you initialize the
Xbox-backed PlayFab account before PlayStation needs to link to it.

Deploy `GameTestAppXbox` and pass the controller IP:

```
/controller 192.168.1.100
```

The device connects as `xbox` and is assigned **DeviceA**.

#### 3. Start GameTestAppPS on the PlayStation dev kit

Deploy `GameTestAppPS` to the dev kit and pass the controller IP:

```
/controller 192.168.1.100
```

The device connects as `psx` and is assigned **DeviceB**.

#### 4. Verify

```
> list-devices
Connected devices (2):
  DeviceA [xbox]
    Endpoint: 192.168.1.50:xxxxx
    Status:   Ready

  DeviceB [psx]
    Endpoint: 192.168.1.60:xxxxx
    Status:   Ready
```

#### 5. Initialize and link accounts

The Xbox identity serves as the **primary linking identity**. Initialize
Xbox first, then set up PlayStation with linking.

```
# Set auto-responses so remote consoles don't block on dialogs
> batch specs\playfab-gamesave\testing\cli\batches\set-auto-responses.txt

# Initialize DeviceA (Xbox) — creates the Xbox-backed PlayFab account
> batch specs\playfab-gamesave\testing\cli\batches\init-device-xbox.txt
> batch specs\playfab-gamesave\testing\cli\batches\reset-cloud.txt

# Switch to DeviceB (PlayStation) and link
> target DeviceB
> batch specs\playfab-gamesave\testing\cli\batches\set-auto-responses.txt
> batch specs\playfab-gamesave\testing\cli\batches\init-device-psx.txt

# If login failed with E_PF_ACCOUNT_NOT_FOUND (first time), link PSN → Xbox:
> batch specs\playfab-gamesave\testing\cli\batches\link-psn-to-xbox.txt
```

#### 6. Test cross-platform save sync

With both devices linked to the same PlayFab entity, upload from one and
download on the other:

```
# Upload from Xbox (DeviceA)
> target DeviceA
> batch specs\playfab-gamesave\testing\cli\batches\upload-golden-path.txt

# Download and verify on PlayStation (DeviceB)
> target DeviceB
> batch specs\playfab-gamesave\testing\cli\batches\download-and-verify.txt
```

#### 7. Tear down

```
> batch specs\playfab-gamesave\testing\cli\batches\teardown-device.txt
> exit
```

> **Important:** When testing across platforms:
> - Always set auto-responses before running operations — you can't interact
>   with dialogs on remote consoles from the CLI.
> - Initialize Xbox first so the primary linking identity account exists
>   before PlayStation tries to link.
> - The linking step (`link-psn-to-xbox.txt`) only needs to run once per test
>   user. On subsequent sessions, `init-device-psx.txt` succeeds directly.

---

## Connecting Remote Devices

Devices on the local network connect to the controller's WebSocket endpoint.
The default URI is `ws://localhost:15080/ws/`.

**On the device** (Xbox, PlayStation, or a remote PC), pass the controller
machine's IP:

```
GameTestAppXbox.exe /controller 192.168.1.100
GameTestAppPS.exe   /controller 192.168.1.100
```

**Firewall:** Ensure port 15080 (TCP) is open on the controller machine.

**Connection lifecycle:**
1. Device connects to `ws://<controller-ip>:15080/ws/`
2. Device sends a `capabilities` JSON message (engine type + command list)
3. Controller assigns the device name (DeviceA or DeviceB) in connection order
4. Controller sends the assignment back; device is now **Ready**

Devices auto-reconnect every 2 seconds if the connection drops.

---

## Xbox Console Testing Notes

Running GameTestAppXbox on a dev kit has several platform-specific behaviors
compared to local in-proc testing.

### Controller IP Setup

The Xbox app reads `controllerip.txt` (in the deployed package) to know where
the controller is. Update this file with the controller PC's Ethernet IP
address before deploying:

```
# Test\GameTestAppXbox\controllerip.txt — set to your PC's IP
10.124.184.53
```

Verify the PC and Xbox are on the same subnet and port 15080 is open:
```powershell
# Create firewall rule (run as admin, once)
New-NetFirewallRule -DisplayName "GameTestController WebSocket (TCP 5000)" `
    -Direction Inbound -Protocol TCP -Localport 15080 -Action Allow -Profile Any
```

### Xbox Authentication

Xbox uses XUser (Xbox Live) authentication, not CustomID. The YAML scenarios
in `gamesave-xbox-*.yml` use `XUserAddAsync` + `PFLocalUserCreateHandleWithXboxUser`
instead of `PFLocalUserCreateHandleWithPersistedLocalId`.

**Requirements:**
- An Xbox Live user must be signed in on the dev kit
- `XUserAddAsync` with `AddDefaultUserSilently` succeeds automatically when a
  user is signed in; otherwise it falls back to a system UI picker dialog
  (which blocks and eventually times out if nobody interacts)
- On Xbox, `PFGameSaveFilesInitialize` should NOT receive a `saveFolder`
  parameter — the GRTS (Game Runtime Service) manages the save location

### Xbox Save Folder

On Xbox, the GRTS manages the save folder location. Do not pass `saveFolder`
to `PFGameSaveFilesInitialize`. The test harness commands `DeleteSaveRoot`,
`CaptureSaveContainerSnapshot`, and `CompareSaveContainerSnapshots` are no-ops
on Xbox since the platform manages the folder lifecycle.

### Xbox YAML Scenarios

Dedicated Xbox scenarios live in `Test/GameTestScenarios/`:

| File | Description |
|------|-------------|
| `gamesave-xbox-01-single-device-golden-path.yml` | Single-device upload + re-download using XUser auth |
| `gamesave-xbox-02-two-device-golden-path.yml` | Two-device cross-upload with XUser auth |

These differ from the standard `gamesave-01/02` scenarios in:
1. **Auth:** `XUserAddAsync` + `PFLocalUserCreateHandleWithXboxUser` (not CustomID)
2. **No saveFolder:** `PFGameSaveFilesInitialize` has no `saveFolder` parameter
3. **No snapshot compare:** `CaptureSaveContainerSnapshot` / `CompareSaveContainerSnapshots` removed
4. **XUser cleanup:** `XUserCloseHandle` added to teardown and cleanup blocks

### Xbox Log Files

The Xbox app writes logs to `D:\` (system scratch drive):
- `D:\device-DeviceA-log.txt` — detailed trace log
- `D:\device-DeviceA-summary.txt` — command results summary

The controller automatically gathers these via `GatherLogs` at the end of each
scenario. To manually retrieve logs:

```powershell
# List files on Xbox D: drive
& "C:\Program Files (x86)\Microsoft GDK\bin\xbdir.exe" xd:\*.txt

# Copy logs to local machine
& "C:\Program Files (x86)\Microsoft GDK\bin\xbcopy.exe" xd:\ "C:\temp\xboxlogs\" "*.txt" /overwrite
```

> **Note:** Log files are locked while the app is running. `FlushFileBuffers`
> is called after each write so logs are readable, but `xbcopy` may fail with
> `0x80070020` (sharing violation) if the app holds the file. The controller's
> `GatherLogs` reads the file content over WebSocket to avoid this.

### Xbox Teardown

The Xbox teardown batch should include `XUserCloseHandle`:

```
PFLocalUserCloseHandle
XUserCloseHandle
PFGameSaveFilesUninitializeAsync
PFServicesUninitializeAsync
PFUninitializeAsync
PFServiceConfigCloseHandle
XTaskQueueCloseHandle
```

---

## Troubleshooting

| Symptom | Cause | Fix |
|---------|-------|-----|
| `list-devices` shows 0 devices | Device not started, or can't reach port 15080 | Check firewall, verify `/controller <ip>` arg |
| Device shows "Waiting for capabilities" | Device connected but hasn't announced commands yet | Wait a few seconds; check device logs for errors |
| `launch` says "DeviceA and DeviceB are already connected" | Both slots taken | Close a device window or restart the controller |
| Command returns HRESULT error | API call failed on the device | Check the HRESULT against PFGameSave error codes |
| `paste` says "Clipboard does not contain text" | Clipboard empty or contains non-text data | Copy your command list to the clipboard first |
| Xbox device won't connect | Network issue or wrong IP | Ping between machines; verify the `/controller` argument |
| Xbox `XUserAddAsync` fails with `0x80004004` | No user signed in, or UI picker timed out | Sign in an Xbox Live user on the dev kit before running |
| Xbox `PFGameSaveFilesAddUserWithUiAsync` returns `0x80070032` | Using CustomID auth instead of XUser on Xbox | Use `gamesave-xbox-*.yml` scenarios (XUser auth) |
| Xbox WebSocket disconnects after 2 seconds | HC refcount bug (internal cleanup tears down HTTP stack) | Fixed in DeviceWebSocketClient.cpp — always call `HCInitialize` |
| Xbox logs are empty (0 bytes) | File buffers not flushed to disk | Fixed — `FlushFileBuffers` called after every write |

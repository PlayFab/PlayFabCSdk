# GameTestAppXbox — Dev Spec

## Overview

Create a new test device application at `Test/GameTestAppXbox` that runs on **GDK PC** (`Gaming.Desktop.x64`), **Xbox Series X|S** (`Gaming.Xbox.Scarlett.x64`), and **Xbox One** (`Gaming.Xbox.XboxOne.x64`). This app participates in the same automated test infrastructure as the existing Windows test device — a remote test controller sends JSON commands over WebSocket and this app executes them.

### Goals

1. **Xbox console support** — the existing `GameTestAppWindows` builds for `Gaming.Desktop.x64` only; this new project adds Xbox console targets.
2. **Code sharing** — common (cross-platform) code lives in `Test/GameTestAppShared/` so both the Windows and Xbox projects reference the same files.
3. **No changes to GameTestAppWindows behavior** — the Windows project continues to work exactly as it does today; only its `.vcxproj` file paths are updated to point at the new shared location.
4. **Based on the working Xbox console sample** — `Samples/PlayFabGameSaveSample-XboxConsole` is the reference for project configuration, PLM handling, and Xbox console packaging.

### Rendering and Input

- **DirectX rendering with ATG framework** — same rendering infrastructure as `PlayFabGameSaveSample-XboxConsole` (D3D12, DirectXTK12, ATG TextConsole, SampleGUI, LiveResources, LiveInfoHUD).
- **Log output rendered on screen** — the ATG `TextConsole` displays scrolling log output in real time, replacing the GDI text rendering used by the Windows test device.
- **Minimal controller input** — gamepad support is limited to quit/exit; all test interaction is via WebSocket commands from the test controller.

---

## Architecture

### Directory Layout After Implementation

```
Test/
├── GameTestAppShared/          # ← NEW: extracted from Windows Common/
│   ├── DeviceApplication.h
│   ├── DeviceCommandHandlers.cpp/h
│   ├── DeviceCommandProcessor.cpp/h
│   ├── DeviceGameSave.cpp/h
│   ├── DeviceGameSaveState.h
│   ├── DeviceLogging.h
│   ├── DeviceWebSocketClient.cpp/h
│   ├── DeviceWebSocketConnection.cpp/h
│   ├── PFCommandHandlers.cpp/h
│   ├── XUserHandlers.cpp/h
│   ├── XblHandlers.cpp/h
│   ├── XGameRuntimeHandlers.cpp/h
│   ├── XTaskQueueHandlers.cpp/h
│   ├── ChaosModeHandler.cpp/h
│   ├── WriteGameSaveData.cpp/h
│   ├── GatherSnapshot.cpp/h
│   ├── CaptureSaveContainerSnapshot.cpp/h
│   ├── DeleteSaveRoot.cpp/h
│   ├── DeleteLocalFolder.cpp/h
│   ├── CopyTargetFolderToSaveFolder.cpp/h
│   ├── ConsumeDiskSpace.cpp/h
│   ├── GatherLogs.cpp/h
│   ├── GetDebugStats.cpp/h
│   ├── HttpMock.cpp/h
│   ├── HttpMockHandlers.cpp/h
│   ├── SmokeCommandHandlers.cpp/h
│   ├── CommandHandlerShared.h
│   └── HarnessHash.h
│
├── GameTestAppWindows/         # ← EXISTING (minimal change)
│   ├── Platform/Windows/               # Unchanged platform-specific files
│   │   ├── Main.cpp
│   │   ├── pch.cpp / pch.h
│   │   ├── DeviceApplication_Win32.cpp
│   │   ├── DeviceLogging_Win32.cpp
│   │   ├── DeviceGameSaveState_Win32.cpp
│   │   ├── DeviceWindow.cpp / DeviceWindow.h
│   │   └── HarnessHash_Win32.cpp
│   ├── MicrosoftGameConfig.mgc
│   └── GameTestAppWindows.vcxproj   # Include paths updated
│
└── GameTestAppXbox/            # ← NEW
    ├── Platform/Xbox/
    │   ├── Main_Xbox.cpp
    │   ├── pch_Xbox.cpp / pch_Xbox.h
    │   ├── DeviceApplication_Xbox.cpp
    │   ├── DeviceLogging_Xbox.cpp
    │   ├── DeviceGameSaveState_Xbox.cpp
    │   └── HarnessHash_Xbox.cpp
    ├── MicrosoftGameConfig.mgc
    └── GameTestAppXbox.vcxproj
```

### Code Sharing Strategy

All 46 files currently under `GameTestAppWindows/Common/` are **moved** to `GameTestAppShared/`. Both `.vcxproj` files reference them via relative paths:

| Project | Relative path to shared files |
|---------|-------------------------------|
| `GameTestAppWindows` | `..\GameTestAppShared\*.cpp` / `..\GameTestAppShared\*.h` |
| `GameTestAppXbox` | `..\GameTestAppShared\*.cpp` / `..\GameTestAppShared\*.h` |

The shared files compile into each project independently (no shared static lib). Each project's `pch.h` provides the platform-specific headers that the shared code depends on.

### Changes to GameTestAppWindows

The only changes to the Windows project are:

1. **`.vcxproj` include/compile paths** — change `Common\Foo.cpp` → `..\GameTestAppShared\Foo.cpp` for every shared file.
2. **`AdditionalIncludeDirectories`** — add `..\GameTestAppShared\` (replacing `$(ProjectDir)` implicit Common/ resolution).
3. **`pch.h`** — change `#include "Common/DeviceLogging.h"` → `#include "DeviceLogging.h"` (or leave as-is and add an include path that resolves it; the include-directory change above suffices).

No C++ source code changes. No behavior changes. The Windows project continues to build for `Gaming.Desktop.x64` and `Win32` exactly as before.

---

## Project Configuration (`.vcxproj`)

### Platform Targets

Modeled after `PlayFabGameSaveSample-XboxConsole.vcxproj`:

| Platform | Configurations |
|----------|----------------|
| `Gaming.Desktop.x64` | Debug, Release, Profile |
| `Gaming.Xbox.Scarlett.x64` | Debug, Release, Profile |
| `Gaming.Xbox.XboxOne.x64` | Debug, Release, Profile |

### Key Properties

```xml
<PropertyGroup Label="Globals">
  <RootNamespace>GameTestAppXbox</RootNamespace>
  <ProjectGuid>{NEW-GUID}</ProjectGuid>
  <DefaultLanguage>en-US</DefaultLanguage>
  <Keyword>Win32Proj</Keyword>
  <MinimumVisualStudioVersion>15.0</MinimumVisualStudioVersion>
  <TargetRuntime>Native</TargetRuntime>
  <GDKExtLibNames>Xbox.Services.API.C</GDKExtLibNames>
</PropertyGroup>
```

### SDK Imports

Same as `GameTestAppWindows`:

```xml
<Import Project="..\..\Build\PlayFab.C.GDK.props" />
<Import Project="$(PlayFabBuildRoot)\PlayFabCore.import.props" />
<Import Project="$(PlayFabBuildRoot)\PlayFabGameSave.import.props" />
<Import Project="$(PlayFabBuildRoot)\PlayFabSharedInternal.import.props" />
```

### Xbox Console Configuration

For Xbox platforms (`Gaming.Xbox.Scarlett.x64`, `Gaming.Xbox.XboxOne.x64`), follow the Xbox console sample patterns:

```xml
<!-- Xbox console platforms require these -->
<EmbedManifest>false</EmbedManifest>
<GenerateManifest>false</GenerateManifest>
```

### Preprocessor Defines

Same as the Windows test device, for all platforms:

```
PF_GAMESAVE_USE_GDK_PROVIDER;LIBARCHIVE_STATIC;%(PreprocessorDefinitions)
```

Plus the standard per-configuration defines (`_DEBUG`, `NDEBUG`, `PROFILE`, `__WRL_NO_DEFAULT_LIB__`).

### Include Directories

```xml
<AdditionalIncludeDirectories>
  $(ProjectDir);
  $(ProjectDir)Platform\Xbox\;
  ..\GameTestAppShared\;
  ..\..\Samples\Kits\LiveTK;
  ..\..\Samples\Kits\DirectXTK12\Inc;
  ..\..\Samples\Kits\ATGTK;
  ..\..\Samples\Kits\ATGTelemetry\GDK;
  $(ProjectDir)\..\..\Source\PlayFabGameSave\Include\;
  $(ProjectDir)\..\..\Source\PlayFabCore\Include\;
  $(ProjectDir)\..\..\Source\PlayFabCore\Include\GeneratedGdk\;
  $(ProjectDir)\..\..\Source\PlayFabGameSave\Include\GDK\;
  $(ProjectDir)\..\..\Source\PlayFabGameSave\Source\Api\;
  $(NLohmannIncludeDir)\;
  $(PlayFabExternalDir)\libarchive\libarchive\;
  $(PlayFabExternalDir)\libarchive.config\
</AdditionalIncludeDirectories>
```

### Link Dependencies

**Xbox console platforms:**
```xml
<AdditionalDependencies>uuid.lib;$(Console_Libs);%(XboxExtensionsDependencies);%(AdditionalDependencies)</AdditionalDependencies>
```

**Desktop platform:**
```xml
<AdditionalDependencies>$(Console_Libs);Microsoft.Xbox.Services.GDK.C.Thunks.lib;%(AdditionalDependencies)</AdditionalDependencies>
```

### Library Paths

**Xbox console platforms:**
```xml
<LibraryPath>$(Console_SdkLibPath);$(LibraryPath)</LibraryPath>
<IncludePath>$(Console_SdkIncludeRoot);$(IncludePath)</IncludePath>
```

**Desktop platform (Debug example):**
```xml
<LibraryPath>$(GameDKLatest)GRDK\ExtensionLibraries\Xbox.Services.API.C\Lib\x64\Debug;$(Console_SdkLibPath);$(LibraryPath)</LibraryPath>
```

### Post-Build Events (Desktop Only)

Copy the XSAPI thunks DLL and `xgameruntime.dll` to output, same as the Windows test device:

```xml
<ItemDefinitionGroup Condition="'$(Platform)'=='Gaming.Desktop.x64'">
  <PostBuildEvent>
    <Command>
      xcopy /y /d "$(GameDKLatest)GRDK\ExtensionLibraries\Xbox.Services.API.C\Lib\x64\$(Configuration)\Microsoft.Xbox.Services.GDK.C.Thunks.dll" "$(OutDir)"
      xcopy /y /d "$(GameDKLatest)windows\bin\x64\xgameruntime.dll" "$(OutDir)"
    </Command>
  </PostBuildEvent>
</ItemDefinitionGroup>
```

### Project References

```xml
<ProjectReference Include="..\..\Build\PlayFabCore.GDK\PlayFabCore.GDK.vcxproj">
  <Project>{d5c6a9a7-da63-4032-8ab2-3350f89162fc}</Project>
</ProjectReference>
```

---

## MicrosoftGameConfig.mgc

Use the same title identity as the Windows test device (same TitleId and MSAAppId so the same test controller and PlayFab title configuration works):

```xml
<?xml version="1.0" encoding="utf-8"?>
<Game configVersion="1">
  <Identity Name="41336MicrosoftATG.XboxLiveE2E"
            Publisher="CN=A4954634-DF4B-47C7-AB70-D3215D246AF1"
            Version="1.7.0.0" />

  <ExecutableList>
    <Executable Name="GameTestAppXbox.exe" Id="Game"/>
  </ExecutableList>

  <MSAFullTrust>false</MSAFullTrust>
  <MSAAppId>000000004C26FED0</MSAAppId>
  <TitleId>76029B4D</TitleId>

  <PersistentLocalStorage>
    <SizeMB>1024</SizeMB>
  </PersistentLocalStorage>

  <ShellVisuals DefaultDisplayName="PF GameSave Test Device (Xbox)"
                PublisherDisplayName="Xbox ATG"
                StoreLogo="Assets\StoreLogo.png"
                Square480x480Logo="Assets\LargeLogo.png"
                Square150x150Logo="Assets\Logo.png"
                Square44x44Logo="Assets\SmallLogo.png"
                Description="GameTestAppXbox"
                ForegroundText="dark"
                BackgroundColor="#000000"
                SplashScreenImage="Assets\SplashScreen.png"/>
</Game>
```

**Note:** Copy the `Assets/` folder from `GameTestAppWindows` (or create minimal placeholder PNGs). Xbox packaging requires these image assets to be present.

---

## Platform-Specific Files

### pch_Xbox.h

The precompiled header is based on the Xbox console sample's `pch.h`, including D3D12, DirectXTK12, and ATG framework headers. It also adds the PlayFab and shared test device headers that the shared code depends on.

Start from a copy of `Samples/PlayFabGameSaveSample-XboxConsole/pch.h` and append:

```cpp
// --- PlayFab SDK and test device additions (append to end of Xbox sample pch.h) ---

#include <playfab/services/PFServices.h>
#include <playfab/services/PFAccountManagement.h>
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/core/PFServiceConfig.h>
#include <playfab/core/PFLocalUser.h>

extern void ExitGame() noexcept;

#include "DeviceLogging.h"

HRESULT Sample_Xbox_AttemptSignIn();

inline std::string GetStringFromU8String(const std::string& u8str)
{
    return std::string(u8str.begin(), u8str.end());
}
```

The Xbox console sample's `pch.h` already provides D3D12, DirectXTK12, WRL, GDK, XUser, XTaskQueue, XSystem, XGameRuntime, XSAPI, PIX, `DX::ThrowIfFailed`, and `RETURN_IF_FAILED`.

### pch_Xbox.cpp

```cpp
// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch_Xbox.h"
```

### Main_Xbox.cpp

Entry point modeled after the Xbox console sample's `Main.cpp`. Includes:
- D3D12 device creation and swap chain (via ATG `DeviceResources`)
- PLM (Process Lifecycle Management) suspend/resume on Xbox console
- Standard Win32 message loop with `Tick()` rendering

The overall structure matches `PlayFabGameSaveSample-XboxConsole/Main.cpp` closely. The `WndProc` handles PLM via `WM_USER` on Xbox and standard resize/suspend on Desktop. Instead of the sample's interactive `Sample` class, we create a `TestDeviceApp` class (see DeviceApplication section below).

```
wWinMain / SampleMain
  ├── XMVerifyCPUSupport()
  ├── CoInitializeEx()
  ├── XGameRuntimeInitialize()
  ├── SetThreadAffinityMask (Xbox console only)
  ├── Create TestDeviceApp instance
  ├── Register window class, create HWND
  ├── RegisterAppStateChangeNotification (Xbox console only)
  ├── TestDeviceApp::Initialize(hwnd, width, height)
  ├── Message loop:
  │     ├── PeekMessage / TranslateMessage / DispatchMessage
  │     └── TestDeviceApp::Tick()  ← renders frame + pumps WebSocket
  ├── Cleanup
  ├── UnregisterAppStateChangeNotification (Xbox console only)
  └── XGameRuntimeUninitialize()
```

PLM handling follows the Xbox console sample pattern:

```cpp
#ifdef _GAMING_XBOX
HANDLE g_plmSuspendComplete = nullptr;
HANDLE g_plmSignalResume = nullptr;

// In WndProc, WM_USER:
//   Signal suspend complete, wait for resume signal
// RegisterAppStateChangeNotification callback:
//   On quiesce: post WM_USER, wait for suspend-complete event
//   On resume: signal resume event
#endif
```

On suspend, the app should:
1. Flush WebSocket state (no need to disconnect; the OS handles network suspension)
2. Set the `g_plmSuspendComplete` event

On resume, the app should:
1. Resume the message loop
2. WebSocket auto-reconnect will re-establish the connection naturally

### DeviceApplication_Xbox.cpp / TestDeviceApp class

The main application class, modeled after `PlayFabGameSaveSample-XboxConsole.h/.cpp` but with test device logic instead of interactive sample UI. Uses the ATG framework for rendering.

**Key components from the Xbox console sample to include:**
- `ATG::DeviceResources` — D3D12 device, swap chain, render targets
- `ATG::TextConsole` — scrolling text console for rendering log output on screen
- `ATG::LiveResources` — Xbox Live sign-in management
- `ATG::LiveInfoHUD` — displays gamertag/gamerpic on screen
- `DirectX::GamePad` — for quit input only (e.g., View+Menu to exit)
- `StepTimer` — frame timing

**Tick loop:**
```cpp
void TestDeviceApp::Tick()
{
    m_timer.Tick([this]()
    {
        Update(m_timer);       // pump WebSocket, process commands
    });

    Render();                  // draw text console to screen
}

void TestDeviceApp::Update(const DX::StepTimer& timer)
{
    // Check gamepad for quit combo
    auto pad = m_gamePad->GetState(0);
    if (pad.IsConnected())
    {
        if (pad.IsViewPressed() && pad.IsMenuPressed())
        {
            ExitGame();
        }
    }

    PumpWebSocketAutoConnect(m_state);
}
```

**Rendering:**
Log output is rendered via `ATG::TextConsole`. The `LogToWindow()` implementation writes text to the `TextConsole` instance, which handles scrolling and rendering automatically.

**Initialize flow:**
```
TestDeviceApp::Initialize(hwnd, width, height)
  ├── Create DeviceResources, set HWND
  ├── CreateDeviceDependentResources()  ← load fonts, create TextConsole
  ├── CreateWindowSizeDependentResources()
  ├── DetectSampleDeviceEngineType() + SetSampleDeviceEngineType()
  ├── WebSocket client Initialize()
  ├── Sample_GameSave_ConfigureWebSocketLogging(state)
  ├── Parse command line arguments
  ├── Create XTaskQueue
  └── ATG::LiveResources / LiveInfoHUD setup
```

**Kits dependencies:**
The project references the same ATG Kits as the Xbox console sample:
```
../Kits/ATGTK/       — TextConsole, SampleGUI, ControllerFont, StringUtil, Json
../Kits/LiveTK/      — LiveResources, LiveInfoHUD
../Kits/DirectXTK12/ — SpriteBatch, SpriteFont, GamePad, Keyboard, etc.
../Kits/ATGTelemetry/ — ATGTelemetry (optional)
```

These are included via `AdditionalIncludeDirectories` and compiled directly into the project (same as the Xbox console sample).

### DeviceLogging_Xbox.cpp

Implements the same logging interface as `DeviceLogging_Win32.cpp` (`DeviceLogging.h`), but renders log output via `ATG::TextConsole` instead of GDI:

| Function | Xbox Implementation |
|----------|---------------------|
| `LogToWindow()` | Write to `ATG::TextConsole` + `OutputDebugStringA()` + file logging |
| `LogToWindowFormat()` | Write to `ATG::TextConsole` + `OutputDebugStringA()` + file logging |
| `LogToWindowVerbose()` | File logging only (no screen output) |
| `LogToWindowFormatVerbose()` | File logging only (no screen output) |
| `EnableFileLoggingForDevice()` | File logging to persistent local storage path |
| `CloseLogFile()` | Same as Win32 |
| `FlushDeviceLogFile()` | Same as Win32 |
| `TryGetCurrentLogFilePath()` | Same as Win32 |
| `ResolveLogFilePathForDevice()` | Same as Win32 |
| `InitializeHCTraceToVerboseLog()` | Same as Win32 (HCTrace API is cross-platform) |
| `DeviceLoggingInitializePlatform()` | Store pointer to `ATG::TextConsole` instance |
| `DeviceLoggingPaintPlatform()` | No-op (TextConsole renders itself during `Render()`) |
| `UpdateWindowTitleWithDeviceName()` | Update LiveInfoHUD subtitle or `OutputDebugStringA` (no settable window title on console) |
| `DeviceLoggingHandleDestroy()` | `CloseLogFile()`, clear TextConsole pointer |

**Key difference from Win32:** Instead of `InvalidateRect(hwnd)` to trigger GDI repaint, `LogToWindow` writes directly to the `ATG::TextConsole` which is rendered each frame by the `Render()` method. The TextConsole handles scrolling automatically.

```cpp
// Global pointer set during DeviceLoggingInitializePlatform
static ATG::TextConsole* g_textConsole = nullptr;

void DeviceLoggingInitializePlatform(void* platformContext)
{
    g_textConsole = static_cast<ATG::TextConsole*>(platformContext);
}

// In LogInternal, after timestamping:
if (showOnScreen && g_textConsole)
{
    g_textConsole->Write(timestamped.c_str());
}
OutputDebugStringA(timestamped.c_str());
OutputDebugStringA("\n");
```

### DeviceGameSaveState_Xbox.cpp

This file provides the global state singleton and engine-type detection. The Xbox version is **nearly identical** to the Win32 version because `DeviceGameSaveState_Win32.cpp` already handles Xbox console detection via `XSystemGetDeviceType()`:

```cpp
static DeviceGameSaveState g_deviceGameSaveState;

DeviceGameSaveState* GetSampleGameSaveState()
{
    return &g_deviceGameSaveState;
}

void SetSampleDeviceEngineType(DeviceEngineType engineType)
{
    g_deviceGameSaveState.engineType = engineType;
}

DeviceEngineType DetectSampleDeviceEngineType()
{
    // XSystemGetDeviceType is available on all GDK platforms
    auto deviceType = XSystemGetDeviceType();

    if (deviceType == XSystemDeviceType::XboxOne ||
        deviceType == XSystemDeviceType::XboxOneS ||
        deviceType == XSystemDeviceType::XboxOneX ||
        deviceType == XSystemDeviceType::XboxOneXDevkit ||
        deviceType == XSystemDeviceType::XboxScarlettLockhart ||
        deviceType == XSystemDeviceType::XboxScarlettAnaconda ||
        deviceType == XSystemDeviceType::XboxScarlettDevkit)
    {
        return DeviceEngineType::Xbox;
    }

    // Desktop GDK PC — same detection as Win32 version
    const bool outOfProcAvailable = IsOutOfProcGRTSAvailable();
    const bool forceLocalServices = IsForceUseLocalServicesEnabled();

    if (!outOfProcAvailable || forceLocalServices)
    {
        return DeviceEngineType::PcInproc;
    }

    if (IsForceUseInprocGameSavesRegkeySet())
    {
        return DeviceEngineType::PcInprocGameSaves;
    }

    return DeviceEngineType::PcGrts;
}
```

The registry helper functions (`IsOutOfProcGRTSAvailable`, `IsForceUseLocalServicesEnabled`, `IsForceUseInprocGameSavesRegkeySet`) are needed for Desktop builds but not for Xbox console. They use `RegOpenKeyEx` which is available on both platforms (GDK provides registry APIs on console for compatibility), but on Xbox console the `IsRunningOnXboxConsole()` check returns early before they are called.

**Note:** The existing Win32 version's `#if HC_PLATFORM == HC_PLATFORM_GDK` guard already handles this correctly. The Xbox version can copy the Win32 implementation as-is since both are GDK platforms.

### HarnessHash_Xbox.cpp

SHA256 hashing using BCrypt APIs. The existing `HarnessHash_Win32.cpp` uses `BCryptOpenAlgorithmProvider`, `BCryptHashData`, `BCryptFinishHash`, and `CreateFileW/ReadFile` for file I/O. All of these APIs are available on Xbox console via the GDK.

**The Xbox version can be an exact copy of `HarnessHash_Win32.cpp`.** No platform-specific changes are needed — BCrypt and file I/O APIs are identical across GDK platforms.

**Refactoring opportunity:** Since the implementation is identical, consider moving `HarnessHash.cpp` (without platform suffix) into `GameTestAppShared/` instead of duplicating it. Both projects would compile the same file. This is recommended.

---

## ExitGame Function

The shared code and `pch.h` declare `extern void ExitGame() noexcept;`. Each platform provides the implementation:

**Xbox (`Main_Xbox.cpp` or `DeviceApplication_Xbox.cpp`):**
```cpp
void ExitGame() noexcept
{
    PostQuitMessage(0);
}
```

This is the same as the Win32 version since both use the Windows message loop.

---

## XUser Authentication on Xbox Console

The existing shared code in `XUserHandlers.cpp` already handles Xbox user sign-in via `XUserAddAsync`. On Xbox console:

- `XUserAddAsync` with `XUserAddOptions::AddDefaultUserSilently` performs silent sign-in for the default user (already signed in on the console)
- No interactive sign-in UI is needed for automated testing on dev kits
- The dev kit must have a test account signed in before the test device app is launched

The test controller sends the `SignInXbox` command, which calls `Sample_Xbox_AttemptSignIn()` defined in the shared `XUserHandlers.cpp`. This should work without changes on Xbox console.

**Verification needed:** Confirm that `XUserAddOptions::AddDefaultUserSilently` works reliably on Xbox dev kits in automated scenarios. If the dev kit has no user signed in, the command should return an appropriate error that the test controller can report.

---

## WebSocket Connectivity on Xbox Console

The WebSocket client uses libHttpClient (`HCWebSocketCreate`, `HCWebSocketConnectAsync`, etc.) which is available on all GDK platforms. The test device connects to the test controller at `ws://<controller-ip>:15080/ws/`.

### Dev Kit Networking Requirements

- The Xbox dev kit and the test controller PC must be on the same network
- The dev kit must allow outbound WebSocket connections on port 15080
- Xbox dev kits in developer mode have relaxed network restrictions; WebSocket connections to local network addresses work without additional configuration
- The controller IP address is passed via command line argument (`-ip <address>`)

### Command Line on Xbox Console

On Xbox console, command line arguments are configured via the Xbox Device Portal or deployment configuration. The test controller IP must be specified:

```
-ip 192.168.1.100 -deviceId xbox-device-01
```

Options for passing command-line arguments to Xbox console apps:
1. **Xbox Device Portal** → Apps → click the running app → Set command line args
2. **`xbrun`** from a dev PC: `xbrun /O /X:<console-ip> GameTestAppXbox.exe -ip 192.168.1.100`
3. **MicrosoftGameConfig.mgc** → `<AdvancedArgs>` element (not recommended for variable values)

---

## Build and Deployment

### Building

1. Open `PlayFab.C.vs2022.sln` (or add the new project to `PlayFabGameSave.C.GDK.vs2022.sln`)
2. Select the desired platform/configuration:
   - `Debug|Gaming.Xbox.Scarlett.x64` for Xbox Series X|S dev kit
   - `Debug|Gaming.Xbox.XboxOne.x64` for Xbox One dev kit
   - `Debug|Gaming.Desktop.x64` for GDK PC
3. Build the `GameTestAppXbox` project

### Deploying to Xbox Dev Kit

**Via Visual Studio:**
1. Set the Xbox dev kit as the remote machine in project properties → Debugging → Remote Machine
2. F5 or Deploy from the Build menu

**Via command line:**
```cmd
# Package the app (creates an MSIXVC package)
makepkg pack /f layout.xml /d <output-dir> /pd <package-dir>

# Deploy to dev kit
xbapp install <package-path> /r <console-ip>

# Or use loose-file deployment for development
xbapp deploy <build-output-dir> /r <console-ip>
```

### Running

**On Xbox dev kit:**
```cmd
# Launch via xbrun
xbrun /O /X:<console-ip> /A "GameTestAppXbox.exe -ip <controller-ip> -deviceId xbox-series-x-01"

# Or launch via Xbox Device Portal web UI
```

**On GDK PC (same as Windows test device):**
```cmd
Out\Gaming.Desktop.x64\Debug\GameTestAppXbox\GameTestAppXbox.exe -ip localhost -deviceId gdk-pc-01
```

---

## Implementation Checklist

### Phase 1: Extract Shared Code

- [ ] Create `Test/GameTestAppShared/` directory
- [ ] Move all files from `Test/GameTestAppWindows/Common/` to `Test/GameTestAppShared/`
- [ ] Move `HarnessHash_Win32.cpp` to `Test/GameTestAppShared/HarnessHash.cpp` (cross-platform BCrypt — no platform-specific code)
- [ ] Remove `HarnessHash_Win32.cpp` from `GameTestAppWindows/Platform/Windows/`
- [ ] Update `GameTestAppWindows.vcxproj`:
  - Change all `Common\*.cpp` / `Common\*.h` references to `..\GameTestAppShared\*.cpp` / `..\GameTestAppShared\*.h`
  - Add `..\GameTestAppShared\` to `AdditionalIncludeDirectories`
  - Remove old `Common\` include path references
- [ ] Verify Windows project builds and behaves identically

### Phase 2: Create Xbox Project Structure

- [ ] Create `Test/GameTestAppXbox/` directory
- [ ] Create `Test/GameTestAppXbox/Platform/Xbox/` directory
- [ ] Create `GameTestAppXbox.vcxproj` with all nine platform/configuration combinations
- [ ] Create `MicrosoftGameConfig.mgc` (same title identity as Windows test device)
- [ ] Copy `Assets/` folder from Windows test device (or from Xbox console sample)

### Phase 3: Implement Platform Files

- [ ] Create `pch_Xbox.h` / `pch_Xbox.cpp` (based on Xbox console sample's `pch.h` + PlayFab headers)
- [ ] Create `Main_Xbox.cpp` with PLM handling, D3D12 init, message loop (based on Xbox console sample's `Main.cpp`)
- [ ] Create `TestDeviceApp` class (`.h` / `.cpp`) with ATG DeviceResources, TextConsole, LiveResources, LiveInfoHUD, GamePad, StepTimer
- [ ] Copy `DeviceResources.cpp/h` and `StepTimer.h` from Xbox console sample
- [ ] Create `DeviceLogging_Xbox.cpp` (ATG TextConsole-based rendering)
- [ ] Create `DeviceGameSaveState_Xbox.cpp`
- [ ] Implement `ExitGame()` in the Xbox platform files
- [ ] Add ATG Kit source files to the `.vcxproj` (TextConsole, SampleGUI, LiveResources, LiveInfoHUD, StringUtil, ATGTelemetry)

### Phase 4: Build Verification

- [ ] Build for `Gaming.Desktop.x64` — verify all shared code compiles
- [ ] Build for `Gaming.Xbox.Scarlett.x64` — verify Xbox console compilation
- [ ] Build for `Gaming.Xbox.XboxOne.x64` — verify Xbox One compilation
- [ ] Deploy to Xbox Series X|S dev kit and verify launch
- [ ] Deploy to Xbox One dev kit and verify launch (if available)

### Phase 5: Integration Testing

- [ ] Verify WebSocket connection to test controller from Xbox dev kit
- [ ] Run a simple smoke test scenario (e.g., `SignInXbox` → `PFGameSaveFilesInitialize` → `PFGameSaveFilesUploadFiles`)
- [ ] Verify file logging works on Xbox dev kit (check PLS path)
- [ ] Verify `DetectSampleDeviceEngineType()` returns `Xbox` on console
- [ ] Run the full automated test suite from the test controller targeting the Xbox device

### Phase 6: Solution Integration

- [ ] Add `GameTestAppXbox.vcxproj` to `PlayFabGameSave.C.GDK.vs2022.sln`
- [ ] Update `5-linux-port-guide.md` to reference the shared code location in `GameTestAppShared/`

---

## Risk Assessment

| Risk | Mitigation |
|------|------------|
| Shared code uses Win32 APIs not available on Xbox | All shared code is already cross-platform; `#ifdef _WIN32` guards are in place. BCrypt and file I/O APIs are available in the GDK. |
| `std::filesystem` not fully supported on Xbox GDK | The GDK supports `<filesystem>`. If issues arise, replace with `CreateFileW`/`FindFirstFileW` equivalents. |
| WebSocket connection fails on Xbox dev kit | Dev kits in developer mode allow local network connections. Verify firewall settings on the test controller PC. |
| PLM suspend causes WebSocket disconnection | Expected behavior — the auto-reconnect logic in `PumpWebSocketAutoConnect` handles this. The test controller should tolerate brief disconnections. |
| Registry APIs in `DeviceGameSaveState` fail on Xbox | These are only called on the Desktop code path; `IsRunningOnXboxConsole()` returns `true` first on console, bypassing registry checks. |
| `_vscprintf` not available on Xbox console | `_vscprintf` is available in the GDK CRT. If not, use `vsnprintf(nullptr, 0, ...)` as a portable alternative. |
| nlohmann/json dependency | Already provided via `$(NLohmannIncludeDir)` from the build props, available on all GDK platforms. |

---

## Open Questions — Resolved

1. **HarnessHash: shared or duplicated?** → **Shared.** Move to `GameTestAppShared/HarnessHash.cpp`. The BCrypt implementation is identical across all GDK platforms.

2. **Common logging logic: extracted or duplicated?** → **Duplicated.** Each platform's `DeviceLogging_*.cpp` is self-contained. Simpler structure, easier to read in isolation.

3. **Solution file?** → **`PlayFabGameSave.C.GDK.vs2022.sln`** — single solution covers both Windows and Xbox test devices.

4. **ADO pipeline integration?** → **Not now.** Manual builds only for the initial implementation. Pipeline integration can be added later.

5. **Test controller changes?** → The device identifies its platform type in the WebSocket capabilities message. The controller already handles multiple device types; verify the `engineType` field is correctly set to `"Xbox"` and no additional controller changes are needed.

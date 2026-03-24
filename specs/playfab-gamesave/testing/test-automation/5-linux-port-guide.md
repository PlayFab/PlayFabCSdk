# PFGameSaveTestDevice Port Guide (Linux or Other 3pp Console)

This guide describes how to port `PFGameSaveTestDeviceWindows` to Linux or other 3pp console, building on top of the existing `PlayFabServicesManualTestApp` port that your team has already completed.

## Overview

### What is PFGameSaveTestDeviceWindows?

`PFGameSaveTestDeviceWindows` is an automated test device application that executes commands sent from a remote test controller via WebSocket. Unlike the interactive `PlayFabServicesManualTestApp`, this application operates in a fully automated mode where:

1. A **Test Controller** (C# WinForms application running on a PC) sends JSON commands over WebSocket
2. The **Device Application** receives commands, executes them, and returns results
3. This enables automated test scenarios to run across multiple devices without manual intervention

### Key Differences from PlayFabServicesManualTestApp

| Aspect | PlayFabServicesManualTestApp | PFGameSaveTestDeviceWindows |
|--------|------------------------------|------------------------------|
| User Interaction | Keyboard-driven menu | No user interaction (automated) |
| Communication | None (standalone) | WebSocket to test controller |
| Test Control | User presses keys | External controller sends commands |
| Execution Model | Interactive test steps | Command-response loop |
| Focus | General PlayFab Services | GameSave-specific testing |

## Architecture Overview

### Application Data Flow

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           Test Controller (PC)                              │
│                         (C# WinForms Application)                           │
│                                                                             │
│  ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐         │
│  │ Scenario YAML   │───>│ Scenario Engine │───>│ WebSocket Server│         │
│  │ (Test Scripts)  │    │ (Step Executor) │    │ (Port 5000)     │         │
│  └─────────────────┘    └─────────────────┘    └────────┬────────┘         │
└─────────────────────────────────────────────────────────┼──────────────────┘
                                                          │ JSON Commands
                                                          │ JSON Results
                                                          ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                        Device Application (Console/PC)                      │
│                                                                             │
│  ┌────────────────┐     ┌─────────────────┐     ┌────────────────────────┐ │
│  │ WebSocket      │────>│ Command         │────>│ Command Handlers       │ │
│  │ Client         │     │ Processor       │     │ (PF*, X*, Harness)     │ │
│  └────────────────┘     └─────────────────┘     └───────────┬────────────┘ │
│         ▲                                                    │              │
│         │              ┌─────────────────┐                   │              │
│         └──────────────│ Action Result   │<──────────────────┘              │
│                        │ (JSON Response) │                                  │
│                        └─────────────────┘                                  │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Command-Response Loop

1. Device connects to controller WebSocket (`ws://<controller-ip>:5000/ws/`)
2. Device sends capabilities message identifying platform type
3. Controller assigns device a name and role
4. Controller sends command envelope (JSON):
   ```json
   {
     "commandId": "step-001",
     "command": "PFGameSaveFilesInitialize",
     "timeoutSeconds": 30,
     "parameters": { ... },
     "scenarioContext": { "scenarioId": "test-01", "stepIndex": 5 }
   }
   ```
5. Device executes command via handler function
6. Device returns result (JSON):
   ```json
   {
     "type": "actionResult",
     "commandId": "step-001",
     "command": "PFGameSaveFilesInitialize",
     "deviceId": "psx-device-01",
     "status": "succeeded",
     "hresult": 0,
     "elapsedMs": 125
   }
   ```
7. Repeat for each scenario step

## Code Organization

### Directory Structure

```
Test/PFGameSaveTestDeviceWindows/
├── Common/                          # Cross-platform code (reuse as-is)
│   ├── DeviceApplication.h          # Application lifecycle interface
│   ├── DeviceCommandHandlers.cpp/h  # Command router/registry
│   ├── DeviceCommandProcessor.cpp/h # WebSocket message handler
│   ├── DeviceGameSave.cpp/h         # Argument parsing, cleanup
│   ├── DeviceGameSaveState.h        # Shared state structure
│   ├── DeviceLogging.h              # Logging interface (platform-specific impl)
│   ├── DeviceWebSocketClient.cpp/h  # WebSocket wrapper (libHttpClient)
│   ├── DeviceWebSocketConnection.cpp/h # Auto-reconnect logic
│   │
│   ├── PFCommandHandlers.cpp/h      # PlayFab API command handlers
│   ├── XUserHandlers.cpp/h          # Xbox user sign-in handlers
│   ├── XTaskQueueHandlers.cpp/h     # Task queue handlers
│   ├── XGameRuntimeHandlers.cpp/h   # XGameRuntime handlers
│   │
│   ├── ChaosModeHandler.cpp/h       # Random file operation testing
│   ├── WriteGameSaveData.cpp/h      # Test data generation
│   ├── GatherSnapshot.cpp/h         # Save folder archiving
│   ├── CaptureSaveContainerSnapshot.cpp/h # File listing/hashing
│   ├── DeleteSaveRoot.cpp/h         # Save folder deletion
│   ├── DeleteLocalFolder.cpp/h      # Arbitrary folder deletion
│   ├── CopyTargetFolderToSaveFolder.cpp/h # Dataset seeding
│   ├── ConsumeDiskSpace.cpp/h       # Disk space manipulation
│   ├── GatherLogs.cpp/h             # Log retrieval
│   ├── GetDebugStats.cpp/h          # Debug statistics
│   ├── HttpMock.cpp/h               # HTTP mocking support
│   ├── HttpMockHandlers.cpp/h       # Mock configuration handlers
│   ├── SmokeCommandHandlers.cpp/h   # Basic connectivity tests
│   └── CommandHandlerShared.h       # Handler utilities
│
└── Platform/
    └── Windows/                     # Windows-specific (create Linux or other 3pp console equivalent)
        ├── Main.cpp                 # Entry point
        ├── pch.cpp/h                # Precompiled header
        ├── DeviceApplication_Win32.cpp  # App lifecycle implementation
        ├── DeviceLogging_Win32.cpp      # Logging implementation
        ├── DeviceGameSaveState_Win32.cpp # State singleton
        ├── DeviceWindow.cpp/h           # GUI window (optional for PSX)
        └── HarnessHash_Win32.cpp        # SHA256 implementation
```

### What to Reuse vs. Implement

| Component | Action | Notes |
|-----------|--------|-------|
| `Common/*` | **Reuse as-is** | All cross-platform code |
| `Platform/Windows/Main.cpp` | **Create Linux or other 3pp console version** | Entry point with platform init |
| `Platform/Windows/DeviceApplication_Win32.cpp` | **Create Linux or other 3pp console version** | Application lifecycle |
| `Platform/Windows/DeviceLogging_Win32.cpp` | **Create Linux or other 3pp console version** | Logging to console/file |
| `Platform/Windows/DeviceGameSaveState_Win32.cpp` | **Create Linux or other 3pp console version** | Global state singleton |
| `Platform/Windows/DeviceWindow.cpp` | **Skip or stub** | GUI not needed on Linux or other 3pp console |
| `Platform/Windows/HarnessHash_Win32.cpp` | **Create Linux or other 3pp console version** | SHA256 hashing |
| `Platform/Windows/pch.cpp/h` | **Create Linux or other 3pp console version** | Platform headers |

## Implementation Steps

### Step 1: Create Platform Directory

Create `Test/PFGameSaveTestDeviceWindows/Platform/<YourPlatform>/` with:

```
Platform/<YourPlatform>/
├── Main_Psx.cpp
├── pch_Psx.cpp
├── pch_Psx.h
├── DeviceApplication_Psx.cpp
├── DeviceLogging_Psx.cpp
├── DeviceGameSaveState_Psx.cpp
└── HarnessHash_Psx.cpp
```

### Step 2: Implement Platform Files

#### Main_Psx.cpp

Entry point that:
1. Initializes platform runtime
2. Creates global `DeviceGameSaveState` instance
3. Initializes WebSocket client
4. Configures WebSocket message handlers
5. Creates task queue
6. Runs main loop calling `PumpWebSocketAutoConnect()`
7. Cleans up on exit

Reference: `Platform/Windows/DeviceApplication_Win32.cpp`

Key functions to call:
```cpp
DeviceGameSaveState* state = GetSampleGameSaveState();
SetSampleDeviceEngineType(DeviceEngineType::Psx);
state->websocketClient.Initialize();
Sample_GameSave_ConfigureWebSocketLogging(state);  // Sets up command handler
XTaskQueueCreate(..., &state->taskQueue);

// Main loop
while (!state->quit)
{
    // Platform event processing
    PumpWebSocketAutoConnect(state);
}

Sample_GameSave_Cleanup(state);
```

#### DeviceApplication_Psx.cpp

Implement the lifecycle functions declared in `DeviceApplication.h`:
- `DeviceApplicationInitialize()` - Platform-specific setup
- `DeviceApplicationRunLoop()` - Event pump
- `DeviceApplicationShutdown()` - Cleanup

#### DeviceLogging_Psx.cpp

Implement logging functions from `DeviceLogging.h`:

```cpp
void LogToWindow(const std::string& text);           // Output to debug console
void LogToWindowFormat(const char* format, ...);     // Printf-style logging
void LogToWindowVerbose(const std::string& text);    // Verbose-only logging
void EnableFileLoggingForDevice(const std::string& deviceName);  // Start file logging
void CloseLogFile();                                 // Flush and close log
void UpdateWindowTitleWithDeviceName(const std::string& deviceName);  // No-op on PSX
void DeviceLoggingInitializePlatform(void* platformContext);  // Platform init
void DeviceLoggingPaintPlatform(void* platformContext);       // No-op on PSX
void DeviceLoggingHandleDestroy();                   // Cleanup
```

#### DeviceGameSaveState_Psx.cpp

Implement the global state singleton:

```cpp
static DeviceGameSaveState s_gameSaveState{};

DeviceGameSaveState* GetSampleGameSaveState()
{
    return &s_gameSaveState;
}

void SetSampleDeviceEngineType(DeviceEngineType engineType)
{
    s_gameSaveState.engineType = engineType;
}

DeviceEngineType DetectSampleDeviceEngineType()
{
    return DeviceEngineType::Psx;  // Linux or other 3pp console engine type
}
```

#### HarnessHash_Psx.cpp

Implement SHA256 hashing (used for file verification):

```cpp
// From HarnessHash.h
std::string ComputeSHA256ForFile(const std::string& filePath);
std::string ComputeSHA256ForBuffer(const void* data, size_t length);
```

Use platform crypto APIs or a portable library like mbedtls/OpenSSL.

### Step 3: Update pch.h for Your Platform

The precompiled header needs platform-specific includes. Key items:

```cpp
// Platform detection
#define PF_PLATFORM_YOURPLATFORM 1

// Standard C++ (keep these)
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// Platform APIs (replace Windows.h)
// Add your platform headers here

// PlayFab SDK headers (keep these)
#include <playfab/core/PFCore.h>
#include <playfab/core/PFLocalUser.h>
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/gamesave/PFGameSaveFilesUi.h>

// libHttpClient (keep - provides WebSocket)
#include <httpClient/httpClient.h>

// JSON library (keep)
#include <nlohmann/json.hpp>
```

### Step 4: Handle Platform-Specific Command Handlers

Some commands are Windows/Xbox-specific. For Linux or other 3pp console:

| Command | Action |
|---------|--------|
| `XGameRuntimeInitialize` | Return success immediately (no-op) |
| `XUserAddAsync` | Implement platform-specific user sign-in |
| `XUserCloseHandle` | Implement platform-specific user cleanup |
| `XTaskQueueCreate` | May need platform-specific implementation |
| `XblInitialize` | Return success (Xbox Live not applicable) |
| `XblContextCreateHandle` | Return success (not applicable) |

The command handlers in `XUserHandlers.cpp` and `XGameRuntimeHandlers.cpp` may need platform-specific implementations. You can either:
1. Create platform-specific versions of these files
2. Use `#ifdef` guards within the existing files
3. Return success for inapplicable commands

### Step 5: WebSocket Connectivity

The WebSocket client (`DeviceWebSocketClient`) uses libHttpClient which should work on Linux or other 3pp console if your `PlayFabServicesManualTestApp` port already has it working.

Key configuration:
- Default controller address: `localhost` (override with `/controller <ip>` argument)
- WebSocket URI: `ws://<controller-ip>:5000/ws/`
- Reconnect interval: 2 seconds

The device announces its platform type on connect:
```json
{"type":"capabilities","engine":"psx"}
```

### Step 6: Command-Line Arguments

The device supports command-line arguments (see `DeviceGameSave.cpp`):

| Argument | Purpose |
|----------|---------|
| `/controller <ip>` | Controller IP address (default: localhost) |
| `/v` | Enable verbose logging |
| `/p <path>` | Save folder path override |

Parse these appropriately for your platform (may be config file instead of command line).

## Calling Into Common Files - Complete API Reference

This section provides the complete reference for how your platform-specific code calls into the common files. The common code handles all the command processing logic - your platform code just needs to initialize and pump the system.

### Required Headers to Include

Your main platform file should include these headers from `Common/`:

```cpp
#include "Common/DeviceApplication.h"        // Lifecycle interface
#include "Common/DeviceCommandProcessor.h"   // WebSocket message setup
#include "Common/DeviceGameSave.h"           // Cleanup and arg parsing
#include "Common/DeviceGameSaveState.h"      // Global state structure
#include "Common/DeviceLogging.h"            // Logging (you implement)
#include "Common/DeviceWebSocketConnection.h" // Auto-reconnect pump
```

### Essential APIs You Must Call

#### 1. State Management (DeviceGameSaveState.h)

These functions are **declared** in the common header but **implemented** by your platform code:

```cpp
// Get the global state singleton - YOU IMPLEMENT THIS
DeviceGameSaveState* GetSampleGameSaveState();

// Set the engine type - YOU IMPLEMENT THIS
void SetSampleDeviceEngineType(DeviceEngineType engineType);

// Detect platform type - YOU IMPLEMENT THIS
DeviceEngineType DetectSampleDeviceEngineType();
```

#### 2. WebSocket Setup (DeviceCommandProcessor.h)

This is the **key function** that wires up command handling:

```cpp
// Call this AFTER initializing the WebSocket client
// This sets up handlers that parse incoming JSON commands and route to handlers
void Sample_GameSave_ConfigureWebSocketLogging(DeviceGameSaveState* state);
```

**What it does:**
- Sets `state->websocketClient.SetTextMessageHandler(...)` to parse incoming commands
- Routes commands to `BuildActionResult()` which dispatches to individual handlers
- Sends JSON results back over WebSocket
- Handles device assignment messages from controller

#### 3. WebSocket Auto-Reconnect (DeviceWebSocketConnection.h)

Call this repeatedly in your main loop:

```cpp
// Pumps WebSocket reconnection logic - call every frame/iteration
void PumpWebSocketAutoConnect(DeviceGameSaveState* state);
```

**What it does:**
- Checks if WebSocket is disconnected
- Waits for reconnect interval (2 seconds)
- Attempts reconnection to controller
- Sends capabilities message on connect

#### 4. Cleanup (DeviceGameSave.h)

Call this when shutting down:

```cpp
// Cleans up PlayFab, WebSocket, task queue, and user handles
HRESULT Sample_GameSave_Cleanup(DeviceGameSaveState* state);
```

**What it does:**
- Disconnects WebSocket
- Closes PFLocalUserHandle
- Closes PFServiceConfigHandle  
- Uninitializes PFGameSaveFiles, PFServices, PF
- Closes task queue
- Closes XUser handle

#### 5. Argument Parsing (DeviceGameSave.h)

Optional - parse command line or config:

```cpp
// Parses command-line arguments into state
bool Sample_GameSave_ParseArgs(const char* cmdLine, DeviceGameSaveState* state);
```

### Complete Main Function Template

Here's the complete template showing all API calls in order:

```cpp
#include "pch.h"
#include "Common/DeviceApplication.h"
#include "Common/DeviceCommandProcessor.h"
#include "Common/DeviceGameSave.h"
#include "Common/DeviceGameSaveState.h"
#include "Common/DeviceLogging.h"
#include "Common/DeviceWebSocketConnection.h"

int main(int argc, char* argv[])
{
    //==========================================================================
    // PHASE 1: Platform Initialization
    //==========================================================================
    
    // Initialize platform-specific logging
    InitializeHCTraceToVerboseLog();  // Sets up libHttpClient tracing
    
    LogToWindow("PlayFab Game Save Test Device starting...");
    
    //==========================================================================
    // PHASE 2: Get State and Set Engine Type
    //==========================================================================
    
    // Get the global state singleton (your implementation)
    DeviceGameSaveState* state = GetSampleGameSaveState();
    
    // Set engine type to your platform (your implementation)
    SetSampleDeviceEngineType(DeviceEngineType::Psx);
    
    //==========================================================================
    // PHASE 3: Initialize WebSocket Client
    //==========================================================================
    
    // Initialize the WebSocket client (common code, uses libHttpClient)
    HRESULT wsHr = state->websocketClient.Initialize();
    LogToWindowFormat("WebSocket client init (hr=0x%08X)", static_cast<uint32_t>(wsHr));
    if (FAILED(wsHr))
    {
        LogToWindow("Failed to initialize WebSocket client");
        return 1;
    }
    
    //==========================================================================
    // PHASE 4: Configure Command Handlers (CRITICAL!)
    //==========================================================================
    
    // This wires up the WebSocket to receive commands and dispatch to handlers
    // WITHOUT THIS CALL, the device will not respond to any commands!
    Sample_GameSave_ConfigureWebSocketLogging(state);
    
    //==========================================================================
    // PHASE 5: Parse Arguments (Optional)
    //==========================================================================
    
    // Build command line string from argv or use config file
    std::string cmdLine = BuildCommandLineFromArgs(argc, argv);  // Your implementation
    if (!Sample_GameSave_ParseArgs(cmdLine.c_str(), state))
    {
        LogToWindow("Failed to parse arguments");
        return 1;
    }
    
    //==========================================================================
    // PHASE 6: Create Task Queue
    //==========================================================================
    
    // Task queue is needed for async PlayFab operations
    HRESULT queueHr = XTaskQueueCreate(
        XTaskQueueDispatchMode::ThreadPool,
        XTaskQueueDispatchMode::ThreadPool,
        &state->taskQueue);
    
    if (FAILED(queueHr))
    {
        LogToWindow("Failed to create task queue");
        return 1;
    }
    
    // Make it the process default queue
    XTaskQueueSetCurrentProcessTaskQueue(state->taskQueue);
    
    //==========================================================================
    // PHASE 7: Main Loop
    //==========================================================================
    
    LogToWindow("Entering main loop - waiting for controller connection...");
    
    while (!state->quit)
    {
        // Platform-specific event processing (controller input, system events)
        ProcessPlatformEvents();  // Your implementation
        
        // Pump WebSocket auto-reconnect - THIS IS CRITICAL
        // This maintains the connection to the controller and enables command flow
        PumpWebSocketAutoConnect(state);
        
        // Small sleep to prevent spinning
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    //==========================================================================
    // PHASE 8: Cleanup
    //==========================================================================
    
    LogToWindow("Shutting down...");
    
    // Clean up all PlayFab and WebSocket resources
    Sample_GameSave_Cleanup(state);
    
    // Close task queue
    if (state->taskQueue)
    {
        XTaskQueueCloseHandle(state->taskQueue);
        state->taskQueue = nullptr;
    }
    
    return 0;
}
```

### How Command Flow Works Internally

When `Sample_GameSave_ConfigureWebSocketLogging()` is called, it sets up this flow:

```
Controller sends JSON command via WebSocket
                    │
                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│ DeviceWebSocketClient.m_textHandler (set by ConfigureWebSocketLogging)  │
└─────────────────────────────────────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│ TryParseCommandEnvelope() - parses JSON into CommandEnvelopeView        │
│   - commandId, command, parameters, scenarioContext                     │
└─────────────────────────────────────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│ BuildActionResult() in DeviceCommandHandlers.cpp                        │
│   - Looks up command in s_handlers map                                  │
│   - Calls appropriate Handle* function                                  │
└─────────────────────────────────────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│ Handle*() function (e.g., HandlePFGameSaveFilesInitialize)              │
│   - Executes the actual PlayFab API call                                │
│   - Returns CommandResultPayload with status and hresult                │
└─────────────────────────────────────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│ state->websocketClient.SendText(payload.result.dump())                  │
│   - Sends JSON result back to controller                                │
└─────────────────────────────────────────────────────────────────────────┘
```

### Logging Functions You Must Implement

These are declared in `DeviceLogging.h` and called throughout the common code:

```cpp
// PRIMARY LOGGING - called constantly, must work reliably
void LogToWindow(const std::string& text);
void LogToWindowFormat(const char* format, ...);

// VERBOSE LOGGING - only logs when verboseLogs is enabled
void LogToWindowVerbose(const std::string& text);
void LogToWindowFormatVerbose(bool verboseOnly, const char* format, ...);

// FILE LOGGING - logs to file per-device
void EnableFileLoggingForDevice(const std::string& deviceName);
void CloseLogFile();
bool TryGetCurrentLogFilePath(std::string& pathOut);
std::string ResolveLogFilePathForDevice(const std::string& deviceName);

// TRACE SETUP - configures libHttpClient tracing
void InitializeHCTraceToVerboseLog();
void FlushDeviceLogFile();

// UI FUNCTIONS - can be no-ops on Linux or other 3pp console
void UpdateWindowTitleWithDeviceName(const std::string& deviceName);  // No-op
void DeviceLoggingInitializePlatform(void* platformContext);          // Platform init
void DeviceLoggingPaintPlatform(void* platformContext);               // No-op
void DeviceLoggingHandleDestroy();                                    // Cleanup
```

### Hash Functions You Must Implement

These are declared in `HarnessHash.h` and used for file verification:

```cpp
// Compute SHA256 hash of a file on disk
std::string ComputeSHA256ForFile(const std::string& filePath);

// Compute SHA256 hash of a memory buffer
std::string ComputeSHA256ForBuffer(const void* data, size_t length);
```

Both should return lowercase hex strings (64 characters for SHA256).

### State Fields Your Platform Code May Need to Access

Key fields in `DeviceGameSaveState` that your platform code interacts with:

```cpp
struct DeviceGameSaveState
{
    // WebSocket client - initialized by common code after you call Initialize()
    DeviceWebSocketClient websocketClient{};
    
    // Task queue - YOU create this, common code uses it
    XTaskQueueHandle taskQueue{ nullptr };
    
    // Controller IP - set by arg parsing or your config
    std::string controllerIpAddress{ "localhost" };
    
    // Engine type - YOU set this via SetSampleDeviceEngineType()
    DeviceEngineType engineType{ DeviceEngineType::PcGrts };
    
    // Quit flag - set to true to exit main loop
    bool quit{ false };
    
    // Verbose logging flag - set by /v argument
    bool verboseLogs{ false };
    
    // Device ID assigned by controller
    std::string inputDeviceId;
    
    // Save folder path - set by PFGameSaveFilesGetFolder command
    std::string saveFolder;
    
    // WebSocket connection state (managed by common code)
    std::atomic<bool> websocketConnectInProgress{ false };
    std::chrono::steady_clock::time_point websocketLastAttempt{};
};
```

## Key Components to Understand

### DeviceGameSaveState

Central state structure holding:
- PlayFab handles (`serviceConfigHandle`, `localUserHandle`)
- Task queue (`taskQueue`)
- WebSocket client (`websocketClient`)
- Current scenario info (`currentScenarioId`, `currentScenarioName`)
- Auto-response configuration for UI callbacks
- Various flags and configuration options

### Command Handler Pattern

All commands follow this pattern:

```cpp
CommandResultPayload HandleSomeCommand(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);
    
    auto start = std::chrono::steady_clock::now();
    
    // Execute the actual work...
    HRESULT hr = DoSomething(state, parameters);
    
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
    
    if (SUCCEEDED(hr))
    {
        CommandHandlerShared::MarkSuccess(payload.result);
    }
    else
    {
        CommandHandlerShared::MarkFailure(payload.result, hr, "Error message");
    }
    
    return payload;
}
```

### Command Registration

Commands are registered in `DeviceCommandHandlers.cpp` in a static map:

```cpp
static const std::unordered_map<std::string_view, CommandHandler> s_handlers{
    { "PFInitialize", HandlePFInitialize },
    { "PFGameSaveFilesInitialize", HandlePFGameSaveFilesInitialize },
    // ... etc
};
```

### WebSocket Message Flow

1. `DeviceWebSocketConnection.cpp` - `PumpWebSocketAutoConnect()` handles reconnection
2. `DeviceWebSocketClient.cpp` - Raw WebSocket send/receive
3. `DeviceCommandProcessor.cpp` - Parses JSON, routes to handlers, sends results

## Testing the Port

### Basic Connectivity Test

1. Run the test controller on a PC
2. Run the device application on your target platform
3. Verify WebSocket connection established
4. Send `SmokeCommandEcho` command - should echo parameters back

### Command Execution Test

1. Run a simple scenario that calls:
   - `PFInitialize`
   - `PFServicesInitialize`
   - `PFServiceConfigCreateHandle`
   - `PFGameSaveFilesInitialize`
2. Verify all commands return `succeeded` status

### Full Scenario Test

1. Run a complete GameSave test scenario
2. Verify uploads and downloads work
3. Check snapshot capture and comparison

## Troubleshooting

### WebSocket Connection Issues

- Ensure firewall allows port 5000
- Check controller IP is reachable from device network
- Verify libHttpClient is properly initialized

### Command Timeouts

- Default timeout is in the command envelope (`timeoutSeconds`)
- Long operations (upload/download) may need extended timeouts
- Check task queue is properly created and dispatching

### File Operations

- Many commands operate on the save folder (`state->saveFolder`)
- Ensure filesystem APIs work correctly on your target platform
- Check file path separators (use `std::filesystem` where possible)

## Summary

The port primarily involves:

1. **Creating platform files** in `Platform/<YourPlatform>/`
2. **Implementing logging** appropriate for your platform
3. **Handling user sign-in** using platform-specific identity APIs
4. **Stubbing Xbox-specific commands** that don't apply

The bulk of the test automation logic in `Common/` should work unchanged since it uses:
- Standard C++ and STL
- libHttpClient for WebSocket (already ported for PlayFabServicesManualTestApp)
- nlohmann/json for JSON parsing
- PlayFab C SDK APIs

The device is "dumb" - it just executes commands and returns results. All test intelligence lives in the controller, making the device port relatively straightforward.

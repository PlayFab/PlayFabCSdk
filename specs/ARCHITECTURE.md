# System Architecture

## 1. High-Level Overview

PlayFab C SDK is a native C/C++ SDK that enables game developers to integrate PlayFab backend services — authentication, catalog, inventory, leaderboards, statistics, groups, cloud scripts, push notifications, game saves, and more — into titles shipping on Microsoft GDK (PC and Console), Win32, Linux, iOS, macOS, and Android. Private platform support includes PlayStation and Nintendo Switch.

The primary consumers are game studios building titles that use PlayFab as their game backend. The SDK exposes a **flat C API** (`PF` prefix) for maximum portability and ABI stability. All I/O is asynchronous, driven by the XAsync/XTaskQueue pattern from libHttpClient. The libraries are compiled as static libraries (GDK, Win32) or shared libraries (Linux, Android, iOS/macOS) and linked into game executables.

State is centralized in a process-wide global state singleton (`PFCoreGlobalState`), while per-title configuration is scoped through `PFServiceConfigHandle`. Authenticated user sessions are represented by `PFEntityHandle` objects obtained through login. Authentication supports multiple identity providers: Xbox Live (XUser), Steam, and custom ID.

The SDK consists of three major libraries:
- **PlayFabCore** — Initialization, authentication, entity management, HTTP client, event pipeline, tracing
- **PlayFabServices** — Service wrappers for all PlayFab APIs (catalog, inventory, leaderboards, etc.)
- **PlayFabGameSave** — Cross-platform game save with sync, conflict resolution, and active device management

Service wrappers in PlayFabServices are **partially auto-generated** from PlayFab API specs via the SDKGenerator tool.

## 2. System Boundaries

### External Services

| Boundary | Protocol | Description |
|----------|----------|-------------|
| PlayFab REST APIs | HTTPS | All service calls: authentication, catalog, inventory, leaderboards, statistics, groups, player data, cloud script, push notifications, etc. Endpoint URLs are per-title (e.g., `https://ABCDEF.playfabapi.com`). |
| PlayFab Game Save Service | HTTPS | Manifest management, compressed save data upload/download, active device locking, quota management. |
| PlayFab Event Pipeline | HTTPS | Telemetry event ingestion (PlayStream and Telemetry events). Batched upload via `PFEventPipeline`. |
| Xbox Live (XAL/XUser) | In-process (GDK) | User identity and authentication on Xbox/GDK platforms. Token acquisition delegated to platform XUser APIs. |
| Steam (Steamworks SDK) | In-process | User identity and authentication on Steam. Auth session tickets obtained via Steamworks API. |
| libHttpClient | In-process | HTTP transport abstraction. Provided by GDK on Xbox/Windows; bundled for other platforms. Handles connection pooling, retry, and platform TLS. |

### Local Resources

| Resource | Description |
|----------|-------------|
| Local file system (all platforms) | Game save data (root folder per user), local sync state, event queue persistence. |
| GDK Game Runtime Services (GRTS) | Out-of-process game save provider on Xbox/Windows (background upload survives app exit). |

## 3. Directory Structure

```
PlayFab.C/
├── Source/                              # SDK implementation
│   ├── PlayFabCore/                     #   Core library
│   │   ├── Include/playfab/core/        #     Public headers (PFCore.h, PFAuthentication, PFLocalUser, etc.)
│   │   └── Source/                      #     Implementation
│   │       ├── Api/                     #       Public API entry points
│   │       ├── Authentication/          #       Login flows, entity token management
│   │       ├── Common/                  #       Global state, entity, HTTP client, service config
│   │       ├── EventPipeline/           #       Telemetry batching and upload
│   │       ├── Platform/               #       Platform-specific code (Android, Windows, Generic)
│   │       ├── Trace/                  #       Logging infrastructure
│   │       └── Generated/              #       Auto-generated API code
│   │
│   ├── PlayFabServices/                 #   Service wrapper library
│   │   ├── Include/Generated/playfab/services/  # Public headers (PFCatalog.h, PFInventory.h, etc.)
│   │   └── Source/                      #     Implementation
│   │       ├── Api/                     #       Non-generated API helpers
│   │       ├── Common/                  #       Shared service infrastructure
│   │       ├── Generated/              #       Auto-generated service wrappers (one pair per service)
│   │       └── GDK/                    #       GDK-specific service code
│   │
│   ├── PlayFabGameSave/                 #   Game save library
│   │   ├── Include/playfab/gamesave/    #     Public headers (PFGameSaveFiles.h, types, UI)
│   │   └── Source/                      #     Implementation
│   │       ├── Api/                     #       Public API entry points
│   │       ├── Common/                  #       Global state, HTTP, telemetry, UI callback manager
│   │       ├── Platform/               #       Platform-specific providers (GDK/GRTS, Windows)
│   │       ├── Providers/              #       Async providers (upload, download, reset, description)
│   │       ├── SyncManager/            #       Folder sync engine (lock, compare, download, upload steps)
│   │       ├── Types/                  #       Internal type definitions
│   │       ├── Wrappers/              #       Internal wrapper utilities
│   │       └── Generated/             #       Auto-generated code
│   │
│   ├── PlayFabSharedInternal/           #   Shared internal utilities
│   │   └── Include/                     #     Internal headers (JSON, HTTP, compression, async, memory)
│   │
│   ├── BaseGeneratorTemplate/           #   Shared code generation templates
│   ├── PlayFabCoreGeneratorTemplate/    #   Core-specific generator templates
│   ├── PlayFabServicesGeneratorTemplate/#   Services-specific generator templates
│   └── PlayFabGameSaveGeneratorTemplate/#   GameSave-specific generator templates
│
├── External/                            # Third-party dependencies
│   ├── nlohmann/                        #   nlohmann JSON (header-only)
│   ├── libarchive/                      #   Archive/compression library
│   ├── SDKGenerator/                    #   PlayFab API code generator
│   └── steamworks_sdk/                  #   Valve Steamworks SDK
│
├── Test/                                # Test framework and test apps
│   ├── GameTestController/              #   C# test controller (WebSocket server, YAML scenarios)
│   ├── GameTestAppShared/               #   C++ command handlers (shared across platforms)
│   ├── GameTestAppWindows/              #   Windows GDK test device app
│   ├── GameTestAppXbox/                 #   Xbox test device app
│   ├── PlayFabCore.UnitTests/           #   Core unit tests
│   ├── PlayFabGameSaveUnitTests/        #   Game save unit tests
│   ├── PlayFabServices.GeneratedTests/  #   Generated service wrapper tests
│   ├── PFGameSaveTestController/        #   Game save-specific test controller
│   ├── PFGameSaveTestDeviceWindows/     #   Game save test device (Windows)
│   └── PFGameSaveTestDeviceXbox/        #   Game save test device (Xbox)
│
├── Build/                               # MSBuild projects per platform/library
│   ├── PlayFabCore.*/                   #   Core build projects (GDK, Win32, Linux, Android, Apple)
│   ├── PlayFabServices.*/               #   Services build projects
│   ├── PlayFabGameSave.*/               #   Game save build projects
│   ├── PlayFabSharedInternal.*/         #   Shared internal build projects
│   ├── Shared/                          #   Shared MSBuild files
│   └── *.props                          #   Property sheets (paths, imports, platform config)
│
├── Samples/                             # Sample applications
│   ├── PlayFabGameSaveSample-Windows/   #   Game save Windows sample
│   └── PlayFabGameSaveSample-XboxConsole/ # Game save Xbox sample
│
├── Utilities/                           # Scripts and tools
│   ├── Scripts/                         #   Build, test, and maintenance scripts
│   └── GameSave/                        #   Game save-specific utilities
│
├── Pipelines/                           # CI/CD pipeline definitions
├── specs/                               # Architecture specs, design docs, test docs
├── *.sln                                # Visual Studio solutions
├── Directory.Packages.props             # NuGet central package versioning
├── NuGet.config                         # NuGet feed configuration
└── AGENTS.md                            # AI context bootstrap
```

## 4. Core Tech Stack & Entry Points

### Languages & Frameworks

| Technology | Role |
|------------|------|
| C++17 | Primary implementation language |
| C | Public API surface (flat C with `PF` prefix) |
| C# / .NET 8 | Test controller (GameTestController) |
| YAML | Test scenario definitions |
| MSBuild / vcxproj | Primary build system (Windows/GDK) |
| CMake | Build system (Linux, Android, Apple) |
| Gradle | Android build orchestration |
| Xcode | iOS/macOS build orchestration |

### Entry Points

**Library initialization (required before any API call):**
```
PFServicesInitialize(nullptr)  →  PFCoreGlobalState::Create()  →  HCInitialize()
```
- Defined in: `Source/PlayFabCore/Source/Api/`
- Header: `Source/PlayFabCore/Include/playfab/core/PFCore.h`

**Service configuration (connects to a specific PlayFab title):**
```
PFServiceConfigCreateHandle(endpoint, titleId, &handle)
```
- Header: `Source/PlayFabCore/Include/playfab/core/PFServiceConfig.h`

**Authentication (per-user login):**
```
PFAuthenticationLoginWith*Async(serviceConfig, &request, &asyncBlock)
→ PFAuthenticationLoginWith*GetResult(&asyncBlock, &entityHandle, ...)
```
- Headers: `Source/PlayFabCore/Include/playfab/core/PFAuthentication_Xbox.h` (GDK), `Source/PlayFabCore/Include/Generated/` (other methods)

**Service calls (using entity handle):**
```
PF<Service><Verb>Async(entityHandle, &request, &asyncBlock)
PF<Service><Verb>GetResult(&asyncBlock, ...)
```
- Headers: `Source/PlayFabServices/Include/Generated/playfab/services/PF<Service>.h`

**Cleanup:**
```
PFEntityCloseHandle(entityHandle)
PFServiceConfigCloseHandle(serviceConfigHandle)
PFServicesUninitializeAsync(&asyncBlock)
```

### Platform Targets

| Platform | Build System | Output |
|----------|-------------|--------|
| GDK (Gaming.Desktop.x64) | MSBuild | Static lib |
| GDK (Gaming.Xbox.XboxOne.x64) | MSBuild | Static lib |
| GDK (Gaming.Xbox.Scarlett.x64) | MSBuild | Static lib |
| Win32 (x64) | MSBuild | Static lib |
| Linux (x64) | CMake | Shared library (.so) |
| Android (arm64-v8a, x86_64) | Gradle + CMake | Shared library (.so) + AAR |
| iOS (arm64) | Xcode | Framework |
| macOS | Xcode | Framework |

## 5. Data Flow & State Management

### Initialization Flow

```
Game calls PFServicesInitialize(nullptr)
    │
    ├─▶ HCInitialize() — init libHttpClient transport
    ├─▶ PFCoreGlobalState::Create() — allocate singleton
    │       ├─▶ Initialize tracing
    │       └─▶ Initialize platform layer
    │
    ├─▶ PFServiceConfigCreateHandle(endpoint, titleId, &config)
    │       └─▶ Create ServiceConfig with endpoint URL and title ID
    │
    └─▶ Ready for PFAuthenticationLoginWith*Async()
```

### Service Call Data Flow (e.g., Get Catalog Items)

```
1. Game calls PFCatalogSearchItemsAsync(entityHandle, &request, &asyncBlock)
2. Generated wrapper builds HTTP request:
   - URL: https://{endpoint}/Catalog/SearchItems
   - Headers: Content-Type: application/json
   - Body: JSON-serialized request struct (via nlohmann JSON)
3. HttpClient::MakeRequest() invoked:
   a. Entity token injected (X-EntityToken header)
   b. Delegates to HCHttpCallPerformAsync() (libHttpClient)
4. libHttpClient executes HTTPS request, handles retry/timeout
5. Response arrives:
   a. HTTP status + JSON body parsed via nlohmann JSON
   b. Deserialized into typed result structs
   c. XAsyncComplete() signals the async block
6. Game calls PFCatalogSearchItemsGetResult() to retrieve data
```

### Game Save Data Flow

```
Download (AddUser):
  List manifests → Acquire active device lock → Fetch extended manifest
  → Compare vs local state → Detect conflicts → Resolve via UI callbacks
  → Download needed compressed bundles → Expand & update local state

Upload:
  Compare local vs last synced state → Determine changed/deleted files
  → Bundle/compress ≤64MB sets → Reuse unchanged bundles
  → Upload with progress callbacks → Finalize manifest
  → Optionally release active device
```

### State Ownership

| State | Owner | Lifetime |
|-------|-------|----------|
| Process-wide config | `PFCoreGlobalState` (singleton) | `PFServicesInitialize` → `PFServicesUninitializeAsync` |
| Per-title endpoint config | `ServiceConfig` | `CreateHandle` → `CloseHandle` (ref-counted) |
| Per-user auth tokens | `Entity` (wraps entity token) | Login → `PFEntityCloseHandle` (ref-counted) |
| Game save sync state | `GameSaveGlobalState` | `PFGameSaveFilesInitialize` → `PFGameSaveFilesUninitializeAsync` |
| Active device lock | Game Save Service (server-side) | `AddUser` → upload with `ReleaseDeviceAsActive` |
| HTTP transport | libHttpClient | `HCInitialize` → `HCCleanupAsync` |
| Event pipeline | `PFEventPipeline` | Created by game → destroyed by game |

### Source of Truth

- **Authentication state**: Entity tokens from PlayFab service. Tokens refresh automatically on expiry.
- **Service data**: PlayFab backend. The SDK is a stateless client (no local caching of service data).
- **Game save data**: Split — local file system is authoritative for current session; cloud manifests are authoritative for cross-device sync. Conflicts are resolved explicitly by the player.
- **Configuration**: `PFServiceConfigHandle` at init time (endpoint URL + title ID).

## 6. Component Diagram

```
Game Code
    │
    ▼
┌─────────────────────────────────────┐
│  PlayFabServices                    │  ← Generated service wrappers
│  ├─ Catalog                         │
│  ├─ Inventory                       │
│  ├─ Leaderboards                    │
│  ├─ Statistics                      │
│  ├─ Groups                          │
│  ├─ Friends                         │
│  ├─ Profiles                        │
│  ├─ AccountManagement               │
│  ├─ Data                            │
│  ├─ CloudScript                     │
│  ├─ Experimentation                 │
│  ├─ MultiplayerServer               │
│  ├─ PlayerDataManagement            │
│  ├─ PushNotifications               │
│  ├─ Segments                        │
│  ├─ TitleDataManagement             │
│  └─ Localization                    │
├─────────────────────────────────────┤
│  PlayFabCore                        │  ← Auth, entity, HTTP, events
│  ├─ Authentication (Login flows)    │
│  ├─ Entity (token management)       │
│  ├─ ServiceConfig (endpoint/title)  │
│  ├─ HttpClient (→ libHttpClient)    │
│  ├─ EventPipeline (telemetry)       │
│  ├─ LocalUser (Xbox/Steam bridge)   │
│  └─ Trace (logging)                 │
├─────────────────────────────────────┤
│  PlayFabGameSave                    │  ← File-based game saves
│  ├─ SyncManager (lock/compare/      │
│  │   download/upload steps)         │
│  ├─ Providers (async operations)    │
│  ├─ UICallbackManager               │
│  ├─ Platform (GDK/GRTS, Windows)    │
│  └─ Compression (libarchive/zlib)   │
├─────────────────────────────────────┤
│  PlayFabSharedInternal              │  ← Cross-cutting utilities
│  ├─ JSON (nlohmann wrappers)        │
│  ├─ HTTP request helpers            │
│  ├─ Async/XAsync utilities          │
│  ├─ Memory management               │
│  ├─ Result<T> / error types         │
│  └─ Compression helpers             │
├─────────────────────────────────────┤
│  External Dependencies              │
│  ├─ libHttpClient (HTTP + TLS)      │
│  ├─ nlohmann JSON (parsing)         │
│  ├─ libarchive (compression)        │
│  └─ Steamworks SDK (Steam auth)     │
└─────────────────────────────────────┘
         │                    │
         ▼                    ▼
  PlayFab REST APIs    Game Save Service
  (HTTPS)              (HTTPS)
```

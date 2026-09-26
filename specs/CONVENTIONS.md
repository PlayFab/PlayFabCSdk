# Coding Conventions & Patterns

Reference for contributors and AI assistants working in the PlayFab C SDK codebase.

## API Surface Rules

### Single API Layer (Flat C)

Unlike XSAPI's dual C/C++ layers, PlayFab C SDK exposes only a **flat C API**. There is no public C++ wrapper layer.

| Layer | Header Location | Prefix | Audience |
|-------|----------------|--------|----------|
| C API (Core) | `Source/PlayFabCore/Include/playfab/core/` | `PF` | All consumers |
| C API (Services) | `Source/PlayFabServices/Include/Generated/playfab/services/` | `PF` | All consumers |
| C API (Game Save) | `Source/PlayFabGameSave/Include/playfab/gamesave/` | `PFGameSave` | Game save consumers |

**Rule:** All public API is flat C. Never expose C++ types, exceptions, or STL containers across the API boundary.

### C API Naming

```
PF<Service><Verb>[Noun]Async()        — Initiate async operation
PF<Service><Verb>[Noun]GetResultSize() — Get result buffer size (variable-size results)
PF<Service><Verb>[Noun]GetResult()     — Retrieve result
```

- Types: `PF<Type>` (e.g., `PFEntityHandle`, `PFServiceConfigHandle`, `PFCatalogItem`)
- Functions: `PF<Service><Verb>Async` (e.g., `PFCatalogSearchItemsAsync`, `PFAuthenticationLoginWithCustomIDAsync`)
- Enums: `PF<Type>` (e.g., `PFLoginIdentityProvider`, `PFGameSaveUploadOption`)
- Constants/Error codes: `E_PF_*` (e.g., `E_PF_GAMESAVE_NOT_INITIALIZED`)
- Result getters: `PF<Service><Verb>GetResultSize` + `PF<Service><Verb>GetResult`

### Internal Naming

- Internal classes: `PascalCase` (e.g., `PFCoreGlobalState`, `HttpClient`, `FolderSyncManager`)
- Internal methods: `PascalCase` (e.g., `MakeRequest`, `CompareLocalAndRemote`)
- File names: `PascalCase` for classes (e.g., `GameSaveGlobalState.cpp`, `UICallbackManager.h`)
- Generated files: Match the service name (e.g., `Catalog.cpp`, `CatalogTypes.cpp`, `PFCatalog.cpp`)

## Memory Management

### Handle Pattern

All opaque resources use a create/close handle pattern with internal ref-counting:

```c
PFServiceConfigHandle configHandle;
PFServiceConfigCreateHandle(endpoint, titleId, &configHandle);  // Create
// ... use handle ...
PFServiceConfigCloseHandle(configHandle);                       // Close (ref-count decrement)
```

Handles in the SDK:
- `PFServiceConfigHandle` — Per-title endpoint configuration
- `PFEntityHandle` — Authenticated user/entity session
- `PFEventPipelineHandle` — Telemetry event pipeline

### Result Buffers (Count-Then-Fetch)

For variable-size results, the SDK uses a two-step pattern:

```c
// Step 1: Get required buffer size
size_t bufferSize;
PFCatalogSearchItemsGetResultSize(&asyncBlock, &bufferSize);

// Step 2: Allocate and retrieve
std::vector<char> buffer(bufferSize);
PFCatalogSearchItemsResponse* result;
PFCatalogSearchItemsGetResult(&asyncBlock, buffer.size(), buffer.data(), &result, nullptr);
```

The result struct and all its string/array members point into the provided buffer. The buffer must outlive any use of the result pointer.

### Custom Memory Hooks

Games may provide custom allocators. Must be set **before** `PFServicesInitialize`:

```c
PFMemoryHooks hooks{};
hooks.alloc = myAlloc;
hooks.free = myFree;
PFMemSetFunctions(&hooks);
```

## Async Pattern

All I/O operations use the XAsync pattern from libHttpClient.

### Blocking Style (Simple)

```c
XAsyncBlock async{};
PFCatalogSearchItemsAsync(entityHandle, &request, &async);
XAsyncGetStatus(&async, true);  // blocking wait
// ... get result ...
```

### Callback Style (Recommended)

```c
auto async = std::make_unique<XAsyncBlock>();
async->callback = [](XAsyncBlock* async)
{
    std::unique_ptr<XAsyncBlock> asyncBlockPtr{ async };
    // ... get result ...
};

PFCatalogSearchItemsAsync(entityHandle, &request, async.get());
if (SUCCEEDED(hr))
{
    async.release();  // callback owns it now
}
```

### Key Rules

- Never block in callbacks. All callbacks execute on the XTaskQueue work thread.
- Use `std::unique_ptr<XAsyncBlock>` to manage lifetime — release ownership when the async operation starts.
- Capture handles by value (ref-counted), not raw pointers.
- All async APIs return `HRESULT` synchronously for immediate validation errors.

## Error Handling

| Layer | Pattern |
|-------|---------|
| C API | Returns `HRESULT`. `S_OK` = success. Use `SUCCEEDED()` / `FAILED()` macros. |
| HTTP errors | Manifest as failure HRESULTs (e.g., `HTTP_E_STATUS_NOT_FOUND`). |
| PlayFab errors | `E_PF_*` error codes for service-specific failures. |
| Internal | `Result<T>` template (HRESULT + optional payload) in `PlayFabSharedInternal`. |

For service call failures, enable debug tracing to see detailed error messages from the PlayFab service in the debugger output.

### Common Error Macros (Internal)

```cpp
RETURN_HR_IF(E_INVALIDARG, !handle);           // Return early if condition true
RETURN_HR_IF_FAILED(SomeFunction());            // Return early if HRESULT failed
RETURN_IF_FAILED(hr);                           // Propagate failure
```

## Code Generation

### Overview

PlayFab service wrappers are **partially auto-generated** from API specs. The generator produces:
- Public headers (`PF<Service>.h`, `PF<Service>Types.h`)
- Type serialization/deserialization (`<Service>Types.cpp`)
- API entry points (`PF<Service>.cpp`)
- Internal service implementation (`<Service>.cpp`, `<Service>.h`)

### What Is Generated vs. Hand-Written

| Component | Generated? | Location |
|-----------|-----------|----------|
| Service headers (PF*.h) | Yes | `Source/PlayFabServices/Include/Generated/` |
| Service type headers (PF*Types.h) | Yes | `Source/PlayFabServices/Include/Generated/` |
| Service API impl (PF*.cpp) | Yes | `Source/PlayFabServices/Source/Generated/` |
| Service internal impl (*.cpp, *.h) | Yes | `Source/PlayFabServices/Source/Generated/` |
| Core headers | Mixed | `Source/PlayFabCore/Include/` |
| Core implementation | Mostly hand-written | `Source/PlayFabCore/Source/` |
| Game save headers | Hand-written | `Source/PlayFabGameSave/Include/` |
| Game save implementation | Hand-written | `Source/PlayFabGameSave/Source/` |
| Shared internal | Hand-written | `Source/PlayFabSharedInternal/` |

### Generator Templates

Templates live in `*GeneratorTemplate/` directories:
- `BaseGeneratorTemplate/` — Shared templates
- `PlayFabCoreGeneratorTemplate/` — Core-specific templates
- `PlayFabServicesGeneratorTemplate/` — Services-specific templates
- `PlayFabGameSaveGeneratorTemplate/` — Game save-specific templates

### Regenerating

```cmd
Utilities\Scripts\pf-gensdk.cmd
```

**Rule:** Never hand-edit files marked as generated. Edit the generator templates instead and regenerate.

## HTTP Call Pattern

Generated service wrappers follow a consistent pattern:

1. Validate input parameters (return `E_INVALIDARG` immediately if invalid).
2. Serialize request struct to JSON via nlohmann JSON.
3. Build HTTP request (method, URL, headers, body).
4. Inject entity token (`X-EntityToken` header).
5. Delegate to `HCHttpCallPerformAsync()` (libHttpClient).
6. Parse JSON response; deserialize into typed result struct.
7. Signal `XAsyncComplete()`.

## Platform Abstraction

Platform-specific code is isolated in `Source/<Library>/Source/Platform/<Platform>/` directories. Build projects select which platform files to include.

PlayFab C SDK uses its own platform detection (not `HC_PLATFORM` macros from libHttpClient):

| Build Configuration | Platform |
|-------------------|----------|
| `Gaming.Desktop.x64` | GDK Desktop |
| `Gaming.Xbox.XboxOne.x64` | Xbox One |
| `Gaming.Xbox.Scarlett.x64` | Xbox Series X|S |
| Win32 x64 | Desktop Windows |
| Linux CMake | Linux |
| Android Gradle/CMake | Android |
| Xcode schemes | iOS / macOS |

Platform differences are primarily in:
- **Authentication**: XUser (GDK), Steam auth tickets (Steam), custom ID (all platforms)
- **Local storage**: Platform-specific file paths and APIs
- **Game save provider**: GRTS out-of-process (GDK/Windows) vs. in-process (other platforms)

## Logging

Enable debug tracing for development:

```c
PFSettingsSetTraceLevel(PFTraceLevel::Verbose);
HCSettingsSetTraceLevel(HCTraceLevel::Verbose);
```

PFSettingsSetTraceLevel enables verbose events from PlayFabCore, PlayFabServices, and PlayFabGameSave. HCSettingsSetTraceLevel enables verbose libHttpClient events. See the [tracing documentation](https://learn.microsoft.com/gaming/playfab/sdks/c/tracing) for hooking into custom log sinks.

## Build Configuration

### Adding Source Files

1. **Generated service files**: Regenerate via `pf-gensdk.cmd` (do not add manually).
2. **Core/GameSave files**: Add to the appropriate `Build/<Library>.Common/*.vcxitems` shared items project.
3. Platform-specific files go in the corresponding platform build project only.
4. Props files (`PlayFab.C.paths.props`, `*.import.props`) control include paths — do not hardcode paths in `.vcxproj` files.

### Key Property Sheets

| File | Purpose |
|------|---------|
| `Build/PlayFab.C.paths.props` | Root path definitions |
| `Build/PlayFab.C.GDK.props` | GDK-specific configuration |
| `Build/PlayFab.C.Win32.props` | Win32-specific configuration |
| `Build/PlayFab.C.GRTS.props` | Game Runtime Services configuration |
| `Build/PlayFabCore.import.props` | Import PlayFabCore into consuming projects |
| `Build/PlayFabServices.import.props` | Import PlayFabServices into consuming projects |
| `Build/PlayFabGameSave.import.props` | Import PlayFabGameSave into consuming projects |

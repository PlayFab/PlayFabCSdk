# External Dependencies

Detailed reference for all third-party and sibling library dependencies consumed by the PlayFab C SDK.

## Dependency Map

| Dependency | Location | Type | Platforms |
|------------|----------|------|-----------|
| libHttpClient | GDK extension (Xbox/Windows) or bundled | GDK extension / submodule | All |
| nlohmann JSON | `Build/vcpkg_installed/<triplet>/include/nlohmann/` | vcpkg (header-only) | All |
| libarchive | `Build/vcpkg_installed/<triplet>/include/libarchive/` | vcpkg (source staged by the port) | Game save platforms |
| zlib | Via libarchive / system | Vendored or system | Game save platforms |
| Steamworks SDK | `External/steamworks_sdk/` | Vendored | Steam platforms |
| SDKGenerator | `External/SDKGenerator/` | Git submodule | Build-time only |

## libHttpClient

**Purpose:** Cross-platform HTTP and WebSocket client library. Handles connection management, TLS, automatic retry, and platform-specific transport.

**Integration:**
- PlayFabCore's `HttpClient` wraps `HCCallHandle` for REST requests.
- Global init/cleanup: `HCInitialize()` / `HCCleanupAsync()`, called from `PFCoreGlobalState`.
- Task queue: `XTaskQueue` from libHttpClient drives all async operations.
- Tracing: `HCSettingsSetTraceLevel()` controls HTTP-level debug output.

**Key APIs consumed:**
- `httpClient/httpClient.h` — HTTP call lifecycle
- `XAsync.h` / `XTaskQueue.h` — Async execution model
- Trace level configuration

**Platform notes:**
- On GDK (Xbox/Windows), libHttpClient is provided as a GDK extension. The SDK links against it via `Build/libHttpClient.import.props`.
- On other platforms (Linux, Android, iOS/macOS), libHttpClient may be bundled as source or pre-built binary.
- Build integration files: `Build/libHttpClient.import.props`, `Build/libHttpClient.Win32/`

## nlohmann JSON

**Purpose:** JSON parsing and serialization (header-only C++ library).

**Integration:**
- All PlayFab REST request bodies are serialized using nlohmann JSON.
- All REST response bodies are deserialized using nlohmann JSON.
- Wrapper utilities in `PlayFabSharedInternal/Include/JsonUtils.h` provide safe extraction helpers.
- The generated service wrappers (`Source/PlayFabServices/Source/Generated/`) use nlohmann JSON extensively for type serialization.

**Location:** `Build/vcpkg_installed/<triplet>/include/nlohmann/`, restored by vcpkg (see [vcpkg](#vcpkg-nlohmann-json-and-libarchive)).

## libarchive

**Purpose:** Multi-format archive and compression library. Used by PlayFabGameSave for creating and extracting ZIP bundles during game save sync.

**Integration:**
- `PlayFabSharedInternal/Include/ArchiveOperations.h` wraps libarchive for ZIP operations.
- `PlayFabGameSave/Source/Common/ZipUtils.cpp` uses archive operations for save data compression.
- Save data is bundled into ≤64MB compressed ZIP archives for efficient upload/download.
- Build integration: `Build/PlayFabSharedInternal.libarchive/`

**Location:** `Build/vcpkg_installed/<triplet>/include/libarchive/`, restored by vcpkg (see [vcpkg](#vcpkg-nlohmann-json-and-libarchive)). Configuration (`config.h`) stays in `External/libarchive.config/`.

## zlib

**Purpose:** General-purpose compression library. Used as the compression backend for libarchive ZIP operations.

**Integration:**
- Linked as a dependency of libarchive.
- Build integration: `Build/PlayFabSharedInternal.zlib/`

## Steamworks SDK

**Purpose:** Valve's Steamworks API for Steam platform authentication and identity.

**Integration:**
- `PlayFabCore/Source/Common/LocalUser_Steam.cpp` bridges Steam auth into PlayFab's `PFLocalUser` system.
- Used to obtain Steam auth session tickets for `PFAuthenticationLoginWithSteamAsync`.
- Only included on Steam platform builds.

**Location:** `External/steamworks_sdk/` — vendored.

**Build integration:** `Build/Steam.import.props`

## SDKGenerator

**Purpose:** PlayFab's code generation tool. Produces service wrapper code (headers, types, API entry points) from PlayFab API specification files.

**Integration:**
- Templates in `Source/*GeneratorTemplate/` directories define the code generation patterns.
- Output goes to `Source/PlayFabServices/Source/Generated/` and `Source/PlayFabServices/Include/Generated/`.
- Run via `Utilities/Scripts/pf-gensdk.cmd`.

**Location:** `External/SDKGenerator/` — git submodule.

**Note:** This is a build-time-only dependency. It is not linked into the SDK or shipped to consumers.

## NuGet Packages

Managed via central package versioning (`Directory.Packages.props`):

| Package | Version | Purpose |
|---------|---------|---------|
| System.ServiceProcess.ServiceController | 8.0.0 | Test infrastructure |
| YamlDotNet | 13.7.1 | YAML scenario parsing (GameTestController) |

## vcpkg (nlohmann JSON and libarchive)

nlohmann JSON and libarchive are not vendored or submodules. They are restored by vcpkg from the internal `PlayFab.vcpkg` registry.

- Manifest: `Build/vcpkg.json`. Registry and baseline: `Build/vcpkg-configuration.json`. Pinned vcpkg tool: `Build/vcpkg/vcpkg-pin.json`.
- Restore scripts: `Build/vcpkg/Restore-Vcpkg.ps1` (Windows) and `Build/vcpkg/restore-vcpkg.sh` (Linux/macOS). Apple wrapper: `Build/PlayFabCore.Apple/Restore-VcpkgApple.sh`.
- Output: `Build/vcpkg_installed/<triplet>/` (git-ignored). The bootstrapped vcpkg tool lives in `.tools/vcpkg` unless `XBBL_VCPKG_ROOT` points elsewhere.
- Android triplets are overridden by `Build/vcpkg/triplets/*-android.cmake` because these ports are not compiled.

Every platform reads headers from its own triplet:

| Platform | Triplet | How the build selects it |
|----------|---------|--------------------------|
| Windows / GDK | `x64-windows` | `PlayFabVCPKGInstalledDir` in `Build/PlayFab.C.paths.props` |
| Linux | `x64-linux` | `Build/vcpkg/Vcpkg.cmake` (the `.bash` scripts also pass `-D VCPKG_TARGET_TRIPLET=x64-linux`) |
| Android | `arm64-android`, `arm-android`, `x64-android`, `x86-android` | `Build/vcpkg/Vcpkg.cmake` maps `ANDROID_ABI` (`arm64-v8a`, `armeabi-v7a`, `x86_64`, `x86`) |
| iOS | `arm64-ios` (device), `arm64-ios-simulator`, `x64-ios` (x86_64 simulator) | `PF_VCPKG_TRIPLET[sdk=...][arch=...]` in the Xcode projects |
| macOS | `arm64-osx`, `x64-osx` | `PF_VCPKG_TRIPLET[sdk=...][arch=...]` in the Xcode projects |

`Vcpkg.cmake` fails the configure step if the triplet cannot be determined or has not been restored; it never falls back to another triplet.

## Cloning with Dependencies

The only remaining submodule is SDKGenerator (build-time only), so clone with submodules and then restore the vcpkg triplet(s) for your platform:

```bash
git clone --recurse-submodules https://github.com/PlayFab/PlayFabCSdk
# or, after a shallow clone:
git submodule update --init --recursive
```

> **Warning:** GitHub's "Download ZIP" feature does not include submodules and will not build.

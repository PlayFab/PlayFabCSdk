# AGENTS.md — AI Context Bootstrap

> This file is designed for AI coding assistants (Copilot, Cursor, Aider, etc.) operating in this repository. Read this first.

## What This Repo Is

**PlayFab C SDK** is a native C/C++ SDK that game titles link against to access PlayFab backend services: authentication, catalog, inventory, leaderboards, statistics, groups, cloud scripts, push notifications, game saves, and more. It supports GDK (PC + Console), Win32, Linux, iOS, macOS, and Android.

This is **not** a web service or a standalone application. It is a set of static/shared libraries compiled into game executables.

## Key Files to Read First

| File | What You Learn |
|------|---------------|
| `specs/ARCHITECTURE.md` | System overview, directory map, data flow, component diagram |
| `specs/CONVENTIONS.md` | Naming rules, async patterns, memory management, error handling, code generation rules |
| `specs/DEPENDENCIES.md` | External libraries (nlohmann JSON, libarchive, libHttpClient, Steamworks, SDKGenerator) |
| `specs/BUILD_AND_TEST.md` | Build commands, test execution, AI build-test-fix loop guide |
| `specs/PUBLIC_DOCS.md` | Pointers to authoritative PlayFab documentation on learn.microsoft.com |
| `specs/admin-config-apis.md` | How to read/write title config (API access policy, title data, etc.) via `POST /Admin/*` REST endpoints — for agentic investigation/repair of a title behind failing SDK calls |
| `Test/AGENTS.md` | Test framework details, tags, scenario reference |
| `specs/playfab-gamesave/ai-summary.md` | Deep dive on game save architecture and API |
| `README.md` | Repo overview, platform setup, and clone instructions |

## Critical Rules

### 1. Flat C API Only
All public API is flat C with `PF` prefix. Never expose C++ types, exceptions, or STL containers across the API boundary.

### 2. Do Not Hand-Edit Generated Code
Service wrappers in `Source/PlayFabServices/Source/Generated/` and `Source/PlayFabServices/Include/Generated/` are auto-generated. Edit the templates in `*GeneratorTemplate/` directories and regenerate with `Utilities/Scripts/pf-gensdk.cmd`.

### 3. Async Pattern
All I/O uses the XAsync pattern. Never block a thread waiting for a result. The pattern is:

```c
PF<Service><Verb>Async(handle, &request, &asyncBlock)    // initiate
PF<Service><Verb>GetResult(&asyncBlock, ...)              // retrieve (in callback or after completion)
```

### 4. Memory: Handle-Based Lifetime
Public resources use create/close handle pattern with internal ref-counting. Handles: `PFServiceConfigHandle`, `PFEntityHandle`, `PFEventPipelineHandle`.

### 5. Error Handling
- C API returns `HRESULT`. Use `SUCCEEDED()` / `FAILED()` macros.
- PlayFab-specific errors: `E_PF_*` codes.
- Never throw exceptions across API boundaries.

### 6. Naming Conventions

| Context | Style | Example |
|---------|-------|---------|
| C API types | `PF` prefix + PascalCase | `PFEntityHandle`, `PFCatalogItem` |
| C API functions | `PF` + Service + Verb + `Async` | `PFCatalogSearchItemsAsync` |
| C API enums | `PF` prefix | `PFLoginIdentityProvider` |
| Error codes | `E_PF_` prefix | `E_PF_GAMESAVE_NOT_INITIALIZED` |
| Internal classes | PascalCase | `PFCoreGlobalState`, `FolderSyncManager` |
| File names | PascalCase | `GameSaveGlobalState.cpp` |

### 7. Build System
- Shared source files live in `Build/<Library>.Common/*.vcxitems`.
- Platform-specific build projects import shared items.
- Include paths controlled by `.props` files — don't hardcode in `.vcxproj`.
- Use `PlayFabGameSave.C.GDK.vs2022.sln` for builds that include test apps.

## Architecture at a Glance

```
Game Code
    │
    ▼
┌─────────────────────────────────────┐
│  PlayFabServices                    │  ← Generated service wrappers
│  (Catalog, Inventory, Leaderboards, │
│   Statistics, Groups, Friends, ...)  │
├─────────────────────────────────────┤
│  PlayFabCore                        │  ← Auth, entity, HTTP, events
│  (Authentication, Entity, HttpClient,│
│   EventPipeline, LocalUser, Trace)   │
├─────────────────────────────────────┤
│  PlayFabGameSave                    │  ← Cross-platform game saves
│  (SyncManager, Providers, UI,       │
│   Platform/GRTS, Compression)        │
├─────────────────────────────────────┤
│  PlayFabSharedInternal              │  ← JSON, HTTP, async, memory
├─────────────────────────────────────┤
│  External Dependencies              │
│  (libHttpClient, nlohmann JSON,     │
│   libarchive, Steamworks SDK)        │
└─────────────────────────────────────┘
```

## Repository Structure

```
Source/
  PlayFabCore/           # Core SDK: init, auth, entity, HTTP, telemetry
  PlayFabServices/       # Service wrappers: catalog, inventory, leaderboards, groups, etc.
  PlayFabGameSave/       # Game save / connected storage
  PlayFabSharedInternal/ # Shared internal utilities
  *GeneratorTemplate/    # Code generation templates (do not edit generated output directly)
External/                # Third-party: nlohmann JSON, libarchive, SDKGenerator, Steamworks
Test/                    # Test framework (see Test/AGENTS.md)
Utilities/Scripts/       # Build and test runner scripts
specs/                   # Architecture specs, design docs, test docs
Samples/                 # Sample apps
Build/                   # Build configuration and props files
Pipelines/               # CI/CD pipeline definitions
```

## Solutions

| Solution | Use For |
|----------|---------|
| `PlayFabGameSave.C.GDK.vs2022.sln` | **Test apps + full SDK** — use this for building/testing |
| `PlayFab.C.vs2022.sln` | SDK libraries only (no test apps) |

## Common Tasks

### Building and Testing

```powershell
# Build test apps
Utilities\Scripts\tests-build.ps1

# Run tests by component
py Utilities\Scripts\tests-run.py pfcore
py Utilities\Scripts\tests-run.py pfservices
py Utilities\Scripts\tests-run.py gamesave-inproc
py Utilities\Scripts\tests-run.py xsapi
```

See `specs/BUILD_AND_TEST.md` for complete build/test/debug loop instructions.

**NuGet:** `tests-build.ps1` moves the repo `NuGet.config` aside during the build **on purpose** — it
pins `xbox_PublicPackages`, which `401`s without an authenticated session. Never work around a NuGet
failure with `-Source https://api.nuget.org/...`; `nuget.org` is disabled machine-wide as a
supply-chain control. See
[`specs/BUILD_AND_TEST.md` §NuGet](specs/BUILD_AND_TEST.md#nuget-why-the-build-script-moves-nugetconfig-aside).

### Adding a New Service API

1. Update API specs and regenerate: `Utilities\Scripts\pf-gensdk.cmd`
2. If manual additions needed, edit templates in `*GeneratorTemplate/` directories.
3. Add test scenarios in `Test/GameTestScenarios/`.
4. Add command handlers in `Test/GameTestAppShared/`.

### Debugging Service Calls

Enable verbose tracing to see full HTTP request/response details:
```c
PFSettingsSetTraceLevel(PFTraceLevel::Verbose);
HCSettingsSetTraceLevel(HCTraceLevel::Verbose);
```

## Submodule Warning

This repo uses git submodules. Always clone with `--recurse-submodules`:

```bash
git clone --recurse-submodules <url>
git submodule update --init --recursive
```

GitHub's "Download ZIP" **will not work**.

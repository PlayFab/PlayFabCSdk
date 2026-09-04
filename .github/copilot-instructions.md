# PlayFab C SDK

Cross-platform native C/C++ SDK for PlayFab backend services. Game titles link against these libraries to access authentication, catalog, inventory, leaderboards, game saves, and more.

## Tech Stack & Dependencies

- C/C++ (MSBuild, Visual Studio 2022)
- C# .NET 8 (test controller only)
- NuGet packages → see `Directory.Packages.props`
- Platform SDKs → see `Build/*.props`
- External deps → vendored in `External/` (nlohmann JSON, libarchive, libHttpClient, Steamworks)

## Build & Test

```powershell
Utilities\Scripts\tests-build.ps1            # Build test apps (Debug)
Utilities\Scripts\tests-build.ps1 -Clean     # Clean rebuild
py Utilities\Scripts\tests-run.py pfcore     # Run core tests
py Utilities\Scripts\tests-run.py pfservices # Run services tests
py Utilities\Scripts\tests-run.py gamesave-inproc   # Run game save tests
py Utilities\Scripts\tests-run.py xsapi      # Run XSAPI tests
Utilities\Scripts\pf-gensdk.cmd              # Regenerate service wrappers
```

## Project Structure

```
Source/PlayFabCore/           - Auth, entity, HTTP, events, tracing
Source/PlayFabServices/       - Generated service wrappers (catalog, inventory, etc.)
Source/PlayFabGameSave/       - Cross-platform game save / connected storage
Source/PlayFabSharedInternal/ - Shared internal utilities (JSON, HTTP, async)
External/                     - Third-party libraries (vendored/submodules)
Test/                         - Test framework, scenarios, harness → see Test/AGENTS.md
Build/                        - MSBuild .props and platform-specific configs
Pipelines/                    - Azure Pipelines CI/CD (40+ task definitions)
Utilities/Scripts/            - Build, test, and code generation scripts
specs/                        - Architecture, conventions, build guides → see AGENTS.md
```

## Key Rules

The team-wide C/C++ source rules (flat C `PF` prefix public API, XAsync pattern for all I/O, handle-based memory with create/close ref-counting, generated-code discipline, naming conventions) apply here — see the [`partycore-cpp-source-conventions.md`](https://dev.azure.com/PlayFabInternal/Main/_git/copilot-instructions-partycore?path=/.github/instructions/partycore-cpp-source-conventions.md) shared topic referenced in the footer. Repo-specific specifics layered on top of those team rules:

- Generated wrappers live in `Source/PlayFabServices/Source/Generated/` (and matching `Include/Generated/`); edit templates in `*GeneratorTemplate/` and regenerate via `Utilities\Scripts\pf-gensdk.cmd`
- Error codes use the `E_PF_*` namespace returned as `HRESULT`, checked with `SUCCEEDED()`/`FAILED()` macros

## Key References

- Architecture details → `specs/ARCHITECTURE.md`
- Coding conventions → `specs/CONVENTIONS.md`
- Build/test/debug loop → `specs/BUILD_AND_TEST.md`
- External dependencies → `specs/DEPENDENCIES.md`
- Test framework → `Test/AGENTS.md`
- Game save deep dive → `specs/playfab-gamesave/ai-summary.md`
- Full AI bootstrap → `AGENTS.md`

## Solutions

| Solution | Use For |
|----------|---------|
| `PlayFabGameSave.C.GDK.vs2022.sln` | Full SDK + test apps (use this for building/testing) |
| `PlayFab.C.vs2022.sln` | SDK libraries only (no test apps) |

## Submodules

Always clone with `--recurse-submodules`. Run `git submodule update --init --recursive` after clone. SHA-bump PR discipline, refresh helpers, and the Windows long-paths config are documented in the team-wide [`partycore-submodule-conventions.md`](https://dev.azure.com/PlayFabInternal/Main/_git/copilot-instructions-partycore?path=/.github/instructions/partycore-submodule-conventions.md) shared topic.

## Cross-cutting team conventions

This repo follows the PlayFab `Party Service and Core SDKs` team's shared conventions. Five shared topics from the team's instructions repos are directly relevant here:

- [`partycore-cpp-source-conventions.md`](https://dev.azure.com/PlayFabInternal/Main/_git/copilot-instructions-partycore?path=/.github/instructions/partycore-cpp-source-conventions.md) — `PF` prefix and flat-C public-API surface, XAsync I/O pattern, shared base + platform overlay architecture, generated-code discipline, naming. Heavily relevant: this repo is the canonical non-NDA core of the PlayFab C SDK.
- [`partycore-props-pattern.md`](https://dev.azure.com/PlayFabInternal/Main/_git/copilot-instructions-partycore?path=/.github/instructions/partycore-props-pattern.md) — external/import pair pattern for `.props` files, path discipline, and CMake mirror discipline. Relevant: `Build/` contains the canonical `PlayFab*.import.props` and `*.external.props` files that NDA overlay repos (PlayFab.C.PS, PlayFab.C.Switch) consume.
- [`partycore-submodule-conventions.md`](https://dev.azure.com/PlayFabInternal/Main/_git/copilot-instructions-partycore?path=/.github/instructions/partycore-submodule-conventions.md) — SHA-bump PR discipline, refresh helpers, Windows long-paths. Relevant: this repo pins multiple external libraries (`libHttpClient`, `nlohmann-json`, `libarchive`, `Steamworks`) under `External/`.
- [`partycore-nda-boundary.md`](https://dev.azure.com/PlayFabInternal/Main/_git/copilot-instructions-partycore?path=/.github/instructions/partycore-nda-boundary.md) — NDA-vs-non-NDA placement policy. Relevant: this repo is the non-NDA core that pairs with NDA overlay siblings (`PlayFab.C.PS`, `PlayFab.C.Switch`) and contains Sony/Switch pipeline task scaffolding in `Pipelines/Tasks/` whose substantive build steps must remain NDA-tented.

- [`partycore-pipelines-base.md`](https://dev.azure.com/PlayFabInternal/Main/_git/copilot-instructions-partycore?path=/.github/instructions/partycore-pipelines-base.md) — ADO pipeline conventions: 3-layer template pattern, parameter forwarding, queue-time parameter rules, secrets/variable groups, dry-run protocol, security/compliance tasks, script conventions. Relevant: this repo has ADO pipeline YAML that follows these team-wide patterns.

These topics live in two router repos and are loaded automatically by Copilot CLI when `COPILOT_CUSTOM_INSTRUCTIONS_DIRS` points at them. Non-NDA contributors get the non-NDA router (`copilot-instructions-partycore`); NDA-tented contributors additionally get the NDA router (`copilot-instructions-partycore-nda`). The footer references both because the same repo is worked on by both groups.

See the pilot PRs for the reference shape of these references:

- [`PlayFab.SDKs.All` PR #15678197](https://dev.azure.com/PlayFabInternal/Main/_git/PlayFab.SDKs.All/pullrequest/15678197) (non-NDA super-repo)
- [`LibHttpClient.PS` PR #3005](https://dev.azure.com/PlayFabInternal/Main/_git/LibHttpClient.PS/pullrequest/3005) (NDA library overlay)

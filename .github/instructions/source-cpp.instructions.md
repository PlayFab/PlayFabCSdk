---
applyTo: "Source/**/*.cpp,Source/**/*.h,Source/**/*.c"
---

# C/C++ Source Code Conventions

The team-wide C/C++ source conventions cover the public-API surface and `PF` prefix, the XAsync I/O pattern, the shared base + platform overlay architecture, handle-based memory management, generated-code discipline, naming, and platform-conditional code rules. They are documented in the [`partycore-cpp-source-conventions.md`](https://dev.azure.com/PlayFabInternal/Main/_git/copilot-instructions-partycore?path=/.github/instructions/partycore-cpp-source-conventions.md) shared topic referenced from the root `copilot-instructions.md` footer.

This file documents only the PlayFab.C-specific layering on top of those team rules.

## Repo-specific rules

| Topic | Rule |
|-------|------|
| Generated code location | `Source/PlayFabServices/Source/Generated/` (sources) + `Source/PlayFabServices/Include/Generated/` (headers) |
| Regeneration | `Utilities\Scripts\pf-gensdk.cmd` from templates in `*GeneratorTemplate/` directories |
| Error code namespace | `E_PF_*` returned as `HRESULT`, checked with `SUCCEEDED()` / `FAILED()` |
| Shared source items | `Build/<Library>.Common/*.vcxitems` — platform-specific `.vcxproj` files import these |
| Include path control | Driven by `Build/*.props` files; never hardcode include paths in `.vcxproj` |
| Solution for builds with test apps | `PlayFabGameSave.C.GDK.vs2022.sln` |
| Solution for SDK libraries only | `PlayFab.C.vs2022.sln` |

## Key References

- Full conventions → `specs/CONVENTIONS.md`
- Architecture → `specs/ARCHITECTURE.md`
- Dependencies → `specs/DEPENDENCIES.md`
- MSBuild `.props` pattern (team-wide) → [`partycore-props-pattern.md`](https://dev.azure.com/PlayFabInternal/Main/_git/copilot-instructions-partycore?path=/.github/instructions/partycore-props-pattern.md)
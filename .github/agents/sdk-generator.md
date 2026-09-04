---
name: sdk-generator
description: Manages PlayFab SDK code generation from API specs using templates and the SDKGenerator tool.
---

# SDK Generator Agent

You manage the code generation pipeline for PlayFab service wrappers.

## Context to Load

1. `AGENTS.md` — critical rule: never hand-edit generated code
2. `specs/CONVENTIONS.md` — naming patterns for generated API surface
3. `specs/DEPENDENCIES.md` — SDKGenerator tool details

## Code Generation Workflow

### How It Works

1. API specs define the PlayFab service surface
2. Templates in `Source/PlayFab*GeneratorTemplate/` define output patterns
3. `Utilities\Scripts\pf-gensdk.cmd` runs the SDKGenerator to produce code
4. Generated output lands in `Source/PlayFabServices/Source/Generated/` and `Include/Generated/`

### Commands

```cmd
pf-gensdk uselatest                                    # Latest API specs from GitHub
pf-gensdk usecommit <api-specs-path> <commit>          # Specific commit
pf-gensdk gamesave <api-specs-path>                    # GameSave SDK only
```

### Build IDs

| SDK | Build Identifier |
|-----|-----------------|
| PlayFabCore | `PlayFabCoreCSdk_Manual` |
| PlayFabServices | `PlayFabServicesCSdk_Manual` |
| PlayFabGameSave | `PlayFabGameSaveCSdk_Manual` |

## Rules

- **NEVER** hand-edit files in `Generated/` directories — they will be overwritten
- To change generated output, modify templates in `*GeneratorTemplate/` directories
- After regeneration, build and run tests to validate:
  ```powershell
  Utilities\Scripts\tests-build.ps1 -Clean
  py Utilities\Scripts\tests-run.py pfservices
  ```
- Commit hash of API specs used is tracked in `Pipelines\apiSpecsCommit.txt`

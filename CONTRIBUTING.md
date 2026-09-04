# Contributing to PlayFab C SDK

## Getting Started

1. Clone with submodules: `git clone --recurse-submodules <url>`
2. Open `PlayFabGameSave.C.GDK.vs2022.sln` in Visual Studio 2022
3. Read `AGENTS.md` for repository overview and conventions
4. Read `specs/ARCHITECTURE.md` for system architecture

## Prerequisites

- Visual Studio 2022
- GDK (Gaming Development Kit)
- .NET 8 SDK (for test controller)
- PlayFab developer account with test title data

## Development Workflow

### Build

```powershell
Utilities\Scripts\tests-build.ps1            # Debug build
Utilities\Scripts\tests-build.ps1 -Clean     # Clean rebuild
```

### Test

```powershell
py Utilities\Scripts\tests-run.py pfcore       # Core tests
py Utilities\Scripts\tests-run.py pfservices   # Services tests
py Utilities\Scripts\tests-run.py gamesave-inproc     # Game save tests
py Utilities\Scripts\tests-run.py xsapi        # XSAPI tests
```

### Code Generation

Service wrappers are auto-generated. To update:

1. Edit templates in `*GeneratorTemplate/` directories
2. Run `Utilities\Scripts\pf-gensdk.cmd`
3. **Never** hand-edit files in `Source/PlayFabServices/Source/Generated/`

## Coding Standards

### API Conventions

- All public API is flat C with `PF` prefix
- No C++ types, STL containers, or exceptions across API boundaries
- XAsync pattern for all I/O — never block threads
- Handle-based memory with create/close ref-counting
- HRESULT error handling with `E_PF_*` codes

### Naming

| Context | Convention | Example |
|---------|-----------|---------|
| C API functions | `PF<Service><Verb>Async` | `PFCatalogSearchItemsAsync` |
| C API types | `PF` + PascalCase | `PFEntityHandle` |
| Error codes | `E_PF_` prefix | `E_PF_GAMESAVE_NOT_INITIALIZED` |
| Internal classes | PascalCase | `FolderSyncManager` |
| File names | PascalCase | `GameSaveGlobalState.cpp` |

### File Organization

- Shared source in `Build/<Library>.Common/*.vcxitems`
- Platform-specific projects import shared items
- Include paths via `.props` files — don't hardcode in `.vcxproj`

## Branching & Pull Request Workflow

### Branch Naming

Use descriptive branch names with a prefix:

| Prefix | Use For | Example |
|--------|---------|---------|
| `feature/` | New functionality | `feature/add-inventory-batch-api` |
| `fix/` | Bug fixes | `fix/gamesave-conflict-race-condition` |
| `refactor/` | Internal improvements | `refactor/shared-internal-http-cleanup` |
| `docs/` | Documentation only | `docs/update-build-test-guide` |
| `test/` | Test additions or fixes | `test/add-pfcore-login-scenarios` |

### Creating a Pull Request

1. **Create a branch** from `main`:
   ```bash
   git checkout main && git pull
   git checkout -b feature/your-change-description
   ```

2. **Make your changes** following the conventions in `specs/CONVENTIONS.md`.

3. **Build and test locally**:
   ```powershell
   Utilities\Scripts\tests-build.ps1
   py Utilities\Scripts\tests-run.py <relevant-suite>
   ```

4. **If you modified generated code templates**, regenerate:
   ```cmd
   Utilities\Scripts\pf-gensdk.cmd uselatest
   ```

5. **Push and open a PR** targeting `main`. Fill out the PR template completely.

6. **CI runs automatically** — the PR pipeline builds across 6+ platforms (Linux, Android, iOS, macOS, GDK/Xbox, PlayStation). All must pass before merge.

7. **Address review feedback** and ensure all checklist items are checked.

### PR Requirements

| Requirement | Details |
|-------------|---------|
| Tests pass | All relevant component tests pass locally before submission |
| New tests | New functionality must include YAML test scenarios |
| No hand-edited generated code | Use templates + `pf-gensdk.cmd` |
| CI green | All 6+ platform builds must pass |
| PR template complete | All applicable checklist items checked |
| Specs referenced | PR description links relevant architecture/convention docs |

### CLA Requirement

When you submit a pull request, a CLA bot will automatically determine whether you need to provide a Contributor License Agreement. Follow the bot's instructions. This is a one-time requirement across all repos using our CLA.

## Key References

| Document | What It Covers |
|----------|---------------|
| `AGENTS.md` | AI bootstrap, critical rules, architecture overview |
| `specs/ARCHITECTURE.md` | System design, directory map, data flow |
| `specs/CONVENTIONS.md` | Naming, async patterns, memory management |
| `specs/BUILD_AND_TEST.md` | Complete build/test/debug guide |
| `specs/DEPENDENCIES.md` | External library details |

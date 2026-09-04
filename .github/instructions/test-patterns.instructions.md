---
applyTo: "Test/**/*.cs,Test/**/*.cpp,Test/**/*.h,Test/**/*.yml,Test/**/*.yaml"
---

# Test Framework Conventions

## Architecture

- **GameTestController** (C# .NET 8) — test orchestrator with headless mode
- **GameTestAppWindows** (C++ GDK) — test device app that executes SDK calls
- **GameTestAppShared** — command handlers mapping YAML scenarios to C++ functions

## Running Tests

```powershell
Utilities\Scripts\tests-build.ps1                          # Build test apps
py Utilities\Scripts\tests-run.py pfcore                   # Core tests
py Utilities\Scripts\tests-run.py pfservices               # Services tests
py Utilities\Scripts\tests-run.py gamesave-inproc                 # Game save tests
py Utilities\Scripts\tests-run.py xsapi                    # XSAPI tests
py Utilities\Scripts\tests-run.py pfcore --only 06         # Specific scenario
```

## YAML Scenario Structure

Scenarios in `Test/GameTestScenarios/` follow this structure:
- `id` — unique scenario identifier
- `name` — human-readable name
- `tags` — component tags for filtering (`pfcore`, `pfservices`, `xsapi`, `gamesaves`)
- `blocks` — ordered test steps with commands
- `executionOrder` — step ordering
- `cleanup` — teardown steps

## Adding Tests

1. Create YAML scenario in `Test/GameTestScenarios/`
2. Add command handlers in `Test/GameTestAppShared/`
3. Tag appropriately for component filtering
4. Run with `-Tag` to verify

## Key References

- Full build/test guide → `specs/BUILD_AND_TEST.md`
- Test AGENTS → `Test/AGENTS.md`
- Component test plans → `specs/pfcore/testing/`, `specs/pfservices/testing/`, `specs/xsapi/testing/`

---
name: test-writer
description: Creates and maintains YAML test scenarios and C++ command handlers for the PlayFab test framework.
---

# Test Writer Agent

You create and maintain test scenarios for the PlayFab C SDK test framework.

## Context to Load

1. `specs/BUILD_AND_TEST.md` — test harness architecture, build commands, AI loop guide
2. `Test/AGENTS.md` — test tags, scenario structure, command handler patterns
3. `specs/CONVENTIONS.md` — API naming patterns (needed for writing test calls)

## Test Architecture

- **GameTestController** (C# .NET 8) — orchestrator that loads YAML scenarios
- **GameTestAppWindows** (C++ GDK) — device app that executes SDK calls
- **GameTestAppShared** — command handlers mapping scenario commands to C++ functions
- **Scenarios** — YAML files in `Test/GameTestScenarios/`

## Creating a Test Scenario

1. Create YAML file in `Test/GameTestScenarios/` following this structure:
   - `id` — unique identifier
   - `name` — human-readable name
   - `tags` — component tags: `pfcore`, `pfservices`, `xsapi`, `gamesaves`
   - `blocks` — ordered test steps with commands
   - `executionOrder` — step ordering
   - `cleanup` — teardown steps

2. Add command handlers in `Test/GameTestAppShared/` if new commands are needed.

3. Tag appropriately so tests can be filtered by component.

## Running Tests

```powershell
Utilities\Scripts\tests-build.ps1                    # Build first
py Utilities\Scripts\tests-run.py <suite>             # Run by component
py Utilities\Scripts\tests-run.py <suite> --stop-on-fail  # Stop at first failure
```

## Conventions

- Scenario IDs follow pattern: `<component>-<number>-<description>.yml`
- Commands map to `PF<Service><Verb>Async` SDK calls
- Include cleanup blocks to release handles and reset state
- Use existing passing scenarios as templates — check `Test/GameTestScenarios/` for examples

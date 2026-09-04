## Description

<!-- Briefly describe what this PR does and why. -->

## Related Issues

<!-- Closes #issue or References #issue. Remove if not applicable. -->

## What Changed

- [ ] New API functions
- [ ] Bug fix (describe root cause below)
- [ ] Internal refactoring
- [ ] Generated code update (via `pf-gensdk.cmd`)
- [ ] Documentation / specs update
- [ ] Test scenarios added or updated
- [ ] Build / pipeline changes

## Testing

### Local Verification

- [ ] Builds succeed: `Utilities\Scripts\tests-build.ps1`
- [ ] Relevant tests pass (check all that apply):
  - [ ] `py tests-run.py pfcore`
  - [ ] `py tests-run.py pfservices`
  - [ ] `py tests-run.py gamesave-inproc`
  - [ ] `py tests-run.py xsapi`
- [ ] New functionality includes test scenarios in `Test/GameTestScenarios/`

### Platforms Verified Locally

<!-- Check any you built/tested locally. CI covers the rest. -->

- [ ] Windows / GDK
- [ ] Linux
- [ ] iOS / macOS
- [ ] Android

## Checklist

### API & Naming Conventions

- [ ] Public API is flat C with `PF` prefix — no C++ types, STL, or exceptions across boundaries
- [ ] Functions follow `PF<Service><Verb>Async` / `PF<Service><Verb>GetResult` pattern
- [ ] Error codes use `E_PF_*` prefix, returned as HRESULT

### Async & Memory

- [ ] All I/O uses XAsync pattern — no thread blocking
- [ ] Handle-based lifetime (create/close with ref-counting) for new resources
- [ ] Result buffers follow count-then-fetch pattern

### Generated Code

- [ ] Files in `Source/PlayFabServices/Source/Generated/` and `Include/Generated/` are NOT hand-edited
- [ ] Template changes made in `*GeneratorTemplate/` directories
- [ ] Ran `Utilities\Scripts\pf-gensdk.cmd` after template changes

### Build Configuration

- [ ] No hardcoded include paths in `.vcxproj` — use `.props` files
- [ ] Shared source added to `Build/<Library>.Common/*.vcxitems`
- [ ] Platform-specific code isolated in `Source/<Library>/Source/Platform/<Platform>/`

## CI Pipeline

This PR will automatically run:
- Linux builds (Debug + Release)
- Android build (x64_arm8 Debug)
- iOS builds (Debug + Release, simulator)
- macOS builds (Debug + Release)
- GDK/Xbox builds (x64 Debug + Release)
- PlayStation build

## References

<!-- Link relevant specs or docs the reviewer should read. -->

- [`AGENTS.md`](../AGENTS.md) — Critical rules and architecture
- [`specs/CONVENTIONS.md`](../specs/CONVENTIONS.md) — Naming, async, memory rules
- [`specs/ARCHITECTURE.md`](../specs/ARCHITECTURE.md) — System design
- [`specs/BUILD_AND_TEST.md`](../specs/BUILD_AND_TEST.md) — Build/test guide

---
applyTo: "Pipelines/**/*.yml,Pipelines/**/*.yaml"
---

# CI/CD Pipeline Conventions

## Pipeline Structure

- Main pipelines in `Pipelines/` root (PR validation, official builds, releases)
- Reusable task definitions in `Pipelines/Tasks/` (40+ tasks)
- Multi-platform: GDK, Android, iOS, Linux, macOS, Sony, Switch

## Key Pipelines

| Pipeline | Purpose |
|----------|---------|
| `PlayFab.C.PullRequest.yml` | PR validation |
| `PlayFab.C-Official.yml` | Official builds |
| `PlayFab.C.Github.Release.yml` | GitHub release automation |
| `PlayFab.C.GameSaveTests.yml` | Game save test runs |

## Conventions

- Security scanning via `Tasks/security-compliance.yml`
- Component governance via `Tasks/component-governance.yml`
- NuGet central versioning → `Directory.Packages.props`
- Build properties imported from `Build/*.props` files
- vcpkg dependencies (nlohmann, libarchive) must be restored before any build step: use `Tasks/vcpkg-restore.yml` (Windows/GDK), `Tasks/android/vcpkg-restore.yml`, `Tasks/linux/vcpkg-restore.yml` or `Tasks/apple/vcpkg-restore.yml`. In OneBranch tasks include them with `@self`; they run in the restore phase.
- Reference scripts under `Build/` with that exact casing (`Build/vcpkg/...`); Linux agents are case-sensitive
- TSA configuration in `.config/tsaOptions.json`

## When Adding Pipeline Tasks

- Follow existing task template patterns in `Pipelines/Tasks/`
- Reference shared build props, don't hardcode paths
- Include security/compliance tasks for release pipelines

# Public Documentation Guide

Reference to authoritative PlayFab and GDK documentation that explains the services the PlayFab C SDK implements. These docs live on learn.microsoft.com and describe the **service-side behavior** that the SDK wraps.

## Essential Reading (Start Here)

### PlayFab C SDK Fundamentals

| Document | URL | Why It Matters |
|----------|-----|----------------|
| PlayFab C SDK Overview | [learn.microsoft.com/gaming/playfab/sdks/c](https://learn.microsoft.com/gaming/playfab/sdks/c) | Landing page for the C SDK — getting started, platform guides, API reference |
| Quickstart: PlayFab C SDK | [learn.microsoft.com/gaming/playfab/sdks/c/quickstart](https://learn.microsoft.com/gaming/playfab/sdks/c/quickstart) | Step-by-step setup for first PlayFab API call |
| Debug Tracing | [learn.microsoft.com/gaming/playfab/sdks/c/tracing](https://learn.microsoft.com/gaming/playfab/sdks/c/tracing) | **Must-read.** How to enable verbose HTTP logging for debugging |

### Async Programming Model

| Document | URL | Why It Matters |
|----------|-----|----------------|
| Async Programming Model (GDK) | [learn.microsoft.com/gaming/gdk/_content/gc/system/overviews/async-programming-model](https://learn.microsoft.com/gaming/gdk/_content/gc/system/overviews/async-programming-model) | **Must-read.** Explains `XAsyncBlock`, `XTaskQueueHandle`, and the async model used by every SDK call |
| XAsync Library | [learn.microsoft.com/gaming/gdk/_content/gc/system/overviews/async-libraries/async-library-xasync](https://learn.microsoft.com/gaming/gdk/_content/gc/system/overviews/async-libraries/async-library-xasync) | Detailed async/task queue documentation |

### PlayFab Service Overview

| Document | URL | Why It Matters |
|----------|-----|----------------|
| PlayFab Overview | [learn.microsoft.com/gaming/playfab/what-is-playfab](https://learn.microsoft.com/gaming/playfab/what-is-playfab) | High-level feature map of all PlayFab services |
| PlayFab Game Manager | [learn.microsoft.com/gaming/playfab/gamemanager](https://learn.microsoft.com/gaming/playfab/gamemanager) | Web portal for title configuration, player data, analytics |

> **Inspecting/repairing title config from an agent:** the same configuration Game Manager edits is
> reachable as `POST /Admin/*` REST endpoints. When a service call fails with `1191
> NotAuthorizedByTitle` / `403`, or you need to read/change title config without a build or repro,
> see `specs/admin-config-apis.md`.

## Service-Specific Documentation

Read these when working on the corresponding SDK service module.

### Authentication & Identity

Maps to: `Source/PlayFabCore/Source/Authentication/`

| Document | URL |
|----------|-----|
| Authentication Overview | [learn.microsoft.com/gaming/playfab/features/authentication](https://learn.microsoft.com/gaming/playfab/features/authentication) |
| Login Basics & Best Practices | [learn.microsoft.com/gaming/playfab/features/authentication/login/login-basics-best-practices](https://learn.microsoft.com/gaming/playfab/features/authentication/login/login-basics-best-practices) |

### Economy (Catalog, Inventory, Stores)

Maps to: `Source/PlayFabServices/Source/Generated/Catalog.cpp`, `Inventory.cpp`

| Document | URL |
|----------|-----|
| Economy v2 Overview | [learn.microsoft.com/gaming/playfab/features/economy-v2](https://learn.microsoft.com/gaming/playfab/features/economy-v2) |
| Catalog Overview | [learn.microsoft.com/gaming/playfab/features/economy-v2/catalog](https://learn.microsoft.com/gaming/playfab/features/economy-v2/catalog) |
| Inventory Overview | [learn.microsoft.com/gaming/playfab/features/economy-v2/inventory](https://learn.microsoft.com/gaming/playfab/features/economy-v2/inventory) |

### Leaderboards & Statistics

Maps to: `Source/PlayFabServices/Source/Generated/Leaderboards.cpp`, `Statistics.cpp`

| Document | URL |
|----------|-----|
| Leaderboards Overview | [learn.microsoft.com/gaming/playfab/features/social/tournaments-leaderboards](https://learn.microsoft.com/gaming/playfab/features/social/tournaments-leaderboards) |
| Statistics Overview | [learn.microsoft.com/gaming/playfab/features/data/playerdata](https://learn.microsoft.com/gaming/playfab/features/data/playerdata) |

### Groups & Friends

Maps to: `Source/PlayFabServices/Source/Generated/Groups.cpp`, `Friends.cpp`

| Document | URL |
|----------|-----|
| Groups Overview | [learn.microsoft.com/gaming/playfab/features/social/groups](https://learn.microsoft.com/gaming/playfab/features/social/groups) |
| Friends Overview | [learn.microsoft.com/gaming/playfab/features/social/friends](https://learn.microsoft.com/gaming/playfab/features/social/friends) |

### CloudScript

Maps to: `Source/PlayFabServices/Source/Generated/CloudScript.cpp`

| Document | URL |
|----------|-----|
| CloudScript Overview | [learn.microsoft.com/gaming/playfab/features/automation/cloudscript](https://learn.microsoft.com/gaming/playfab/features/automation/cloudscript) |
| Azure Functions | [learn.microsoft.com/gaming/playfab/features/automation/cloudscript-af](https://learn.microsoft.com/gaming/playfab/features/automation/cloudscript-af) |

### Multiplayer Servers

Maps to: `Source/PlayFabServices/Source/Generated/MultiplayerServer.cpp`

| Document | URL |
|----------|-----|
| Multiplayer Servers Overview | [learn.microsoft.com/gaming/playfab/features/multiplayer/servers](https://learn.microsoft.com/gaming/playfab/features/multiplayer/servers) |

### Push Notifications

Maps to: `Source/PlayFabServices/Source/Generated/PushNotifications.cpp`

| Document | URL |
|----------|-----|
| Push Notifications | [learn.microsoft.com/gaming/playfab/features/engagement/push-notifications](https://learn.microsoft.com/gaming/playfab/features/engagement/push-notifications) |

### Player Data Management

Maps to: `Source/PlayFabServices/Source/Generated/PlayerDataManagement.cpp`, `Data.cpp`

| Document | URL |
|----------|-----|
| Player Data Overview | [learn.microsoft.com/gaming/playfab/features/data/playerdata](https://learn.microsoft.com/gaming/playfab/features/data/playerdata) |
| Entity Objects (Files) | [learn.microsoft.com/gaming/playfab/features/data/entities](https://learn.microsoft.com/gaming/playfab/features/data/entities) |

### Experimentation

Maps to: `Source/PlayFabServices/Source/Generated/Experimentation.cpp`

| Document | URL |
|----------|-----|
| Experimentation | [learn.microsoft.com/gaming/playfab/features/analytics/experiments](https://learn.microsoft.com/gaming/playfab/features/analytics/experiments) |

### Game Saves

Maps to: `Source/PlayFabGameSave/`

| Document | URL |
|----------|-----|
| Game Saves Overview | [learn.microsoft.com/gaming/playfab/features/data/game-saves](https://learn.microsoft.com/gaming/playfab/features/data/game-saves) |

> **Note:** Additional game save design docs are in `specs/playfab-gamesave/design/` and `specs/playfab-gamesave/docs/` within this repo.

## API Reference

### C API Reference

The complete C API reference is on learn.microsoft.com:

| Section | URL |
|---------|-----|
| API Reference (all services) | [learn.microsoft.com/gaming/playfab/api-references/c](https://learn.microsoft.com/gaming/playfab/api-references/c) |
| PFAuthentication | [learn.microsoft.com/gaming/playfab/api-references/c/pfauthentication/pfauthentication_members](https://learn.microsoft.com/gaming/playfab/api-references/c/pfauthentication/pfauthentication_members) |
| PFCatalog | [learn.microsoft.com/gaming/playfab/api-references/c/pfcatalog/pfcatalog_members](https://learn.microsoft.com/gaming/playfab/api-references/c/pfcatalog/pfcatalog_members) |
| PFInventory | [learn.microsoft.com/gaming/playfab/api-references/c/pfinventory/pfinventory_members](https://learn.microsoft.com/gaming/playfab/api-references/c/pfinventory/pfinventory_members) |

### REST API Reference

Raw REST endpoint documentation (useful when debugging HTTP calls):

| Section | URL |
|---------|-----|
| PlayFab REST API Reference | [learn.microsoft.com/rest/api/playfab](https://learn.microsoft.com/rest/api/playfab) |

## In-Repo Documentation

These documents within this repo provide deeper technical detail:

| Document | Path | Description |
|----------|------|-------------|
| Admin Config APIs | `specs/admin-config-apis.md` | How to read/write PlayFab title config (API access policy, title data, etc.) via `POST /Admin/*` REST endpoints — for agentic investigation and repair |
| Game Save AI Bootstrap | `specs/playfab-gamesave/ai-bootstrap.md` | Context loading instructions for game save work |
| Game Save Summary | `specs/playfab-gamesave/ai-summary.md` | Distilled knowledge pack on PFGameSave |
| Game Save Dev Spec | `specs/playfab-gamesave/design/client-dev-spec.md` | Detailed client implementation spec |
| Game Save State Machine | `specs/playfab-gamesave/design/state-machine.md` | Sync state machine design |
| Game Save Conflict Resolution | `specs/playfab-gamesave/design/conflict-resolution-flow.md` | Conflict resolution flow design |
| Game Save Testing | `specs/playfab-gamesave/testing/` | Test strategy, gap analysis, automation specs |
| Game Save Code Review | `specs/playfab-gamesave/ai-code-review/` | Systematic code review findings and tasks |
| Test Framework Guide | `Test/AGENTS.md` | Test harness usage, build commands, tag reference |

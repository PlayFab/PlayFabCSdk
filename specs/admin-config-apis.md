# PlayFab Admin config APIs

How PlayFab exposes title configuration as read/write REST APIs, why that shape works well for
agentic investigation and repair, and the semantics that will bite you if you don't know them.

> **How this fits the PlayFab C SDK repo.** These are **service-side** Admin APIs — the same title
> configuration that the SDK's client/entity calls run against at runtime. They are not part of the C
> SDK surface, but they are the fastest way for an AI agent to **inspect and repair the title config**
> behind a failing SDK call (for example, a `1191 NotAuthorizedByTitle` / `403` from a service call is
> usually an API-access-policy problem, fixable here without a build or repro). Use a **test title**.
>
> Source of truth for everything below is the PlayFab service front door (`pf-main`). File and line
> references are included so claims can be re-verified as the service changes.

## TL;DR

Most PlayFab title configuration is reachable through the **Admin API** — a flat set of
`POST /Admin/<ApiName>` endpoints authenticated by a single header. Config is generally exposed as
a matched read/write pair (`GetPolicy` / `UpdatePolicy`, `GetTitleData` / `SetTitleData`, and so
on). That means an agent can inspect real state, compute a precise change, apply it, and verify the
result — without a UI, a repro build, or a deployment.

The API access policy (`ApiPolicy`) is the best example of the pattern, and the one that most often
needs repair, so it gets the detailed treatment below.

## Authentication

| Property | Value | Source |
|---|---|---|
| Base route | `/Admin/` | `Server/MainServer/Controllers/AdminAPIController.cs:68` |
| Host | `https://<TitleId>.playfabapi.com` | — |
| Method | `POST` (all Admin APIs) | — |
| Content type | `application/json` (enforced) | `AdminAPIController.cs:62` |
| Credential | `X-SecretKey: <title secret key>` | `Server/Utility/HttpHeaderNames.cs:36` |
| Guard | `[RequireSecretKey]` attribute | `Server/AspNetCoreUtils/Filters/RequireSecretKey.cs:12` |

One header, one credential, no token exchange or refresh. That's most of why this surface is
pleasant to automate against.

The secret key is a **title-wide admin credential**. Treat it as a secret: keep it in an
environment variable, never in a repo, and prefer a test title when experimenting. See
[Secret key management](https://learn.microsoft.com/gaming/playfab/live-service-management/gamemanager/secret-key-management).

## The core loop

The pattern that makes this tractable for an agent:

```
GetPolicy            →  read real state (not what a UI claims)
ValidateApiPolicy    →  dry-run the change, get a diff, no writes
UpdatePolicy         →  apply, additively
GetPolicy            →  re-read and diff to confirm
<exercise the API>   →  prove the behavior actually changed
```

Steps 2 and 4 are what make this safe enough to do unattended. The dry run tells you what *would*
change before anything does, and the re-read plus behavioral test proves the change landed rather
than trusting the write response.

## Worked example: the API access policy

### Reading

```powershell
$h = @{ 'X-SecretKey' = $env:PLAYFAB_SECRET_KEY_TEST; 'Content-Type' = 'application/json' }
$body = @{ PolicyName = 'ApiPolicy' } | ConvertTo-Json
$r = Invoke-RestMethod -Uri 'https://<TitleId>.playfabapi.com/Admin/GetPolicy' `
                       -Method Post -Headers $h -Body $body
$r.data.Statements | Format-Table Resource, Effect, Principal
```

`GetPolicyResponse` returns `PolicyName`, `PolicyVersion`, `Statements[]`, and `LastUpdated`
(`Server/MainServer/Controllers/AdminAPIController/Permissions.cs:131-137`). `LastUpdated` is null
if the title has never customized its policy — a useful signal on its own.

### The statement shape

Defined at `Server/WebAPIModels/AdminAPIModels.cs:2336-2365`.

| Field | Required | Legal values |
|---|---|---|
| `Resource` | yes | `pfrn:api--<path>`, for example `pfrn:api--/Lobby/*` or `pfrn:api--*` |
| `Effect` | yes | `Allow` or `Deny` |
| `Action` | no | Only `*`. Defaults to `*` when omitted. |
| `Principal` | yes | `*`, or an entity-type object such as `{"title_player_account":"*"}` |
| `Comment` | no | Free text, bookkeeping only |
| `ApiConditions` | no | `HasSignatureOrEncryption` — `Any` / `True` / `False` |

The resource string is `ApiPrefix` + `PrefixSeparator` + path, that is `pfrn:api` + `--` + `/Lobby/*`
(`Server/DataModel/Permissions/PolicyConstants.cs:109,119`). Note there is **no** `*` between `--`
and the path; `pfrn:api--*` is the match-everything form.

### Dry-running a change

`POST /Admin/ValidateApiPolicy` applies the merge in memory and returns the result without saving
(`Permissions.cs:186-242`). This is the single most useful endpoint on this surface for automated
work, and it isn't in the public docs.

```powershell
$body = @{
  PolicyName = 'ApiPolicy'; PolicyVersion = 13; OverwritePolicy = $false
  Statements = @(@{ Resource = 'pfrn:api--/Lobby/*'; Action = '*'
                    Effect = 'Allow'; Principal = '*'; Comment = 'restore lobby grant' })
} | ConvertTo-Json -Depth 6
$r = Invoke-RestMethod -Uri 'https://<TitleId>.playfabapi.com/Admin/ValidateApiPolicy' `
                       -Method Post -Headers $h -Body $body
$r.data.Diff
```

It returns `IsValid`, `ValidationErrors`, `Warnings`, the full `ResultingStatements`, and a `Diff`
of `StatementsAdded` / `Removed` / `Unchanged` / `Replaced` / `TotalResultingStatements`.

The `Warnings` are worth reading on their own. `PolicyResourceValidator.ValidateResourcePaths`
cross-checks every resource against the live API list, so it flags statements that can never match
anything (`Permissions.cs:81-83`, and the same check on `UpdatePolicy`). Running this against one
real test title surfaced 23 dead statements — leftovers naming API groups that no longer exist,
such as `/Matchmaker/*`, `/Limits/*`, `/UserGeneratedContent/*`, and `/GameSave/*`. Those are inert,
not harmful, but they're noise that makes a policy harder to reason about.

### Writing

Identical body, sent to `/Admin/UpdatePolicy`. The critical field is `OverwritePolicy`:

- `false` — **merge**. New statements are added; a statement with the same resource replaces the
  existing one. This is what you almost always want.
- `true` — **replace**. The submitted statements become the entire policy. Everything else is
  dropped.

`UpdatePolicy` returns the resulting statements plus the same resource `Warnings`. It does **not**
return a usable `PolicyVersion`, so always re-read with `GetPolicy` to confirm.

## Semantics that bite

**Default-deny.** If no statement matches a request, the verdict is
`PolicyConstants.DisallowByDefaultVerdict` and the call fails
(`Server/UberNetServices/Policies/ClaimPolicyJudge.cs:119`). An API that is simply *absent* from the
policy is denied, not allowed. This produces `403 / 1191 NotAuthorizedByTitle`.

**Deny is evaluated before Allow.** `ClaimPolicyJudge` checks Deny statements first and returns
immediately on a match (`ClaimPolicyJudge.cs:89-102`), only then checking Allow
(`:104-117`). Adding an Allow will not override an existing Deny for the same resource. When
triaging a 1191, check for an explicit Deny *before* assuming a grant is missing.

**`PolicyVersion` is a schema handshake, not optimistic concurrency.** This one is easy to get
wrong. Both `GetPolicy` and `UpdatePolicy` use the global constant
`PolicyConstants.CurrentPolicyVersion` (currently `13`, at `PolicyConstants.cs:18`), not a
per-title revision. `GetPolicy` returns that constant (`Permissions.cs:134`) and `UpdatePolicy`
rejects anything else (`Permissions.cs:56-59`). It confirms you know the current policy *schema*;
it does **not** protect against a concurrent writer. Two agents editing the same title's policy
will silently clobber each other. Serialize your writes.

**Policy changes are not immediate.** Policies are cached per title for 10 minutes hard / 5 minutes
soft refresh, with up to 10 seconds of jitter (`Server/UberNetServices/Policies/PolicyService.cs:611-616`).
There is no production invalidation path — `ResetAllPolicyCache()` exists but isn't called. Expect
up to 10 minutes before a policy edit takes effect, and have the caller re-authenticate to pick up
a fresh entity token.

**Overwriting can permanently strand default grants.** PlayFab periodically adds new default
statements, merged in by version. `MergeVersionedPolicies` only merges defaults *strictly newer*
than the title's stored `BasePolicyVersion`, and the read path bumps that stored version to the max
(`PolicyService.cs:672-712`, especially `:686` and `:707`). So an `OverwritePolicy: true` save
persists at the current max version with whatever statements you sent — and every older default is
now unreachable forever. Prefer `OverwritePolicy: false`.

## Other Admin APIs with the same shape

The merge column is the one to check before writing. It is **not** consistent across this surface.

| Config area | Read | Write | Write semantics |
|---|---|---|---|
| API access policy | `GetPolicy` | `UpdatePolicy` | Merge or replace via `OverwritePolicy` |
| Title data (client-visible) | `GetTitleData` | `SetTitleData` | Additive, one key per call; null value **deletes** the key |
| Title data (server-only) | `GetTitleInternalData` | `SetTitleInternalData` | Same as above |
| Title data (batch) | `GetTitleData` | `SetTitleDataAndOverrides` | Additive, batched, supports override labels |
| Player statistics | `GetPlayerStatisticDefinitions` | `UpdatePlayerStatisticDefinition` | Per-statistic update |
| Segments | `GetSegments` | `CreateSegment` / `UpdateSegment` / `DeleteSegment` | Per-segment; rate limited |
| Cloud Script | `GetCloudScriptRevision` / `GetCloudScriptVersions` | `UpdateCloudScript` / `SetPublishedRevision` | New revision, then explicit publish |
| Catalog (legacy) | `GetCatalogItems` | `UpdateCatalogItems` | **Additive** |
| Catalog (legacy) | `GetCatalogItems` | `SetCatalogItems` | **Full replace — deletes unlisted items** |
| Virtual currency (legacy) | `ListVirtualCurrencyTypes` | `AddVirtualCurrencyTypes` / `RemoveVirtualCurrencyTypes` | Additive / destructive |

Sharp edges worth calling out:

- `SetCatalogItems` and `UpdateCatalogItems` take the *same request model* but have opposite
  semantics. `Set` deletes everything not in the payload.
- `SetTitleData` with a null `Value` deletes the key. There is no separate delete API.
- `SetPublishedRevision` changes live Cloud Script for all players immediately.
- `RemoveVirtualCurrencyTypes` leaves player balances in an undefined state if the currency is
  later recreated.
- The Cloud Script and legacy economy APIs are flagged "Legacy, bugfix-only mode" in source.
- Cloud Script Admin APIs additionally require the `LegacyCloudScript` flight to be enabled.

## Why this shape suits agentic work

1. **Ground truth is queryable.** An agent reads the actual stored config rather than inferring
   from a UI. This matters more than it sounds: Game Manager's API Access Policy page renders a
   *missing* statement as "Allow" — the exact inverse of the server's default-deny — so the portal
   showed a fully-allowed Lobby category for a title where every Lobby call was returning 403. The
   API told the truth immediately.
2. **Changes are diffable.** `ValidateApiPolicy` returns a precise add/remove/replace count before
   anything is written, so an agent can assert "exactly 2 statements added, 0 removed" and abort if
   the shape is unexpected.
3. **Writes can be additive.** `OverwritePolicy: false` means a targeted repair doesn't require
   reconstructing the whole document, which is where destructive mistakes come from.
4. **Verification is cheap and end-to-end.** After writing, the agent can log in a throwaway player
   and call the actual API. That closes the loop far more convincingly than re-reading config.
5. **No build, deploy, or repro environment.** Investigation and repair are pure HTTP against the
   live title.

The combination that actually solved the Lobby case: read the real policy, discover the grant was
absent rather than denied, dry-run an additive fix, apply it, re-read and diff, then log in a test
player and successfully call `CreateLobby`.

## Guardrails

- Use a **test title** whenever possible. A dedicated non-production title is safest to modify
  because it isolates any mistakes from live player impact.
- Prefer `OverwritePolicy: false`. Reserve `true` for a deliberate, backed-up rewrite.
- **Back up before writing.** `GetPolicy` output to a file costs nothing and makes the change
  revertible.
- **Dry-run first** and assert on the diff, not just `IsValid`.
- Re-read after writing. `UpdatePolicy` returns an empty `PolicyVersion`, so the write response is
  not sufficient confirmation.
- Never put the secret key in a repo, a commit, or a work item. Environment variable only.
- Serialize writes — there is no concurrency protection.
- Allow up to 10 minutes for the policy cache, and re-authenticate the caller for a fresh token.

## Related

- Public doc: [API access policy](https://learn.microsoft.com/gaming/playfab/api-references/api-access-policy)
  — accurate on statement shape, but predates the versioned default policies. Its claim that the
  default is a single blanket `pfrn:api--*` allow no longer reflects what titles actually get.
- Public reference: [PlayFab Admin API](https://learn.microsoft.com/rest/api/playfab/admin/)
- `ValidateApiPolicy` is not currently in the public reference. Added 2026-03-27, verified live in
  production 2026-08-12.

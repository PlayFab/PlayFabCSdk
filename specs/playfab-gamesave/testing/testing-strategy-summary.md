# PlayFab Game Saves – Manual Testing Summary

## Purpose
Give the PFGameSaves team a clear, prioritized list of what must be manually tested for PlayFab Game Saves.

## Test apps

- Manual: ShamWow (all manual scenarios below assume ShamWow).

 - Automated: PlayFab team–owned multi‑device PFGameSaves automation test app (plan is to run it across Xbox Gen8/Gen9, Windows MSIXVC/Steam, and Steam Deck).

## Manual vs automated – decision principles (for review/debate)

- Automate deterministic behavior.

	- State machines, math, guard codes, concurrency, timing windows, background triggers.

	- Examples: atomic unit enforcement, quota math, background upload timers, error codes, thread safety.

- Keep manual for human perception moments.

	- Copy clarity, visual affordances, pacing (too fast/slow), perceived safety/trust, first‑run flows.

	- Examples: conflict dialog wording, out‑of‑storage messaging, takeover prompts, Steam Deck QR flows.

- Coordinated, not hybrid.

	- Manual and automated are separate apps. Automation can precondition scenarios (faults, contention) and validate codes/logs; manual executes the same scenario to validate UX. We do not mix them in one test.

- Non‑technical tester constraint.

	- No filesystem/manifest spelunking, no dev tools. File structure/descriptor checks are automation‑only.

- Platform/device nuance.

	- If hardware behavior is hard to simulate reliably (suspend, power pull, QR via Deck), keep a manual smoke; expand with targeted automation.

- Tie‑breakers.

	- If both possible, default to automate and keep one manual smoke for UX.

	- If a scenario is already covered by the platform manual tests, do not track it as a gap; only track remaining automation needs in the automation test app.

	- Coverage note: As of this revision, all gaps listed in the Prioritized manual test list are not covered by PFGameSaves_Platform_Test_Cases.md (to avoid double‑tracking).

Quick examples (separate apps, shared scenarios)

- Conflicts: manual validates dialog copy/choices; automation enforces atomic boundaries and “single decision applies” via the automation test app.

- Disk full: manual validates messaging and Retry; automation validates DISK_FULL codes/resume behavior in logs.

- Steam Deck frequent sync: manual perception of “not going stale”; automation validates cadence/timing separately.

- Power/suspend: manual spot checks across devices; automation runs broader timing/permutation sweeps.

- Active device takeover: manual messaging/recovery; automation validates callback firing/order and guard rails.

## Prioritized manual test list (P1 first)
1) Conflict & contention UX (copy, decisions, pacing)

	- [Automation] 6 — Atomic Unit Boundaries: Conflicts only within modified root folders; others auto-merge

	- [Automation] 8 — Partial Folder Merge: Mixed conflict and clean folders resolve independently

	- [Gap] 37 — UI Progress Cancel Response: Cancel halts operation promptly with correct code

	- 40 — UI Out of Storage Retry Response: Free space then Retry resumes successfully

2) AddUser success and offline onboarding

	- [explanation needed] 119 — Offline Mode API Behavior Validation: APIs offline return expected codes or no-op

3) Active device contention/takeover (mid‑gameplay messaging and recovery)

	- [Automation] 45 — Active Device Changed Callback: Takeover fires single accurate descriptor callback

	- [Automation] 115 — Active Device Changed During Gameplay: Mid gameplay takeover pauses and recovers

	- [Automation] 118 — Multiple Device Simultaneous Active Attempts: Race for active state yields single winner

4) Disk full UX and recovery

	- [explanation needed - on how this differs to out of storage response] 36 — E_PF_GAMESAVE_DISK_FULL: Disk exhaustion triggers callback and DISK_FULL

	- [Gap] 40 — UI Out of Storage Retry Response: Free space then Retry resumes successfully

	- [Automation] 94 — Disk Space Changes During Upload: Mid transfer disk exhaustion aborts cleanly

5) Quota exhaustion UX (what to do next)

	- [Gap] 59 — Storage Quota Enforcement (256MB per title): Approach and exceed quota gives negative remainder and block

	- [explanation needed] 144 — Quota Edge Condition Handling: Near quota transitions to block with correct value

6) Exit during in‑process upload (prevention warning)
	- [explanation needed] 109 — Steam Deck In-Process Sync Behavior: Process exit stops sync no background linger

7) Steam Deck auth and frequent‑sync perception
	- [Gap] 111 — Steam Deck Platform Detection: Detection toggles code paths accurately

	- [Automation] 112 — Steam Deck Registry Configuration: Sandbox registry value applied and revert safe

8) Offline progression then reconnect (conflict options clarity)

	- [explanation needed - power mode and low connectivity is already covered] 124 — Partial Network Connectivity: High latency loss shows progress and retries

	- [gap but P1/P2] 125 — Network Interface Switching During Sync: Switch networks resume or fail gracefully

9) Unicode descriptions and thumbnail presentation (visual fit, truncation)

	- [Gap] 50 — Screenshot Thumbnail Integration: Thumbnail creation replacement absence correctness

	- [Gap] 51 — Save Description with Metadata: Descriptions including Unicode propagate to UI

	- [Gap] 141 — Descriptor Boundary & Truncation: Long Unicode descriptor fields truncate safely

	- [Gap] 142 — Long Unicode Save Descriptions: Long emoji RTL descriptions display intact

	- [Gap] 143 — Thumbnail Lifecycle Integrity: Add replace remove thumbnail updates descriptor


## Explanations for [explanation needed] items

### 119 — Offline Mode API Behavior Validation
- What this is: Validate API-level behavior while offline (expected HRESULTs/no-ops, stable state) across PFGameSaveFiles APIs. Includes confirming IsConnected=false and that operations such as Upload/Enum return the documented offline codes without side effects.
- How it differs from PFGameSaves_Platform_Test_Cases.md: The platform tests cover the connectivity prompt UX (Try again / Continue offline), but they don’t validate API return codes or invariants while operating in offline mode.

### 36 — E_PF_GAMESAVE_DISK_FULL (how this differs from out-of-storage response)
- What this is: Intentionally exhaust local disk space during sync to trigger the DISK_FULL error path and out‑of‑storage callback, then verify correct user messaging and resumability after freeing space.
- How it differs from PFGameSaves_Platform_Test_Cases.md: The platform tests do not include a disk‑full scenario. This gap focuses on the specific DISK_FULL callback/error path and recovery, not just a generic “out of storage” retry UI.

### 144 — Quota Edge Condition Handling
- What this is: Drive remainingQuota precisely to 0, then slightly over, and verify boundary behavior (negative remainder, upload blocked, correct values shown) without state drift.
- How it differs from PFGameSaves_Platform_Test_Cases.md: The platform manual tests don’t exercise storage quota math or boundary conditions; this is not covered there.

### 109 — Steam Deck In‑Process Sync Behavior
- What this is: On Steam Deck, terminate the process during an active sync and verify that sync stops immediately (no background continuation) and state is consistent on next launch.
- How it differs from PFGameSaves_Platform_Test_Cases.md: The platform tests cover mid‑game upload flows but do not test process termination behavior or background‑linger specifics on Steam Deck.

### 124 — Partial Network Connectivity (power mode/low connectivity note)
- What this is: Simulate impaired networks (high latency, jitter, packet loss, intermittent Wi‑Fi) and validate progress UI, retry behavior, and timeouts without freezes.
- How it differs from PFGameSaves_Platform_Test_Cases.md: The platform tests cover full disconnect/reconnect and power/suspend cases, but not degraded network conditions (latency/loss) that keep a connection nominally “up.”


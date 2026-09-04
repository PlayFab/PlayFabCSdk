# PFGameSaveAITestDebugLoop — self-driving AI loops (test-debug and code-review)

A single-folder, self-contained tool with **two modes**:

- **`--mode test`** (default) — runs the `gamesave-xbox` suite and, when tests fail, has
  the Copilot CLI debug them (using ETL, logs, and `ConnectedStorage` source), then
  loops — re-running only what it fixed — until the suite is green.
- **`--mode review`** — runs an **iterative multi-model code review**: two (or more)
  models review the SDK **in parallel**, a fixer verifies and fixes what they found, the
  harness rebuilds and re-validates, and it repeats until a round stops finding real
  defects.

Either mode drives any suite `tests-run.py` accepts via `--suite`
(see [Choosing a suite](#choosing-a-suite)); all suite-derived state is kept per-suite.

The **script drives**; the **AI is only invoked when there is work** (a failing test, or a
review round). It calls the Copilot CLI directly and inlines its agent guidance, so it
works for anyone with the Copilot CLI installed — no agent setup, no always-on
agent, no feature flags.

## Files (all in this folder)

| File | Purpose |
|------|---------|
| `loop.py` | The driver for both modes. Builds, runs tests or reviews, invokes the agent, loops. |
| `stop-loop.py` | Writes a stop signal (`<repo>\Out\stop.txt`) to stop a running `loop.py` gracefully. |
| `debugprompt.txt` | **test mode** — the prompt used when tests fail. Defines the `aidebugresult.json` contract. |
| `buildfixprompt.txt` | Both modes — the prompt used when a rebuild fails, to fix the compile errors. |
| `tester.agent.md` | **test mode** — Test-Debug agent guidance. Inlined into the prompt by default (also usable via `--agent`). |
| `reviewprompt.txt` | **review mode** — given to each parallel reviewer. Points at the review spec and defines the report contract. |
| `reviewfixprompt.txt` | **review mode** — given to the fixer. Defines verify → fix → `aireviewresult.json`. |
| `reviewregressionprompt.txt` | **review mode** — given to the agent when a round's fixes break the suite. |
| `reviewer.agent.md` | **review mode** — Code-Review agent guidance (reviewer and fixer roles). |
| `reviewexclusions.txt` | **review mode** — standing "never report this" list. **Edit this** as findings get declined. |
| `README.md` | This file. |

Paths are computed relative to this script's location: the repo root is two levels
up (`...\PlayFab.C`), so the tool moves with the repo. It drives the existing test
infra in-place:

- Runner: `<repo>\Utilities\Scripts\tests-run.py`
- Build:  `<repo>\Utilities\Scripts\tests-build.ps1`
- Results: `<repo>\Out\gamesave-xbox-tests\` (`testresult.txt`, `aidebugresult.json`, `buildresult.txt`)
- Investigations (platform/blocked bugs the agent logs): `<repo>\Out\Investigations\`
- Platform source (read-only reference): `C:\git\ConnectedStorage`
- Bootstrap guide (project context the agent is primed with): `<repo>\specs\playfab-gamesave\ai-bootstrap.md`

## Prerequisites

- **GitHub Copilot CLI** (`copilot` on PATH)
- **Python 3.10+** (the loop is stdlib-only)

## Run it

```powershell
cd C:\git\PlayFab.C\Test\PFGameSaveAITestDebugLoop
py loop.py                 # test mode (default)
py loop.py --mode review   # iterative code-review mode
```

## Choosing a suite

`--suite` accepts anything `tests-run.py` does — run `py ..\..\Utilities\Scripts\tests-run.py --list`
to see them all:

```powershell
py loop.py                            # gamesave-xbox (default) — needs a devkit + signed-in Xbox user
py loop.py --suite gamesave-inproc    # in-proc GameSave suite — runs entirely on the PC
py loop.py --suite gamesave-pc        # PC GRTS suite
py loop.py --suite lhc                # libHttpClient scenarios
py loop.py --mode review --suite pfcore   # review mode validates against this suite
```

An unknown suite is rejected immediately, before the up-front build, with the list
of valid names.

Everything suite-derived is kept **per-suite**, so suites never clobber each other
and you can alternate between them freely:

| Per-suite state | Path |
|---|---|
| Results / logs | `<repo>\Out\<suite>-tests\` |
| Loop summary | `<repo>\Out\<suite>-tests\testresult.txt` |
| AI contract | `<repo>\Out\<suite>-tests\aidebugresult.json` |
| Primed session cache | `.loop-session-<suite>.json` (this folder) |

The primed-session cache is per-suite deliberately: the loop resumes one Copilot
session across iterations, so the agent accumulates history about specific failing
tests. Sharing it would carry the wrong suite's tests into the next run's context.

## Test mode (`--mode test`)

What it does:

1. **Build up front** — runs `tests-build.ps1` so the first run uses current
   binaries (skip with `--skip-build`).
2. **Run tests** — `tests-run.py gamesave-xbox` (only the AI's rerun subset on
   later iterations).
3. **Write the summary** — parses the run's `pass<N>\summary.csv` and writes
   `Out\gamesave-xbox-tests\testresult.txt`.
4. **All pass?** → done.
5. **Failures?** → **prime the agent with project context** (first time only), then
   invoke it non-interactively:
   ```
   copilot -p "<tester.agent.md + debugprompt.txt>" --allow-all-tools --allow-all-paths --resume <primed-session>
   ```
   The agent fixes test issues, logs platform/blocked bugs under `Out\Investigations\`,
   and writes `Out\gamesave-xbox-tests\aidebugresult.json`.
6. **Rebuild if needed** — if the agent set `"rebuild": true` (it changed compiled
   C++/C# test code), rebuild before re-running. A failing build is handed back to
   the agent to fix and retried (up to 3 attempts).
7. **Loop** — re-run only the tests the agent fixed.

Stops when: all tests pass, the `rerun` list is empty (remaining failures are
platform issues or blocked items the agent logged), the same set of tests fails
`--max-stall` times in a row, the build can't be fixed after 3 attempts, or
`--max-iterations` is hit.

Before building/running, the loop preflights that the Copilot CLI is available; if
not, it logs why and stops immediately.

## Review mode (`--mode review`)

```powershell
py loop.py --mode review                 # full loop: review -> fix -> build -> validate
py loop.py --mode review --skip-tests    # review + build only (much faster)
```

Each **round**:

1. **Review, in parallel** — one Copilot process per model (`--review-models`, default
   `gpt-5.6-sol,claude-opus-5`). Each follows the review specification at
   `specs\playfab-gamesave\ai-codereview-prompt.md` and writes its own report to
   `specs\playfab-gamesave\ai-code-review\code-review-<DATE>-<N>.md`. Reviewers do not
   modify code and do not share a session. Per-reviewer console output goes to
   `<Out>\<suite>-tests\review-<model>.log`.
2. **Fix, once** — the fixer gets both reports and must **verify each finding against
   current source** before changing anything, de-duplicate across the two reports, fix
   the real ones, defer what needs a design change, and write
   `<Out>\<suite>-tests\aireviewresult.json`.
3. **Rebuild** — a failing build is handed back to the agent to fix (up to 3 attempts),
   same as test mode.
4. **Validate** — runs the suite (`--suite`, default `gamesave-inproc`) so a "fix" that
   breaks behavior is caught immediately. Skip with `--skip-tests`.
5. **Repair regressions** — if the round's fixes broke tests, the failures go straight
   back to the agent (`reviewregressionprompt.txt`, which is told exactly what the round
   changed): fix → rebuild → re-run the failing tests → re-validate the full suite, up to
   `--max-regression-attempts` (default 3). A round that lands a real fix *and* a
   regression is normal; the loop only stops for a human if the repair can't converge or
   stalls.
6. **Loop** — findings already fixed are injected into the next round's reviewer prompt
   as *"already fixed, do not re-report"*, and findings deliberately **deferred** are
   injected as *"known, deferred, do not re-report"*. Both matter: a deferred defect is
   still in the source, so without feeding it back the reviewers re-find it every round
   and it keeps counting toward `unique_findings` — which puts a floor under the
   convergence number equal to the deferred count.

**Converges** when a round reports fewer than `--converge-threshold` unique real
findings (default 3). Also stops on `--max-iterations`, a failing validation run, an
unfixable build, or a stop signal.

> **Why two models?** In practice the two families find materially different defects,
> and each one repeatedly caught bugs in the *other's* fixes. Agreement between them is
> a strong signal a finding is real — but the fixer still verifies every finding,
> because both models do produce false positives.

### Result contract (`aireviewresult.json`)

```json
{
  "unique_findings": 4,
  "fixed":    [{ "id": "CR-1", "severity": "Critical", "file": "...", "summary": "one line" }],
  "rejected": [{ "id": "II-3", "reason": "not a real defect because ..." }],
  "deferred": [{ "id": "CR-2", "reason": "needs a design change", "suggest_exclusion": true }],
  "needs_build": true,
  "notes": "optional"
}
```

`unique_findings` is the convergence signal: distinct **real** defects after
de-duplication, i.e. `fixed` + `deferred`. Because deferred items count, they are fed back
to the next round's reviewers as "do not re-report" — otherwise the loop can never drop
below a threshold smaller than the number of things it has deferred.

### Keeping the signal clean

- **`reviewexclusions.txt`** — standing "never report this" entries (declined findings,
  accepted design limitations). Without it, the same known issues crowd out new ones
  every round. When the fixer defers something with `"suggest_exclusion": true`, the
  loop prints it as an exclusion candidate — decide, then edit the file.
- **`.loop-review-state.json`** (gitignored) — accumulated fix summaries, so a *later*
  run of `loop.py` continues where the last one stopped instead of re-reporting old
  fixes. Clear it with `--reset-review-state`.

### Reviewing something else

The reviewers follow `--review-spec` **exactly** — its severity bar, output format and scope
exclusions — and it defaults to the PFGameSave spec. So pointing `--review-scope` at another
component means moving the spec and the report folder with it, otherwise the reviewers get
gamesave instructions and file their reports in the gamesave folder:

```powershell
# Reviewing another component: move the spec and report folder too.
py loop.py --mode review --suite pfcore `
  --review-scope "Source\PlayFabCore" `
  --review-spec  "..\..\specs\playfab-core\ai-codereview-prompt.md" `
  --review-dir   "..\..\specs\playfab-core\ai-code-review"

py loop.py --mode review --review-models gpt-5.6-sol,claude-opus-5,gemini-3.1-pro-preview
py loop.py --mode review --converge-threshold 1 --max-iterations 6
```

The accumulated "already fixed / already deferred" history is keyed off `--review-scope`
(`.loop-review-state-<area>.json`), so each area keeps its own and one component's history is
never presented to another as "already reviewed".

### Continuing a review that earlier rounds already advanced

Round numbers are bookkeeping, but they show up in the logs, in each reviewer's label
(`claude-opus-5 (round 7)`) and in the state file — so a continuation should keep
counting rather than restart at 1:

```powershell
py loop.py --mode review --start-round 7 `
           --prior-fixes-file C:\git\PlayFab.C\specs\playfab-gamesave\ai-code-review\iterative-review-plan.md
```

- **`--start-round N`** — first round number to use. `--max-iterations` still bounds how
  many rounds run (above: 7, 8, 9, …).
- **`--prior-fixes-file PATH`** — the text is injected into every reviewer prompt as
  *"already fixed, do not re-report"*. Use it whenever the earlier rounds weren't driven
  by this loop (or the state file was cleared) — otherwise those reviewers have no idea
  what was already fixed and will report it all again. The loop warns when you start
  mid-loop with no fix history and no seed file.

## Pre-loaded project context (bootstrap)

So the AI starts each debug run already knowing the PFGameSave SDK, the loop
**primes** it with project context before it debugs: it follows the AI bootstrap
guide `specs\playfab-gamesave\ai-bootstrap.md` and reads every doc that guide lists
(ai-summary, ai-architecture, ai-source-map, the dev spec, the test-loop guide, …).

- **`--context-mode prime`** (default) — follow the guide **once** in a session, then
  `--resume` that session for each debug / build-fix call. Context loads once, the
  per-turn prompt stays small, and the agent remembers what it already investigated
  across iterations. (Headless equivalent of "read the bootstrap guide, snapshot the
  session, and debug from there" — the interactive `/fork` isn't available under
  `copilot -p`, but `--session-id` + `--resume` achieve the same.)
- **`--context-mode inline`** — prepend the bootstrap guide to every debug prompt
  (stateless; re-reads the docs each iteration).
- **`--context-mode off`** — no bootstrap context (tester.agent.md guidance only).

Priming is **lazy** — a green suite never pays for it (it runs only when the agent is
first actually needed). If priming fails, the loop falls back to `inline`.

> **Review mode:** the **fixer** uses the primed session like any other agent call, but
> the **reviewers do not** — they run concurrently, and two processes resuming the same
> session would collide. Reviewers get their context from the review specification
> (`ai-codereview-prompt.md`), which is self-contained by design, so each one starts from
> the same clean baseline rather than inheriting another round's conclusions.

**The primed session id is remembered across runs.** After the first prime it's saved
to `.loop-session.json` (gitignored) in this folder, so subsequent `py loop.py` runs
**resume the already-primed session** instead of re-reading all the bootstrap docs.
The loop automatically re-primes a fresh session when:

- the bootstrap guide (`ai-bootstrap.md`) changes (a content fingerprint is stored), or
- you pass **`--refresh-context`** (use this after the underlying docs change materially), or
- resuming the saved session fails because it's stale/deleted (it re-primes once and retries).

The new session id is saved again each time, so you keep resuming the latest primed session.

## Soak / flakiness runs

To run the whole process repeatedly (e.g. to catch flaky failures), use
`--repeat N` — it re-runs the entire build → test → AI-debug flow N times (each run
starts fresh: its own build, iterations, and stall tracking), then prints a pass/fail
summary across runs. `--repeat 0` loops forever until interrupted. Project-context
priming is shared across repeats, so the bootstrap docs are read at most once.

## Options

| Flag | Default | Meaning |
|------|---------|---------|
| `--mode test\|review` | `test` | Loop on failing tests, or on an iterative multi-model code review. |
| `--suite NAME` | `gamesave-xbox` (test), `gamesave-inproc` (review) | Test suite to run, as accepted by `tests-run.py` (`gamesave-inproc`, `gamesave-pc`, `lhc`, ...). In review mode this is the validation suite. Results, the AI contract and the primed-session cache are kept per-suite; an unknown name is rejected before the up-front build. |
| `--agent NAME` | (inlined) | Use an installed Copilot agent instead of inlining `tester.agent.md` / `reviewer.agent.md`. |
| `--context-mode MODE` | `prime` | Pre-load project context: `prime` (follow ai-bootstrap.md once, resume per run), `inline`, or `off`. |
| `--refresh-context` | off | Ignore the saved primed session and re-prime a fresh one (after the bootstrap docs change). |
| `--only 65,81` | (whole suite) | **test mode.** Test IDs to **focus** on. Persists for the entire loop: only these run every iteration, and the agent's rerun list is intersected with this set so the loop never wanders outside it. |
| `--all` | off | **test mode.** Re-run the full suite every iteration instead of the rerun subset. With `--only`, re-runs the focus set every iteration. |
| `--no-captureetl` | ETL on | Skip ETL capture (faster runs, but the agent has less to debug). |
| `--timeout SEC` | runner default | Per-test timeout, passed to `tests-run.py`. |
| `--agent-timeout SEC` | `3600` | Max seconds for the agent per phase (each debug pass; each review round's reviewers; each fix pass). |
| `--max-iterations N` | `10` | Safety cap on test iterations / review rounds. |
| `--max-stall N` | `3` | **test mode.** Stop if the exact same set of tests fails N iterations in a row. |
| `--skip-build` | off | **test mode.** Skip the initial up-front build. |
| `--skip-preflight` | off | Skip the up-front Copilot-availability check. |
| `--repeat N` | `1` | Run the **entire** process N times in a row (soak / flakiness). `0` repeats forever until Ctrl-C. Each run starts fresh. |
| `--review-models A,B` | `gpt-5.6-sol,claude-opus-5` | **review mode.** One parallel reviewer per model. |
| `--review-scope TEXT` | PFGameSave + Compression.cpp | **review mode.** What the reviewers audit (free text, injected into the prompt). Also keys the per-area "already fixed" history. |
| `--review-spec PATH` | PFGameSave spec | **review mode.** The specification reviewers follow exactly. Override when `--review-scope` points at another component. |
| `--review-dir PATH` | PFGameSave `ai-code-review` | **review mode.** Where per-round reports are written. Override alongside `--review-spec`. |
| `--converge-threshold N` | `3` | **review mode.** Stop when a round yields fewer than N unique real findings. |
| `--start-round N` | (continue state, else 1) | **review mode.** Round number to start counting from (logs, reviewer labels, state file). |
| `--prior-fixes-file PATH` | none | **review mode.** Text injected into reviewer prompts as "already fixed, do not re-report". Pair with `--start-round` when continuing work this loop didn't drive. |
| `--skip-tests` | off | **review mode.** Build after each round but skip the validation suite. |
| `--max-regression-attempts N` | `3` | **review mode.** AI repair passes to attempt when a round's fixes break the suite, before stopping for a human. |
| `--reset-review-state` | off | **review mode.** Forget the accumulated "already fixed" history before starting. |

## Stopping the loop gracefully

To stop a running `loop.py` without killing it mid-test, from another shell run:

```
py C:\git\PlayFab.C\Test\PFGameSaveAITestDebugLoop\stop-loop.py
```

This writes a signal file at `<repo>\Out\stop.txt`. The loop finishes the current
iteration **and lets the agent debug that iteration's failures**, then deletes the
signal and exits cleanly. The same signal is honored by `tests-run.py`, which stops
after the current test finishes rather than running the rest of the suite.

- `py stop-loop.py --clear` — cancel a pending stop (delete the signal).
- `py stop-loop.py --status` — report whether a stop is currently pending.

`loop.py` clears any stale `stop.txt` at startup, so a leftover signal from a
previously killed run won't stop a fresh run. When `loop.py` is driving
`tests-run.py`, it owns the signal's lifetime (the child leaves the file for the
parent to consume); a standalone `tests-run.py` deletes the signal itself.

## The result contract — `aidebugresult.json`

The agent writes valid JSON of this shape (see `debugprompt.txt` for full rules):

```json
{
  "rerun": ["18", "20"],
  "rebuild": false,
  "resolved": [ { "test_id": "18", "classification": "test-issue", "action": "..." } ],
  "platform_issues": [ { "test_id": "20", "investigation": "...", "new_etl_copied": true } ],
  "blocked": [ { "test_id": "22", "investigation": "...-blocked", "needs": "..." } ],
  "notes": "optional"
}
```

- **`rerun`** — test IDs the agent fixed and wants re-run. Platform issues and
  blocked items are **not** re-run.
- **`rebuild`** — `true` when the agent changed compiled C++/C# test code, so the
  loop rebuilds before re-running.
- **`blocked`** — test issues the agent could not solve and logged for human
  triage (in `Out\Investigations\` folders whose names end in `-blocked`). The loop
  reports the blocked count when it converges.

## Notes

- Investigations land in `<repo>\Out\Investigations\`.
- The loop parses `summary.csv` (not console output) and measures wall-clock time
  itself for the `Time:` line in `testresult.txt`.
- Everything is self-contained in this one folder; delete it to remove the tool.

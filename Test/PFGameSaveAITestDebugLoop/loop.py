#!/usr/bin/env python3
r"""
PFGameSaveAITestDebugLoop / loop.py

Self-driving AI loops for the PlayFab Game Save SDK. Two modes, one driver:

  --mode test    (default)  test -> AI-debug -> re-test, until the suite is green
  --mode review             parallel multi-model code review -> AI-fix -> build/validate,
                            until a round stops finding real defects

Both modes drive Copilot directly (no agent-mesh), so they work for anyone with the
Copilot CLI — no special feature flags or always-on agent required. The agent guidance
(tester.agent.md / reviewer.agent.md) is inlined into the prompt, so no repo agent setup
is needed; pass --agent NAME to use an installed agent instead. Context priming is lazy
(only when the agent is first needed) and controlled by --context-mode
(prime | inline | off; default prime).

TEST MODE
---------
First it builds the test harness (tests-build.ps1) so the initial run uses current
binaries (skip with --skip-build). Then each iteration:
  1. Runs   C:\git\PlayFab.C\Utilities\Scripts\tests-run.py gamesave-xbox
     (optionally only a subset of test IDs on later iterations).
  2. Parses the run's pass<N>\summary.csv and writes a human-readable summary to
     C:\git\PlayFab.C\Out\gamesave-xbox-tests\testresult.txt  (SUMMARY + FAILURES).
  3. If there are no failures -> done (all pass).
  4. Otherwise PRIMES the agent with project context (follows the AI bootstrap guide
     specs\playfab-gamesave\ai-bootstrap.md once, reading every doc it lists, in a
     session), then runs the Copilot 'tester' agent non-interactively to debug them,
     resuming that primed session:
        copilot -p "<debugprompt.txt contents>" --allow-all-tools --allow-all-paths --resume <primed>
     The agent fixes test issues / logs platform bugs, then writes
        C:\git\PlayFab.C\Out\gamesave-xbox-tests\aidebugresult.json
  5. Reads aidebugresult.json's "rerun" list and loops again (re-running just
     those tests). If the agent set "rebuild": true (it changed compiled test
     code), the loop first runs tests-build.ps1. If any build FAILS, the build
     output is handed back to the agent to fix and retried, up to 3 times, before
     halting. Stops when all tests pass, when "rerun" is empty (only unfixable
     platform issues remain), on no progress (--max-stall), or --max-iterations.

REVIEW MODE
-----------
Each round:
  1. Starts ONE REVIEWER PER MODEL, in PARALLEL (--review-models, default
     gpt-5.6-sol + claude-opus-5). Each follows the review specification at
     specs\playfab-gamesave\ai-codereview-prompt.md and writes its own report to
     specs\playfab-gamesave\ai-code-review\code-review-<DATE>-<N>.md. Reviewers do
     not modify code, and do not share a session (they run concurrently).
     Two different model families find materially different defects — that is the
     reason to pay for two passes rather than one.
  2. Runs the FIXER once, with both reports. It verifies every finding against the
     current source (reviewers do produce false positives), de-duplicates across the
     two reports, fixes the real ones, defers what needs a design change, and writes
        <Out>\<suite>-tests\aireviewresult.json
  3. Rebuilds (a failing build is handed back to the agent, same as test mode) and,
     unless --skip-tests, runs the validation suite so a "fix" that breaks behavior
     is caught immediately. If the round's fixes DID break tests, the failures are
     handed straight back to the agent to repair (fix -> rebuild -> re-run, up to
     --max-regression-attempts), then the full suite is re-validated. The loop only
     stops for a human if that repair can't converge.
  4. Repeats. Findings the fixer already fixed are fed into the next round's prompt as
     "already fixed, do not re-report", persisted in .loop-review-state.json so a later
     run continues where this one stopped. Standing "never report this" entries live in
     reviewexclusions.txt.
  Converges when a round produces fewer than --converge-threshold unique real findings
  (default 3).


Usage (from anywhere):
    py C:\git\PlayFab.C\Test\PFGameSaveAITestDebugLoop\loop.py
    py loop.py --max-iterations 8
    py loop.py --only 18,20        # focus the WHOLE loop on just these test IDs
    py loop.py --all              # re-run the full suite (or the --only focus set) every iteration
    py loop.py --no-captureetl    # skip ETL capture (faster, less to debug)
    py loop.py --repeat 5         # run the whole build/test/debug process 5 times
    py loop.py --repeat 0         # run the whole process forever (until Ctrl-C)

    py loop.py --mode review                      # dual-model review loop until it converges
    py loop.py --mode review --skip-tests         # review + build only (much faster)
    py loop.py --mode review --review-models gpt-5.6-sol,claude-opus-5,gemini-3.1-pro-preview
    py loop.py --mode review --review-scope "Source\PlayFabCore" --suite pfcore
    py loop.py --mode review --converge-threshold 1 --max-iterations 6
    py loop.py --mode review --start-round 7 --prior-fixes-file <plan.md>   # continue earlier rounds

To stop a running loop gracefully, run stop-loop.py from another shell:
    py C:\git\PlayFab.C\Test\PFGameSaveAITestDebugLoop\stop-loop.py
In test mode the loop finishes the current iteration AND lets the agent debug its
failures; in review mode it finishes the current round (review + fix + validate). Then
it deletes the signal and exits. (stop-loop.py --clear cancels a pending stop.)
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import importlib.util
import json
import os
import re
import shutil
import subprocess
import sys
import time
import uuid
from datetime import datetime, timezone
from pathlib import Path

# ── Fixed locations (all derived from this file's location — no hardcoded roots) ─
# This script lives at <PlayFab.C>\Test\PFGameSaveAITestDebugLoop\loop.py, so the
# repo root is two levels up. Everything is computed relative to that.
SCRIPT_DIR = Path(__file__).resolve().parent
PLAYFAB_ROOT = SCRIPT_DIR.parents[1]            # ...\PlayFab.C
TESTS_RUN = PLAYFAB_ROOT / "Utilities" / "Scripts" / "tests-run.py"
TESTS_BUILD = PLAYFAB_ROOT / "Utilities" / "Scripts" / "tests-build.ps1"
SUITE = "gamesave-xbox"
OUT_DIR = PLAYFAB_ROOT / "Out" / f"{SUITE}-tests"
RESULT_TXT = OUT_DIR / "testresult.txt"
AIDEBUG_JSON = OUT_DIR / "aidebugresult.json"
BUILD_RESULT_TXT = OUT_DIR / "buildresult.txt"
AIREVIEW_JSON = OUT_DIR / "aireviewresult.json"   # review mode: fixer's machine-readable result
INVESTIGATIONS = PLAYFAB_ROOT / "Out" / "Investigations"   # agent logs platform/blocked issues here
# Cooperative stop signal (written by stop-loop.py). loop.py owns its lifetime: it
# tells the child tests-run.py to leave the file alone (env var below), finishes the
# current iteration INCLUDING the AI debug of its failures, then deletes the file and
# exits. Path matches tests-run.py's STOP_FILE (repo Out\stop.txt).
STOP_FILE = PLAYFAB_ROOT / "Out" / "stop.txt"
# Set to True when a run stops because of the cooperative stop signal, so the
# --repeat wrapper exits entirely instead of starting another process run.
_STOP_STATE = {"stopped": False}
CONNECTEDSTORAGE = PLAYFAB_ROOT.parent / "ConnectedStorage"  # sibling repo (read-only reference)
TEST_APP = PLAYFAB_ROOT / "Test" / "GameTestAppWindows"      # C++ test device
TEST_CONTROLLER = PLAYFAB_ROOT / "Test" / "GameTestController"  # C# test controller

# The AI bootstrap guide the agent follows to load durable project context before
# debugging. It lists the docs to read (ai-summary, ai-architecture, ai-source-map,
# dev spec, test-loop guide, ...). The agent reads all of them during priming.
AI_BOOTSTRAP = PLAYFAB_ROOT / "specs" / "playfab-gamesave" / "ai-bootstrap.md"

# Bundle files live in the SAME folder as this script (flat layout).
DEBUG_PROMPT = SCRIPT_DIR / "debugprompt.txt"
BUILDFIX_PROMPT = SCRIPT_DIR / "buildfixprompt.txt"
AGENT_PROFILE = SCRIPT_DIR / "tester.agent.md"       # inlined as a preamble when --agent is not used

# ── Review-mode bundle (--mode review) ────────────────────────────────────
# Same shape as the test-mode files above: a prompt per phase plus an agent profile
# that is inlined when --agent is not supplied.
REVIEW_PROMPT = SCRIPT_DIR / "reviewprompt.txt"            # given to each parallel reviewer
REVIEW_FIX_PROMPT = SCRIPT_DIR / "reviewfixprompt.txt"     # given to the fixer after both reviews
REVIEW_REGRESSION_PROMPT = SCRIPT_DIR / "reviewregressionprompt.txt"  # when review fixes break tests
REVIEW_EXCLUSIONS = SCRIPT_DIR / "reviewexclusions.txt"    # standing "do not report" list (editable)
REVIEWER_PROFILE = SCRIPT_DIR / "reviewer.agent.md"
# Review-mode locations. These default to the gamesave spec folder because that is what the
# review specification and the accumulated reports currently live in, but they are NOT
# gamesave-only: --review-scope can point the reviewers at any area (e.g. Source\PlayFabCore),
# and the spec, the report folder and the "already fixed" history must move with it. Overridden
# by --review-spec / --review-dir, and REVIEW_STATE_FILE is re-keyed by configure_review_area().
REVIEW_SPEC = PLAYFAB_ROOT / "specs" / "playfab-gamesave" / "ai-codereview-prompt.md"
# Where the per-round markdown reports are written (same convention the spec documents).
REVIEW_DIR = PLAYFAB_ROOT / "specs" / "playfab-gamesave" / "ai-code-review"
# Accumulated "already fixed" summaries, persisted so a later run doesn't re-report
# what an earlier run already fixed. Re-keyed per review area by configure_review_area().
REVIEW_STATE_FILE = SCRIPT_DIR / ".loop-review-state.json"
DEFAULT_REVIEW_MODELS = "gpt-5.6-sol,claude-opus-5"
DEFAULT_REVIEW_SCOPE = (
    r"Source\PlayFabGameSave\ (excluding Source\Generated\), plus the shared "
    r"Source\PlayFabSharedInternal\Source\Compression.cpp. Skip Test\, Samples\, External\ and packages\."
)

# Prompt files use {{TOKENS}} instead of hardcoded absolute paths; loop.py fills
# these in at runtime from the (relative-derived) locations above.
PROMPT_VARS = {
    "REPO": PLAYFAB_ROOT,
    "OUT": OUT_DIR,
    "TESTRESULT": RESULT_TXT,
    "AIDEBUG": AIDEBUG_JSON,
    "BUILDRESULT": BUILD_RESULT_TXT,
    "INVESTIGATIONS": INVESTIGATIONS,
    "CONNECTEDSTORAGE": CONNECTEDSTORAGE,
    "TESTS_BUILD": TESTS_BUILD,
    "TEST_APP": TEST_APP,
    "TEST_CONTROLLER": TEST_CONTROLLER,
    "AIBOOTSTRAP": AI_BOOTSTRAP,
    "AIREVIEW": AIREVIEW_JSON,
    "REVIEW_SPEC": REVIEW_SPEC,
    "REVIEW_DIR": REVIEW_DIR,
}


def configure_suite(suite: str, mode: str = "test") -> None:
    """Point the loop at a different test suite. The out dir and every artifact path
    derive from the suite name, and the prompts reference them by token, so this keeps
    prompts and code in sync when review mode validates against a different suite."""
    global SUITE, OUT_DIR, RESULT_TXT, AIDEBUG_JSON, BUILD_RESULT_TXT, AIREVIEW_JSON, SESSION_FILE
    SUITE = suite
    OUT_DIR = PLAYFAB_ROOT / "Out" / f"{SUITE}-tests"
    RESULT_TXT = OUT_DIR / "testresult.txt"
    AIDEBUG_JSON = OUT_DIR / "aidebugresult.json"
    BUILD_RESULT_TXT = OUT_DIR / "buildresult.txt"
    AIREVIEW_JSON = OUT_DIR / "aireviewresult.json"
    # Keyed by suite AND mode. The loop resumes one Copilot session across iterations, so the
    # agent accumulates history: which tests are failing (test mode) or which findings are
    # already dispositioned (review mode). Sharing one file would carry the wrong suite's
    # tests -- or the wrong mode's history and role instructions -- into the next run.
    SESSION_FILE = SCRIPT_DIR / f".loop-session-{mode}-{SUITE}.json"
    PROMPT_VARS.update({
        "SUITE": SUITE,
        "OUT": OUT_DIR,
        "TESTRESULT": RESULT_TXT,
        "AIDEBUG": AIDEBUG_JSON,
        "BUILDRESULT": BUILD_RESULT_TXT,
        "AIREVIEW": AIREVIEW_JSON,
    })


def configure_review_area(spec: Path | None, review_dir: Path | None, area_key: str) -> None:
    """Point review mode at a spec, a report folder and an 'already fixed' history.

    The reviewers are told to follow REVIEW_SPEC exactly -- including its scope exclusions
    and codebase context -- so reviewing a different area with the gamesave spec hands them
    the wrong instructions, files the reports in the wrong folder, and (worst) accumulates
    one area's fixed/deferred history into another's, where it reads as "already fixed, do
    not re-report" for defects that were never looked at.

    'area_key' is derived from the review scope so each area keeps its own state file, for
    the same reason SESSION_FILE is keyed by suite and mode."""
    global REVIEW_SPEC, REVIEW_DIR, REVIEW_STATE_FILE
    if spec is not None:
        REVIEW_SPEC = spec
    if review_dir is not None:
        REVIEW_DIR = review_dir
    REVIEW_STATE_FILE = SCRIPT_DIR / f".loop-review-state-{area_key}.json"
    PROMPT_VARS.update({
        "REVIEW_SPEC": REVIEW_SPEC,
        "REVIEW_DIR": REVIEW_DIR,
    })


def review_area_key(scope: str) -> str:
    """A short, filesystem-safe key identifying the reviewed area.

    Derived from the scope text rather than the suite: the suite is the *validation* suite
    and does not have to correspond to the code under review (the default review scope is
    PlayFabGameSave while the default validation suite is gamesave-inproc). Falls back to a
    hash when the scope has no recognizable Source\\<area> path, so the key is always stable
    for a given scope string."""
    m = re.search(r"Source[\\/]+([A-Za-z0-9_.-]+)", scope or "")
    if m:
        return m.group(1).lower()
    digest = hashlib.sha256((scope or "default").encode("utf-8")).hexdigest()[:8]
    return f"scope-{digest}"


def known_suites() -> list[str]:
    """Suite names tests-run.py accepts, or [] if they can't be determined.

    Read straight out of tests-run.py so this never drifts from the real list.
    Importing it is safe: it only defines constants/functions at module scope and
    guards its entry point with __main__. Any failure degrades to no validation --
    tests-run.py still rejects a bad name, just later.
    """
    try:
        spec = importlib.util.spec_from_file_location("_pfgs_tests_run", TESTS_RUN)
        if spec is None or spec.loader is None:
            return []
        mod = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(mod)
        return sorted(set(getattr(mod, "SUITES", {})) | set(getattr(mod, "ALIASES", {})))
    except Exception:
        return []


def validate_suite(suite: str) -> bool:
    """Reject an unknown suite before the loop spends minutes on a build.

    tests-run.py also auto-discovers any Test/GameTestScenarios/<name> directory,
    so accept that too rather than rejecting a valid suite this list doesn't name.
    """
    suites = known_suites()
    if not suites:
        return True  # couldn't introspect; let tests-run.py be the judge
    if suite in suites or (PLAYFAB_ROOT / "Test" / "GameTestScenarios" / suite).is_dir():
        return True
    log(f"ERROR: unknown suite '{suite}'.")
    log(f"       known suites: {', '.join(suites)}")
    log(f"       (or any directory under {PLAYFAB_ROOT / 'Test' / 'GameTestScenarios'})")
    return False


def render_prompt(text: str, extra: dict | None = None) -> str:
    """Substitute {{TOKEN}} placeholders with the computed paths. 'extra' adds
    per-invocation tokens (used by review mode for the per-reviewer report path,
    label, scope, exclusions and prior-fix list)."""
    for name, value in PROMPT_VARS.items():
        text = text.replace("{{" + name + "}}", str(value))
    for name, value in (extra or {}).items():
        text = text.replace("{{" + name + "}}", str(value))
    return text


# ── Project-context priming ───────────────────────────────────────────────
# The agent is "read-loaded" with project context before it debugs, by following
# the AI bootstrap guide (AI_BOOTSTRAP) and reading every doc it references. Modes:
#   "prime"  — follow the guide ONCE in a session (fixed --session-id), then
#              --resume that session for each debug/build-fix call. Loads context
#              once; smaller per-turn prompts; continuity across iterations.
#   "inline" — prepend the guide's text to every debug/build-fix prompt (stateless).
#   "off"    — no bootstrap context (tester.agent.md guidance only).
# Priming is lazy (only when the agent is first needed) and falls back to "inline"
# if the one-time priming call fails.
#
# The primed session id is PERSISTED to SESSION_FILE and reused across runs of
# loop.py, so subsequent runs resume the already-primed session instead of
# re-reading all the bootstrap docs. It is re-primed automatically when the
# bootstrap guide changes, on --refresh-context, or if resuming the saved session
# fails (stale/deleted).
# Placeholder only: configure_suite() rewrites this once --suite/--mode are parsed, so
# the real file is always keyed by both. Kept so the module is importable standalone.
SESSION_FILE = SCRIPT_DIR / f".loop-session-test-{SUITE}.json"
PRIME_STATE = {
    "mode": "prime",       # prime | inline | off
    "session_id": None,
    "done": False,         # session resolved (primed or loaded) this run
    "from_disk": False,    # session_id came from SESSION_FILE (unverified this run)
    "reprimed": False,     # guard: auto-reprime at most once per run
    "force_fresh": False,  # --refresh-context: ignore the saved session and re-prime
}

DEFAULT_PRIME_PROMPT = (
    "You are about to help debug failing gamesave-xbox tests for the PlayFab Game Save "
    "SDK. First, load durable project context so later debugging is fast and accurate:\n"
    "Follow the AI bootstrap guide at {{AIBOOTSTRAP}} EXACTLY — read, in the order it "
    "lists, every context source doc it references (ai-summary, ai-architecture, "
    "ai-source-map, the dev spec, the test-loop guide, and the rest). Actually open and "
    "read each doc; don't skip any.\n"
    "Also skim the read-only platform source at {{CONNECTEDSTORAGE}} and the test code "
    "at {{TEST_APP}} and {{TEST_CONTROLLER}} enough to know where things live.\n"
    "Build a durable mental model of the architecture and the test harness. Do NOT change "
    "any files yet — you'll get specific test failures to debug in follow-up messages. "
    "Reply with 'primed' when you have finished reading."
)


def _bootstrap_fingerprint() -> str:
    """Short hash of the bootstrap guide, so a changed guide invalidates the saved
    session (top-level guide only; use --refresh-context if referenced docs change)."""
    try:
        return hashlib.sha256(AI_BOOTSTRAP.read_bytes()).hexdigest()[:16]
    except OSError:
        return ""


def load_persisted_session() -> str | None:
    """Return the saved primed session id, or None if absent/invalid/guide-changed."""
    if not SESSION_FILE.is_file():
        return None
    try:
        data = json.loads(SESSION_FILE.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, OSError):
        return None
    sid = data.get("session_id")
    if not sid:
        return None
    if data.get("bootstrap_fingerprint") != _bootstrap_fingerprint():
        log("bootstrap guide changed since the primed session was saved; will re-prime.")
        return None
    return sid


def save_persisted_session(sid: str) -> None:
    try:
        SESSION_FILE.write_text(json.dumps({
            "session_id": sid,
            "bootstrap": str(AI_BOOTSTRAP),
            "bootstrap_fingerprint": _bootstrap_fingerprint(),
            "saved_at": datetime.now(timezone.utc).isoformat(),
        }, indent=2), encoding="utf-8")
        log(f"saved primed session id to {SESSION_FILE.name} (reused on future runs).")
    except OSError as e:
        log(f"warning: could not persist session id ({e}).")


MAX_BUILD_ATTEMPTS = 3  # build -> agent-fix cycles before halting

BAR = "=" * 60
PASS_STATUSES = ("PASS", "SKIP")  # everything else counts as a failure


# Make console output robust on non-UTF-8 terminals (e.g. Windows cp1252), so a
# stray non-ASCII character in a log line can never crash the loop.
for _stream in (sys.stdout, sys.stderr):
    try:
        _stream.reconfigure(encoding="utf-8", errors="replace")  # type: ignore[union-attr]
    except (AttributeError, ValueError):
        pass


def log(msg: str) -> None:
    print(f"[loop {datetime.now():%H:%M:%S}] {msg}", flush=True)


# ── pass<N> discovery ─────────────────────────────────────────────────────
def pass_dirs() -> set[Path]:
    if not OUT_DIR.exists():
        return set()
    return {d for d in OUT_DIR.iterdir() if d.is_dir() and d.name.startswith("pass") and d.name[4:].isdigit()}


# ── test run ──────────────────────────────────────────────────────────────
def run_tests(only_ids: list[str] | None, capture_etl: bool, per_test_timeout: int | None) -> tuple[int, float, Path | None]:
    """Run tests-run.py, return (exit_code, wall_seconds, new_pass_dir)."""
    cmd = [sys.executable, str(TESTS_RUN), SUITE]
    if only_ids:
        cmd += ["--only", ",".join(only_ids)]
    if capture_etl:
        cmd += ["--captureetl"]
    if per_test_timeout:
        cmd += ["--timeout", str(per_test_timeout)]

    before = pass_dirs()
    log(f"running: {' '.join(cmd)}")
    # The child tests-run.py honors the same stop.txt, but loop.py owns the file's
    # lifetime (we consume it after debugging). Tell the child not to delete it.
    child_env = {**os.environ, "PFGS_STOP_SIGNAL_MANAGED_BY_PARENT": "1"}
    start = time.monotonic()
    proc = subprocess.run(cmd, cwd=str(TESTS_RUN.parent), env=child_env)
    wall = time.monotonic() - start

    # Only accept a pass dir that this run actually created. Falling back to a
    # pre-existing pass<N> would read STALE results from an earlier run.
    new_dirs = pass_dirs() - before
    new_pass = max(new_dirs, key=lambda d: int(d.name[4:])) if new_dirs else None
    log(f"tests-run.py exit={proc.returncode} wall={wall:.0f}s pass_dir={new_pass}")
    return proc.returncode, wall, new_pass


def run_build() -> tuple[bool, str]:
    """Rebuild the test harness via tests-build.ps1. Returns (success, combined_output).
    Output is echoed to the console and returned so a failure can be handed to the agent."""
    if not TESTS_BUILD.is_file():
        msg = f"tests-build.ps1 not found at {TESTS_BUILD}"
        log(f"ERROR: {msg}")
        return False, msg
    cmd = ["powershell", "-NoProfile", "-ExecutionPolicy", "Bypass", "-File", str(TESTS_BUILD)]
    log(f"rebuilding test harness: {' '.join(cmd)}")
    start = time.monotonic()
    proc = subprocess.run(cmd, cwd=str(TESTS_BUILD.parent),
                          capture_output=True, text=True, encoding="utf-8", errors="replace")
    wall = time.monotonic() - start
    output = (proc.stdout or "") + (proc.stderr or "")
    # Echo so the user still sees the build output in the loop console.
    if output.strip():
        print(output, flush=True)
    log(f"tests-build.ps1 exit={proc.returncode} wall={wall:.0f}s")
    return proc.returncode == 0, output


def read_summary_csv(pass_dir: Path) -> list[dict]:
    csv_path = pass_dir / "summary.csv"
    if not csv_path.is_file():
        return []
    with open(csv_path, newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def is_failure(row: dict) -> bool:
    return (row.get("status") or "").strip() not in PASS_STATUSES


# ── testresult.txt writer ─────────────────────────────────────────────────
def write_testresult(pass_dir: Path, rows: list[dict], wall_seconds: float) -> list[dict]:
    """Write the SUMMARY + FAILURES report; return the failure rows."""
    total = len(rows)
    passed = sum(1 for r in rows if (r.get("status") or "").strip() == "PASS")
    skipped = sum(1 for r in rows if (r.get("status") or "").strip() == "SKIP")
    failures = [r for r in rows if is_failure(r)]
    failed = len(failures)
    csv_path = pass_dir / "summary.csv"

    lines: list[str] = []
    lines.append(BAR)
    lines.append("  SUMMARY")
    lines.append(BAR)
    lines.append(f"  Total:   {total}")
    lines.append(f"  Passed:  {passed}")
    lines.append(f"  Failed:  {failed}")
    lines.append(f"  Skipped: {skipped}")
    lines.append(f"  Time:    {wall_seconds:.0f}s ({wall_seconds/60:.1f} min)")
    lines.append(f"  Logs:    {pass_dir}")
    lines.append(f"  CSV:     {csv_path}")
    lines.append(BAR)

    if failures:
        lines.append("")
        lines.append(f"  FAILURES ({failed}):")
        for r in failures:
            status = (r.get("status") or "FAIL").strip()
            tid = (r.get("test_id") or "?").strip()
            scen = (r.get("scenario") or "?").strip()
            err = (r.get("error") or "").strip()
            suffix = f" \u2014 {err}" if err else ""
            lines.append(f"    [{status}] {tid}: {scen}{suffix}")

    RESULT_TXT.parent.mkdir(parents=True, exist_ok=True)
    RESULT_TXT.write_text("\n".join(lines) + "\n", encoding="utf-8")
    log(f"wrote {RESULT_TXT}  (total={total} passed={passed} failed={failed} skipped={skipped})")
    return failures


def write_runner_failure(reason: str) -> None:
    """Best-effort testresult.txt when the runner produced no CSV."""
    text = (
        f"{BAR}\n  SUMMARY\n{BAR}\n"
        f"  Test runner did not produce results.\n  Reason: {reason}\n{BAR}\n"
    )
    RESULT_TXT.parent.mkdir(parents=True, exist_ok=True)
    RESULT_TXT.write_text(text, encoding="utf-8")
    log(f"wrote runner-failure {RESULT_TXT}: {reason}")


# ── AI debug hand-off (direct Copilot invocation) ─────────────────────────
# We invoke `copilot` non-interactively (-p) with the debug prompt. This does NOT
# use the agent-mesh (which requires a Copilot feature flag not available to all
# accounts) — it just runs the `tester` agent to completion, so it works for
# anyone with the Copilot CLI installed.

def resolve_copilot() -> list[str] | None:
    """Return the base command to launch Copilot, or None if not found.

    Prefers invoking Copilot through `node` (clean arg passing for very large prompts,
    no cmd.exe quoting), and specifically through **npm-loader.js** — the same entry
    point the `copilot` shim itself runs. The loader resolves the CURRENT build, which
    matters because the CLI auto-updates itself in place: `index.js` is the build that
    shipped with the npm package and goes stale after the first auto-update, so calling
    it directly silently pins the loop to an old CLI (symptom: `--model` rejecting model
    IDs that work fine when you type `copilot --model ...` yourself)."""
    copilot = shutil.which("copilot")
    if copilot:
        # npm global: <prefix>\copilot.cmd  +  <prefix>\node_modules\@github\copilot\*.js
        pkg = Path(copilot).parent / "node_modules" / "@github" / "copilot"
        node = shutil.which("node")
        if node:
            for entry in ("npm-loader.js", "index.js"):
                candidate = pkg / entry
                if candidate.is_file():
                    return [node, str(candidate)]
        if os.name == "nt":
            return ["cmd", "/c", copilot]
        return [copilot]
    return None


def preflight_copilot() -> bool:
    """Verify the Copilot CLI is available BEFORE running the (long) tests, so we
    don't spend hours testing only to have the AI hand-off fail."""
    base = resolve_copilot()
    if base:
        # Log the resolved entry point, not just "node": which entry point is used
        # determines which CLI build (and therefore which models) the loop gets.
        log(f"preflight OK: Copilot CLI found ({' '.join(base)}).")
        return True
    log("preflight FAILED: 'copilot' not found on PATH. Install the GitHub Copilot CLI, "
        "then re-run. Not running tests.")
    return False


def _invoke(prompt: str, agent: str | None, timeout_s: int,
            extra_flags: list[str], what: str) -> bool:
    """Low-level: run `copilot -p <prompt>` with the given extra flags. Blocks."""
    base = resolve_copilot()
    if not base:
        log("ERROR: Copilot CLI not found.")
        return False
    cmd = base + ["-p", prompt, "--allow-all-tools", "--allow-all-paths"] + extra_flags
    if agent:
        cmd += ["--agent", agent]
    who = f"agent '{agent}'" if agent else "Copilot (inlined tester profile)"
    resume = next((extra_flags[i + 1] for i, f in enumerate(extra_flags) if f == "--resume"), None)
    ctx = f" [resume {resume[:8]}...]" if resume else ""
    log(f"invoking {who}{ctx} to {what} (timeout {timeout_s}s) ...")
    try:
        proc = subprocess.run(cmd, cwd=str(SCRIPT_DIR), timeout=timeout_s)
    except subprocess.TimeoutExpired:
        log(f"Copilot timed out after {timeout_s}s.")
        return False
    log(f"Copilot finished (exit {proc.returncode}).")
    return proc.returncode == 0


def ensure_primed(agent: str | None, timeout_s: int, force: bool = False) -> None:
    """Resolve the primed session for 'prime' mode. Reuses the session id saved by a
    previous run of loop.py (SESSION_FILE) so we don't re-read the bootstrap docs each
    run. Primes a fresh session (and saves its id) only when there's no valid saved
    session, when the bootstrap guide changed, on --refresh-context, or when 'force'
    is set (stale-session recovery). Runs at most once per phase; falls back to
    'inline' if a required priming call fails."""
    if PRIME_STATE["done"] or PRIME_STATE["mode"] != "prime":
        return
    PRIME_STATE["done"] = True
    if not AI_BOOTSTRAP.is_file():
        log(f"prime mode: AI bootstrap guide not found ({AI_BOOTSTRAP}); falling back to inline.")
        PRIME_STATE["mode"] = "inline"
        return

    # Reuse a previously-saved primed session unless we're forcing a refresh.
    if not force and not PRIME_STATE["force_fresh"]:
        saved = load_persisted_session()
        if saved:
            PRIME_STATE["session_id"] = saved
            PRIME_STATE["from_disk"] = True
            log(f"reusing primed session {saved[:8]}... from {SESSION_FILE.name} "
                f"(skipping re-priming; use --refresh-context to re-prime).")
            return

    sid = str(uuid.uuid4())
    prime_text = render_prompt(DEFAULT_PRIME_PROMPT)
    log(f"priming a session ({sid[:8]}...) — following {AI_BOOTSTRAP.name} and reading the docs it lists ...")
    ok = _invoke(prime_text, agent, timeout_s, ["--session-id", sid], "load project context")
    if ok:
        PRIME_STATE["session_id"] = sid
        PRIME_STATE["from_disk"] = False
        save_persisted_session(sid)
        log("priming complete; debug runs will --resume this primed session.")
    else:
        log("priming failed; falling back to inline context injection.")
        PRIME_STATE["mode"] = "inline"


def build_prompt(prompt_path: Path, agent: str | None, profile: Path | None = None,
                 extra: dict | None = None) -> str | None:
    """Render a prompt file, inlining an agent profile (always, unless --agent is used)
    and — in 'inline' context mode — the AI bootstrap guide, so the agent is loaded with
    project context. In 'prime' mode the guide is NOT inlined (it's in the resumed session).
    'profile' defaults to the tester profile; review mode passes the reviewer profile."""
    if not prompt_path.is_file():
        log(f"ERROR: prompt not found at {prompt_path}")
        return None
    profile = profile or AGENT_PROFILE
    prompt = render_prompt(prompt_path.read_text(encoding="utf-8"), extra)
    if not agent and profile.is_file():
        prompt = render_prompt(profile.read_text(encoding="utf-8"), extra) + "\n\n---\n\n" + prompt
    if PRIME_STATE["mode"] == "inline" and AI_BOOTSTRAP.is_file():
        guide = render_prompt(AI_BOOTSTRAP.read_text(encoding="utf-8"), extra)
        prompt = ("# Project context — follow this guide and read the docs it lists first\n\n"
                  f"{guide}\n\n---\n\n") + prompt
    return prompt


def _resume_flags() -> list[str]:
    if PRIME_STATE["mode"] == "prime" and PRIME_STATE["session_id"]:
        return ["--resume", PRIME_STATE["session_id"]]
    return []


def run_agent(prompt_path: Path, agent: str | None, timeout_s: int, what: str,
              profile: Path | None = None, extra: dict | None = None) -> bool:
    """Run Copilot non-interactively with the given prompt file. Blocks until it
    finishes. Returns True on a clean exit.

    In 'prime' mode it resumes the primed session (reused across loop.py runs). If
    resuming a SAVED session fails, the session is likely stale/deleted, so it
    re-primes a fresh session once and retries. If --agent is not supplied, the
    local tester.agent.md is inlined as a preamble so the bundle is self-contained."""
    ensure_primed(agent, timeout_s)  # lazy, once — only when the agent is actually needed
    prompt = build_prompt(prompt_path, agent, profile, extra)
    if prompt is None:
        return False
    if _invoke(prompt, agent, timeout_s, _resume_flags(), what):
        return True

    # If we were resuming a SAVED (unverified) session, it may be stale/gone. Re-prime
    # a fresh session once and retry (covers a deleted/expired session id on disk).
    if PRIME_STATE["mode"] == "prime" and PRIME_STATE["from_disk"] and not PRIME_STATE["reprimed"]:
        log("resuming the saved session failed; it may be stale. Re-priming a fresh session and retrying ...")
        PRIME_STATE["reprimed"] = True
        PRIME_STATE["done"] = False
        PRIME_STATE["from_disk"] = False
        PRIME_STATE["session_id"] = None
        ensure_primed(agent, timeout_s, force=True)
        prompt = build_prompt(prompt_path, agent, profile, extra)  # mode may have flipped to inline if re-prime failed
        if prompt is None:
            return False
        return _invoke(prompt, agent, timeout_s, _resume_flags(), what)
    return False


def run_agent_debug(agent: str | None, timeout_s: int) -> bool:
    """Run the agent on the current test failures (writes aidebugresult.json)."""
    return run_agent(DEBUG_PROMPT, agent, timeout_s, "debug failures")


# ── Review mode (--mode review) ───────────────────────────────────────────
# A round is: N reviewers in PARALLEL (one process per model, each writing its own
# markdown report) -> one fixer that verifies/de-dupes/fixes and writes
# aireviewresult.json -> build -> optional test validation. The loop repeats until
# the fixer reports fewer than --converge-threshold unique real findings.
#
# Reviewers deliberately do NOT resume the primed session: they run concurrently, and
# two processes resuming the same session would collide. They get their context from
# the review spec (REVIEW_SPEC), which is self-contained by design.

def load_review_state() -> dict:
    """Accumulated review state (fix summaries fed back to later rounds as
    'already fixed, do not re-report'). Persisted so a new run of loop.py continues
    where the last one left off instead of re-reporting old fixes."""
    if not REVIEW_STATE_FILE.is_file():
        return {"rounds": 0, "fixed": [], "deferred": []}
    try:
        data = json.loads(REVIEW_STATE_FILE.read_text(encoding="utf-8"))
        data.setdefault("rounds", 0)
        data.setdefault("fixed", [])
        data.setdefault("deferred", [])
        return data
    except (json.JSONDecodeError, OSError):
        return {"rounds": 0, "fixed": [], "deferred": []}


def save_review_state(state: dict) -> None:
    try:
        REVIEW_STATE_FILE.write_text(json.dumps(state, indent=2), encoding="utf-8")
    except OSError as e:
        log(f"warning: could not persist review state ({e}).")


def reset_review_state() -> None:
    try:
        REVIEW_STATE_FILE.unlink()
        log(f"cleared {REVIEW_STATE_FILE.name} (starting a fresh review history).")
    except FileNotFoundError:
        pass
    except OSError as e:
        log(f"warning: could not clear review state ({e}).")


def read_exclusions() -> str:
    """The standing 'do not report' list, with comment lines stripped."""
    if not REVIEW_EXCLUSIONS.is_file():
        return "(none)"
    lines = [ln for ln in REVIEW_EXCLUSIONS.read_text(encoding="utf-8").splitlines()
             if not ln.lstrip().startswith("#")]
    text = "\n".join(lines).strip()
    return text or "(none)"


def format_prior_fixes(state: dict, seed_file: Path | None = None) -> str:
    """Render accumulated fix summaries for injection into the reviewer prompt.

    'seed_file' lets a run continue work that wasn't driven by this loop (or whose
    state file was lost): its text is injected verbatim ahead of the tracked summaries,
    so the reviewers know what has already been fixed and don't re-report it."""
    blocks: list[str] = []

    if seed_file:
        try:
            seeded = seed_file.read_text(encoding="utf-8").strip()
        except OSError as e:
            log(f"warning: could not read prior-fixes file {seed_file} ({e}).")
            seeded = ""
        if seeded:
            blocks.append(seeded)

    fixed = state.get("fixed") or []
    if fixed:
        lines = []
        for item in fixed:
            rnd = item.get("round", "?")
            sev = item.get("severity", "")
            summary = item.get("summary", "")
            where = item.get("file", "")
            sev_txt = f"[{sev}] " if sev else ""
            where_txt = f" ({where})" if where else ""
            lines.append(f"- Round {rnd}: {sev_txt}{summary}{where_txt}")
        blocks.append("\n".join(lines))

    if not blocks:
        return "(nothing yet — this is the first round)"
    return "\n\n".join(blocks)


def format_prior_deferred(state: dict) -> str:
    """Render accumulated DEFERRED findings for injection into the reviewer prompt.

    These must be fed back separately from the fixed list. A deferred finding is a real
    defect that was deliberately not fixed, so it is still present in the source and a
    reviewer will find it again every round. It also counts toward `unique_findings`
    (the fixer counts fixed + deferred), so without this the convergence number has a
    floor equal to the deferred count: with 3 deferred and the default threshold of 3,
    the loop can never converge and burns every iteration before exiting non-zero."""
    deferred = state.get("deferred") or []
    if not deferred:
        return "(none)"
    lines = []
    for item in deferred:
        rnd = item.get("round", "?")
        sev = item.get("severity", "")
        summary = item.get("summary", "")
        where = item.get("file", "")
        why = item.get("reason", "") or item.get("rationale", "")
        sev_txt = f"[{sev}] " if sev else ""
        where_txt = f" ({where})" if where else ""
        why_txt = f" — deferred because: {why}" if why else ""
        lines.append(f"- Round {rnd}: {sev_txt}{summary}{where_txt}{why_txt}")
    return "\n".join(lines)


def next_report_index(date_stamp: str) -> int:
    """Reports are code-review-<DATE>-<N>.md; continue N rather than overwriting."""
    if not REVIEW_DIR.is_dir():
        return 1
    highest = 0
    prefix = f"code-review-{date_stamp}-"
    for path in REVIEW_DIR.glob(f"{prefix}*.md"):
        tail = path.stem[len(prefix):]
        if tail.isdigit():
            highest = max(highest, int(tail))
    return highest + 1


def _invoke_async(prompt: str, agent: str | None, model: str | None,
                  log_path: Path) -> subprocess.Popen | None:
    """Start `copilot -p <prompt>` WITHOUT waiting, so reviewers run concurrently.
    Each child's output goes to its own log file (interleaved console output from
    parallel agents is unreadable)."""
    base = resolve_copilot()
    if not base:
        log("ERROR: Copilot CLI not found.")
        return None
    cmd = base + ["-p", prompt, "--allow-all-tools", "--allow-all-paths"]
    if model:
        cmd += ["--model", model]
    if agent:
        cmd += ["--agent", agent]
    log_path.parent.mkdir(parents=True, exist_ok=True)
    handle = log_path.open("w", encoding="utf-8", errors="replace")
    try:
        proc = subprocess.Popen(cmd, cwd=str(SCRIPT_DIR), stdout=handle, stderr=subprocess.STDOUT)
    except OSError as e:
        log(f"ERROR: could not start reviewer ({e}).")
        handle.close()
        return None
    proc._loop_log_handle = handle  # type: ignore[attr-defined]  # closed by the waiter
    return proc


def run_reviewers(models: list[str], agent: str | None, timeout_s: int, scope: str,
                  state: dict, round_no: int, seed_file: Path | None = None) -> list[Path]:
    """Run one reviewer per model in parallel. Returns the reports actually written."""
    date_stamp = datetime.now().strftime("%Y-%m-%d")
    index = next_report_index(date_stamp)
    REVIEW_DIR.mkdir(parents=True, exist_ok=True)

    exclusions = read_exclusions()
    prior = format_prior_fixes(state, seed_file)
    prior_deferred = format_prior_deferred(state)

    started: list[tuple[str, Path, subprocess.Popen]] = []
    for offset, model in enumerate(models):
        report = REVIEW_DIR / f"code-review-{date_stamp}-{index + offset}.md"
        label = f"{model} (round {round_no})"
        prompt = build_prompt(REVIEW_PROMPT, agent, REVIEWER_PROFILE, {
            "REVIEW_SCOPE": scope,
            "REVIEW_REPORT": report,
            "REVIEWER_LABEL": label,
            "REVIEW_EXCLUSIONS": exclusions,
            "PRIOR_FIXES": prior,
            "PRIOR_DEFERRED": prior_deferred,
        })
        if prompt is None:
            return []
        # Stale report from a previous run would look like success if the agent dies.
        try:
            report.unlink()
        except FileNotFoundError:
            pass
        child_log = OUT_DIR / f"review-{model.replace('/', '-')}.log"
        proc = _invoke_async(prompt, agent, model, child_log)
        if proc is None:
            for _, _, p in started:
                p.kill()
            return []
        log(f"reviewer '{model}' started -> {report.name}  (log: {child_log})")
        started.append((model, report, proc))

    deadline = time.monotonic() + timeout_s
    reports: list[Path] = []
    for model, report, proc in started:
        remaining = max(1, int(deadline - time.monotonic()))
        try:
            rc = proc.wait(timeout=remaining)
        except subprocess.TimeoutExpired:
            log(f"reviewer '{model}' timed out after {timeout_s}s; killing it.")
            proc.kill()
            rc = -1
        finally:
            handle = getattr(proc, "_loop_log_handle", None)
            if handle:
                handle.close()
        if report.is_file() and report.stat().st_size > 0:
            log(f"reviewer '{model}' finished (exit {rc}) -> {report.name}")
            reports.append(report)
        else:
            # A non-zero exit with a report still written is usable; no report is not.
            # Surface the child's own error (e.g. an invalid --model value) instead of
            # making the operator go open the log to find out why the round died.
            child_log = OUT_DIR / f"review-{model.replace('/', '-')}.log"
            log(f"reviewer '{model}' produced no report (exit {rc}). See {child_log}")
            try:
                tail = [ln.strip() for ln in child_log.read_text(encoding="utf-8", errors="replace").splitlines()
                        if ln.strip()][-5:]
            except OSError:
                tail = []
            for line in tail:
                log(f"    {model}: {line}")
    return reports


def read_aireview() -> dict | None:
    """Read the fixer's machine-readable result for this round."""
    if not AIREVIEW_JSON.is_file():
        log(f"fixer did not write {AIREVIEW_JSON}")
        return None
    try:
        data = json.loads(AIREVIEW_JSON.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, OSError) as e:
        log(f"could not parse {AIREVIEW_JSON}: {e}")
        return None
    log(f"read aireviewresult.json (unique_findings={data.get('unique_findings')}, "
        f"fixed={len(data.get('fixed') or [])}, deferred={len(data.get('deferred') or [])})")
    return data


def unique_findings_count(data: dict) -> int:
    """Trust the fixer's own count when present; otherwise derive it."""
    raw = data.get("unique_findings")
    if isinstance(raw, int) and raw >= 0:
        return raw
    if isinstance(raw, str) and raw.strip().isdigit():
        return int(raw.strip())
    return len(data.get("fixed") or []) + len(data.get("deferred") or [])


def format_review_fixes(data: dict) -> str:
    """One line per fix this round, for the regression prompt: the agent needs to know
    what just changed to know what to suspect."""
    fixed = data.get("fixed") or []
    if not fixed:
        return "(the fixer reported no code changes this round)"
    lines = []
    for item in fixed:
        sev = item.get("severity", "")
        where = item.get("file", "")
        summary = item.get("summary", "")
        lines.append(f"- [{sev}] {where}: {summary}" if sev or where else f"- {summary}")
    return "\n".join(lines)


def repair_review_regressions(args, data: dict, failures: list[dict]) -> bool:
    """The review fixes broke the suite. Drive the agent to repair them, exactly like
    test mode does: fix -> rebuild -> re-run just those tests, until green or out of
    attempts. Returns True when the suite is green again.

    This is deliberately a sub-loop rather than a hard stop: a review round that lands a
    real fix and a regression together is normal, and the loop already knows how to
    debug failing tests — handing that back to the operator wastes the round."""
    if not REVIEW_REGRESSION_PROMPT.is_file():
        log(f"ERROR: regression prompt not found at {REVIEW_REGRESSION_PROMPT}.")
        return False

    review_fixes = format_review_fixes(data)
    failed_ids = [(r.get("test_id") or "?").strip() for r in failures]
    prev_failed: frozenset[str] | None = None
    stall = 0

    for attempt in range(1, args.max_regression_attempts + 1):
        log(f"{BAR}")
        log(f"REGRESSION REPAIR {attempt}/{args.max_regression_attempts} — "
            f"{len(failed_ids)} failing: {', '.join(failed_ids)}")
        log(f"{BAR}")

        # No-progress guard: if the same set keeps failing, the agent isn't converging.
        current = frozenset(failed_ids)
        if prev_failed is not None and current == prev_failed:
            stall += 1
            if stall >= args.max_stall:
                log(f"No progress: the same {len(current)} test(s) failed {args.max_stall} "
                    f"repair attempts in a row. Stopping.")
                return False
        else:
            stall = 0
        prev_failed = current

        try:
            AIDEBUG_JSON.unlink()
        except FileNotFoundError:
            pass
        if not run_agent(REVIEW_REGRESSION_PROMPT, args.agent, args.agent_timeout,
                         "repair the regressions from this review round",
                         REVIEWER_PROFILE,
                         {"REVIEW_FIXES": review_fixes, "REVIEW_SCOPE": args.review_scope}):
            log("Copilot did not complete the regression repair cleanly; stopping.")
            return False

        repair = read_aidebug()
        if repair is None:
            log("No usable aidebugresult.json from the repair pass; stopping.")
            return False

        if normalize_rebuild(repair) and not build_with_agent_fix(args.agent, args.agent_timeout,
                                                                 REVIEWER_PROFILE):
            log("Build could not be fixed after the regression repair; stopping.")
            return False

        # Re-run everything that was failing (not just what the agent claims to have
        # fixed) so a repair that breaks a sibling test is caught in this same loop.
        exit_code, wall, pass_dir = run_tests(failed_ids, args.captureetl, args.timeout)
        if pass_dir is None or not (pass_dir / "summary.csv").is_file():
            write_runner_failure(f"tests-run.py exit {exit_code}, no summary.csv produced")
            log("Repair validation produced no results; stopping.")
            return False

        rows = read_summary_csv(pass_dir)
        failures = write_testresult(pass_dir, rows, wall)
        if not failures:
            log("regressions repaired; re-validating the full suite ...")
            exit_code, wall, pass_dir = run_tests(None, args.captureetl, args.timeout)
            if pass_dir is None or not (pass_dir / "summary.csv").is_file():
                write_runner_failure(f"tests-run.py exit {exit_code}, no summary.csv produced")
                log("Full re-validation produced no results; stopping.")
                return False
            rows = read_summary_csv(pass_dir)
            failures = write_testresult(pass_dir, rows, wall)
            if not failures:
                log("full suite is green again. \u2705")
                return True
            failed_ids = [(r.get("test_id") or "?").strip() for r in failures]
            log(f"full suite still has {len(failures)} failure(s): {', '.join(failed_ids)}")
            continue

        failed_ids = [(r.get("test_id") or "?").strip() for r in failures]
        log(f"still failing after repair: {', '.join(failed_ids)}")

        if stop_requested():
            log("Stop signal detected during regression repair; stopping.")
            clear_stop_signal()
            _STOP_STATE["stopped"] = True
            return False

    log(f"Could not repair the regressions in {args.max_regression_attempts} attempt(s).")
    return False


def run_review_process(args) -> int:
    """Run the iterative dual-model code review to convergence.

    Each round: reviewers (parallel) -> fixer -> build -> optional tests. Converges when
    the fixer reports fewer than --converge-threshold unique real findings."""
    if not REVIEW_SPEC.is_file():
        log(f"ERROR: review specification not found at {REVIEW_SPEC}")
        return 2
    for required in (REVIEW_PROMPT, REVIEW_FIX_PROMPT):
        if not required.is_file():
            log(f"ERROR: review prompt not found at {required}")
            return 2

    models = [m.strip() for m in args.review_models.split(",") if m.strip()]
    if not models:
        log("ERROR: --review-models is empty.")
        return 2

    if args.reset_review_state:
        reset_review_state()
    state = load_review_state()
    scope = args.review_scope

    # Round numbering is bookkeeping only, but it shows up in the logs, in each
    # reviewer's label and in the state file — so a continuation of earlier work can
    # keep counting from where that work stopped instead of restarting at 1.
    if args.start_round is not None:
        if args.start_round < 1:
            log("ERROR: --start-round must be 1 or greater.")
            return 2
        start_round = args.start_round
        state["rounds"] = max(state.get("rounds", 0), start_round - 1)
    else:
        start_round = state.get("rounds", 0) + 1

    seed_file: Path | None = None
    if args.prior_fixes_file:
        seed_file = Path(args.prior_fixes_file).expanduser()
        if not seed_file.is_absolute():
            seed_file = (Path.cwd() / seed_file).resolve()
        if not seed_file.is_file():
            log(f"ERROR: --prior-fixes-file not found at {seed_file}")
            return 2

    log(f"review scope: {scope}")
    log(f"reviewers: {', '.join(models)}  |  convergence: < {args.converge_threshold} unique findings")
    log(f"starting at round {start_round} (through {start_round + args.max_iterations - 1} at most)")
    if state.get("fixed"):
        log(f"carrying {len(state['fixed'])} previously-fixed finding(s) forward as 'do not re-report'.")
    if seed_file:
        log(f"seeding 'already fixed' context from {seed_file}")
    elif start_round > 1 and not state.get("fixed"):
        log("NOTE: starting mid-loop with no recorded fix history — the reviewers will not know "
            "what earlier rounds already fixed and may re-report it. Pass --prior-fixes-file "
            "<summary> (e.g. the review plan/summary doc) to tell them.")

    for round_no in range(start_round, start_round + args.max_iterations):
        log(f"{BAR}")
        log(f"REVIEW ROUND {round_no}")
        log(f"{BAR}")

        reports = run_reviewers(models, args.agent, args.agent_timeout, scope, state, round_no, seed_file)
        if not reports:
            log("No review reports were produced; stopping.")
            return 1
        if len(reports) < len(models):
            log(f"WARNING: only {len(reports)}/{len(models)} reviewers produced a report; "
                "continuing with what we have.")

        # Hand both reports to the fixer. Clear any stale result first.
        try:
            AIREVIEW_JSON.unlink()
        except FileNotFoundError:
            pass
        report_list = "\n".join(f"  - {p}" for p in reports)
        if not run_agent(REVIEW_FIX_PROMPT, args.agent, args.agent_timeout,
                         f"verify and fix round {round_no} findings",
                         REVIEWER_PROFILE, {"REVIEW_REPORTS": report_list, "REVIEW_SCOPE": scope}):
            log("Copilot fixer did not complete cleanly; stopping.")
            return 1

        data = read_aireview()
        if data is None:
            log("No usable aireviewresult.json from the fixer; stopping.")
            return 1

        # Record what was fixed so later rounds don't re-report it.
        for item in (data.get("fixed") or []):
            entry = dict(item)
            entry["round"] = round_no
            state["fixed"].append(entry)
        for item in (data.get("deferred") or []):
            entry = dict(item)
            entry["round"] = round_no
            state["deferred"].append(entry)
        state["rounds"] = round_no
        save_review_state(state)

        found = unique_findings_count(data)
        n_fixed = len(data.get("fixed") or [])
        n_rejected = len(data.get("rejected") or [])
        n_deferred = len(data.get("deferred") or [])
        log(f"round {round_no}: {found} unique finding(s) — {n_fixed} fixed, "
            f"{n_rejected} rejected as not-real, {n_deferred} deferred.")

        candidates = [d for d in (data.get("deferred") or []) if d.get("suggest_exclusion")]
        if candidates:
            log(f"** {len(candidates)} deferred finding(s) suggest adding a standing exclusion — "
                f"consider editing {REVIEW_EXCLUSIONS.name} so later rounds stop re-reporting them. **")
            for c in candidates:
                log(f"   - {c.get('id', '?')}: {c.get('reason', '')}")

        # Validate: build, then (unless skipped) run the suite. A fix that doesn't
        # compile or that breaks the suite is worse than the defect it removed.
        if normalize_rebuild({"rebuild": data.get("needs_build", True)}):
            if not build_with_agent_fix(args.agent, args.agent_timeout, REVIEWER_PROFILE):
                log("Build could not be fixed after the review changes; stopping.")
                return 1

        if not args.skip_tests:
            log(f"validating the review changes against the '{SUITE}' suite ...")
            exit_code, wall, pass_dir = run_tests(None, args.captureetl, args.timeout)
            if pass_dir is None or not (pass_dir / "summary.csv").is_file():
                write_runner_failure(f"tests-run.py exit {exit_code}, no summary.csv produced")
                log("Validation run produced no results; stopping.")
                return 1
            rows = read_summary_csv(pass_dir)
            failures = write_testresult(pass_dir, rows, wall)
            if failures:
                failed_ids = [(r.get("test_id") or "?").strip() for r in failures]
                log(f"** Review changes left {len(failures)} failing test(s): {', '.join(failed_ids)}. "
                    f"Handing them back to the agent to repair. **")
                if not repair_review_regressions(args, data, failures):
                    log(f"Regressions from round {round_no} could not be repaired automatically. "
                        f"See {RESULT_TXT}. Stopping so a human can look.")
                    return 1
            else:
                log("validation suite is green.")

        if stop_requested():
            log("Stop signal detected — finished this review round; stopping.")
            clear_stop_signal()
            _STOP_STATE["stopped"] = True
            return 0

        if found < args.converge_threshold:
            log(f"CONVERGED: {found} unique finding(s) this round (< {args.converge_threshold}). \u2705")
            log(f"Reports: {REVIEW_DIR}")
            return 0

    log(f"Reached max rounds ({args.max_iterations}) without converging. "
        f"Reports: {REVIEW_DIR}")
    return 1


def build_with_agent_fix(agent: str, timeout_s: int, profile: Path | None = None) -> bool:
    """Build the harness; on failure, hand the build output to the agent to fix and
    retry, up to MAX_BUILD_ATTEMPTS times. Returns True once the build succeeds,
    False if it still fails after the last attempt (loop should halt).

    'profile' selects the agent guidance inlined with the build-fix prompt. Review mode
    must pass REVIEWER_PROFILE: a build broken by review fixes is product code, and the
    default tester profile explicitly does not change product code, so handing it that
    job puts the agent in conflict with its own instructions."""
    for attempt in range(1, MAX_BUILD_ATTEMPTS + 1):
        log(f"build attempt {attempt}/{MAX_BUILD_ATTEMPTS}")
        ok, output = run_build()
        if ok:
            if attempt > 1:
                log("build succeeded after agent fix.")
            return True

        if attempt == MAX_BUILD_ATTEMPTS:
            log(f"build still failing after {MAX_BUILD_ATTEMPTS} attempts. Halting.")
            return False

        # Persist the build output so the agent can read the exact errors.
        BUILD_RESULT_TXT.parent.mkdir(parents=True, exist_ok=True)
        BUILD_RESULT_TXT.write_text(
            f"tests-build.ps1 FAILED (attempt {attempt}/{MAX_BUILD_ATTEMPTS})\n"
            f"{'=' * 60}\n{output}\n", encoding="utf-8")
        log(f"build failed; wrote {BUILD_RESULT_TXT}. Handing to agent to fix ...")

        if not BUILDFIX_PROMPT.is_file():
            log(f"ERROR: build-fix prompt not found at {BUILDFIX_PROMPT}. Halting.")
            return False

        # Clear any stale result so what we read below is definitely this invocation's.
        # Callers (run_process, repair_review_regressions) have already read their own
        # aidebugresult.json into memory before calling us, so removing the file is safe.
        try:
            AIDEBUG_JSON.unlink()
        except FileNotFoundError:
            pass

        if not run_agent(BUILDFIX_PROMPT, agent, timeout_s, "fix the build", profile):
            log("Copilot agent did not complete the build fix cleanly. Halting.")
            return False

        # buildfixprompt.txt defines a machine-readable contract, including an explicit
        # give-up signal ("If you could NOT fix the build, set rebuild to false so the loop
        # stops instead of looping"). Honor it. Without this the loop just rebuilds and
        # fails identically until MAX_BUILD_ATTEMPTS is exhausted, which hides the real
        # story: an agent that correctly determined it could not fix the break -- e.g. when
        # the cause is a missing dependency outside this repo -- looks the same as one that
        # tried and failed.
        result = read_aidebug()
        if result is None:
            log("agent finished but wrote no aidebugresult.json, so it reported no fix. "
                "Halting rather than rebuilding an unchanged tree.")
            return False
        if not normalize_rebuild(result):
            notes = str(result.get("notes") or "").strip()
            log("agent reported it could not fix the build (rebuild=false). Halting.")
            if notes:
                log(f"  agent notes: {notes}")
            return False
    return False


def read_aidebug() -> dict | None:
    """Read the agent's machine-readable result written this iteration."""
    if not AIDEBUG_JSON.is_file():
        log(f"agent did not write {AIDEBUG_JSON}")
        return None
    try:
        data = json.loads(AIDEBUG_JSON.read_text(encoding="utf-8"))
        log(f"read aidebugresult.json (rerun={data.get('rerun')})")
        return data
    except (json.JSONDecodeError, OSError) as e:
        log(f"could not parse {AIDEBUG_JSON}: {e}")
        return None


def normalize_rerun(data: dict) -> list[str]:
    raw = data.get("rerun", [])
    if not isinstance(raw, list):
        return []
    return [str(x).strip() for x in raw if str(x).strip()]


def pad_id(x: str) -> str:
    """Match tests-run.py --only normalization: zero-pad a single-digit test ID
    (e.g. '5' -> '05') so focus-set comparisons line up regardless of how the
    user typed the IDs."""
    x = str(x).strip()
    return x.zfill(2) if len(x) == 1 else x


def stop_requested() -> bool:
    """True if a cooperative stop was requested (stop.txt exists)."""
    return STOP_FILE.exists()


def clear_stop_signal() -> None:
    """Delete the stop signal file (loop.py owns its lifetime)."""
    try:
        STOP_FILE.unlink()
        log(f"cleared stop signal {STOP_FILE}")
    except FileNotFoundError:
        pass
    except OSError as e:
        log(f"could not delete stop signal {STOP_FILE}: {e}")


def count_category(data: dict, key: str) -> int:
    v = data.get(key)
    return len(v) if isinstance(v, list) else 0


def normalize_rebuild(data: dict) -> bool:
    """Interpret the agent's 'rebuild' flag tolerantly (bool or common truthy strings)."""
    raw = data.get("rebuild", False)
    if isinstance(raw, bool):
        return raw
    if isinstance(raw, str):
        return raw.strip().lower() in ("true", "yes", "1")
    return bool(raw)


# ── main loop ─────────────────────────────────────────────────────────────
def main(argv=None) -> int:
    ap = argparse.ArgumentParser(
        description="Self-driving loop for the PlayFab game save SDK. Two modes: "
                    "'test' (run the suite -> AI-debug failures -> re-test) and "
                    "'review' (parallel dual-model code review -> AI-fix -> build/validate), "
                    "each looping until it converges.")
    ap.add_argument("--mode", choices=("test", "review"), default="test",
                    help="What to loop on: 'test' debugs failing tests (default); 'review' runs an "
                         "iterative multi-model code review and fixes what it finds.")
    ap.add_argument("--suite", default=None,
                    help="Test suite to run (default: gamesave-xbox in test mode, "
                         "gamesave-inproc for review-mode validation).")
    ap.add_argument("--agent", default=None,
                    help="Copilot agent to use. If omitted, the local tester.agent.md (test mode) or "
                         "reviewer.agent.md (review mode) is inlined as a preamble (self-contained). "
                         "Pass e.g. --agent tester if you have it installed under the repo's .github/agents.")
    ap.add_argument("--max-iterations", type=int, default=10,
                    help="Safety cap on loop iterations, i.e. test iterations or review rounds "
                         "(default: 10).")
    ap.add_argument("--max-stall", type=int, default=3,
                    help="Stop if the exact same set of tests fails this many iterations in a row (default: 3).")
    ap.add_argument("--only", default=None,
                    help="Comma-separated test IDs to focus on (e.g. 65,81). Persists for the "
                         "ENTIRE loop: only these tests run every iteration, and the agent's "
                         "rerun list is intersected with this set so the loop never wanders "
                         "outside it. Default: the whole suite.")
    ap.add_argument("--all", action="store_true",
                    help="Re-run the full suite every iteration instead of only the agent's rerun "
                         "list. If combined with --only, re-runs the --only focus set every iteration.")
    ap.add_argument("--no-captureetl", dest="captureetl", action="store_false",
                    help="Do not capture ETL during test runs (faster; agent has less to debug).")
    ap.add_argument("--timeout", type=int, default=None, help="Per-test timeout in seconds (passed to tests-run.py).")
    ap.add_argument("--agent-timeout", type=int, default=3600,
                    help="Max seconds for the Copilot agent to debug each iteration (default: 3600).")
    ap.add_argument("--skip-preflight", action="store_true",
                    help="Skip the up-front check that the Copilot CLI is available (not recommended).")
    ap.add_argument("--skip-build", action="store_true",
                    help="Skip the initial up-front build (assume the harness is already current).")
    ap.add_argument("--context-mode", choices=("prime", "inline", "off"), default="prime",
                    help="How to pre-load project context before debugging: 'prime' (follow "
                         "ai-bootstrap.md once, then --resume that session each run; default), "
                         "'inline' (prepend the guide to every prompt), or 'off'.")
    ap.add_argument("--refresh-context", action="store_true",
                    help="Ignore the saved primed session and re-prime a fresh one (use after "
                         "the bootstrap docs change materially). The new session id is saved and "
                         "reused by later runs.")
    ap.add_argument("--repeat", type=int, default=1,
                    help="Run the entire process this many times in a row (default: 1). Useful for "
                         "soak / flakiness runs. Use 0 to repeat forever until interrupted. Each run "
                         "starts fresh (its own build, iterations, and stall tracking).")

    review = ap.add_argument_group("review mode (--mode review)")
    review.add_argument("--review-models", default=DEFAULT_REVIEW_MODELS,
                        help=f"Comma-separated models to review with, one parallel reviewer each "
                             f"(default: {DEFAULT_REVIEW_MODELS}). Must be model IDs the installed "
                             f"Copilot CLI accepts; an unknown ID makes that reviewer exit "
                             f"immediately (the loop prints its error). Different model families "
                             f"find different defects, which is the point of running more than one.")
    review.add_argument("--review-scope", default=DEFAULT_REVIEW_SCOPE,
                        help="What the reviewers audit. Free text, injected into the prompt. "
                             "Also selects the per-area 'already fixed' state file, so reviewing "
                             "a different area does not inherit another area's history.")
    review.add_argument("--review-spec", default=None,
                        help="Review specification the reviewers follow exactly (severity bar, "
                             "output format, scope exclusions, codebase context). Defaults to the "
                             "PFGameSave spec — override it when --review-scope points at another "
                             "component, otherwise reviewers get gamesave instructions.")
    review.add_argument("--review-dir", default=None,
                        help="Folder the per-round markdown reports are written to. Defaults to "
                             "the PFGameSave ai-code-review folder; override alongside "
                             "--review-spec so another component's reports do not land there.")
    review.add_argument("--converge-threshold", type=int, default=3,
                        help="Stop when a round produces fewer than this many unique real findings "
                             "(default: 3).")
    review.add_argument("--start-round", type=int, default=None,
                        help="Round number to start counting from (default: continue from the "
                             "persisted state, or 1). Bookkeeping only — it sets the round shown in "
                             "the logs, in each reviewer's label and in the state file. Use it to "
                             "continue a review that earlier rounds already advanced, e.g. "
                             "--start-round 7.")
    review.add_argument("--prior-fixes-file", default=None,
                        help="File whose text is injected into the reviewer prompt as 'already "
                             "fixed, do not re-report'. Use with --start-round when continuing work "
                             "this loop didn't drive (point it at the review plan / summary doc), "
                             "so the reviewers don't re-report what earlier rounds already fixed.")
    review.add_argument("--skip-tests", action="store_true",
                        help="Only build after each review round; don't run the validation suite "
                             "(much faster, but a fix that breaks behavior won't be caught).")
    review.add_argument("--max-regression-attempts", type=int, default=3,
                        help="If a review round's fixes break the suite, how many AI repair passes "
                             "to attempt (fix -> rebuild -> re-run) before giving up and stopping "
                             "for a human (default: 3).")
    review.add_argument("--reset-review-state", action="store_true",
                        help="Forget the accumulated 'already fixed' history before starting, so "
                             "reviewers may re-report previously fixed defects.")
    ap.set_defaults(captureetl=True)
    args = ap.parse_args(argv)

    # Suite drives every artifact path, so resolve it before anything reads them.
    # Validate first: an unknown name would otherwise only be caught by tests-run.py,
    # after the loop has already spent minutes on the up-front build.
    resolved_suite = args.suite or ("gamesave-inproc" if args.mode == "review" else SUITE)
    if not validate_suite(resolved_suite):
        return 2
    configure_suite(resolved_suite, args.mode)

    # Review mode reads a spec, writes reports and accumulates fixed/deferred history. All
    # three default to the PFGameSave locations, so point them at the reviewed area before
    # anything reads them -- otherwise another component's review inherits gamesave's
    # instructions, report folder and "already fixed" list.
    if args.mode == "review":
        configure_review_area(
            Path(args.review_spec).expanduser() if args.review_spec else None,
            Path(args.review_dir).expanduser() if args.review_dir else None,
            review_area_key(args.review_scope))

    PRIME_STATE["mode"] = args.context_mode
    PRIME_STATE["force_fresh"] = args.refresh_context

    # Delete any pre-existing stop signal at launch so a leftover file (from a
    # previously killed run, or one written before we started) can't immediately
    # stop this fresh invocation. A stop must be requested AFTER we start.
    if stop_requested():
        log(f"deleting pre-existing stop signal at launch: {STOP_FILE}")
        clear_stop_signal()

    if args.mode == "test":
        if not TESTS_RUN.is_file():
            log(f"ERROR: tests-run.py not found at {TESTS_RUN}")
            return 2
        if not DEBUG_PROMPT.is_file():
            log(f"ERROR: debug prompt not found at {DEBUG_PROMPT}")
            return 2
    else:
        if not args.skip_tests and not TESTS_RUN.is_file():
            log(f"ERROR: tests-run.py not found at {TESTS_RUN} (use --skip-tests to review without validating)")
            return 2

    # Fail fast: make sure the Copilot CLI is available before running any tests.
    if not args.skip_preflight and not preflight_copilot():
        return 2

    runner = run_review_process if args.mode == "review" else run_process

    if args.repeat == 1:
        return runner(args)

    # --repeat: run the whole build/test/AI-debug process multiple times (0 = forever).
    forever = args.repeat <= 0
    total = "\u221e" if forever else str(args.repeat)
    results: list[int] = []
    run_no = 0
    while forever or run_no < args.repeat:
        # A stop requested between process runs exits the repeat loop cleanly
        # rather than starting a run that would immediately abort with no tests.
        if stop_requested():
            log(f"Stop signal detected between process runs ({STOP_FILE}); exiting.")
            clear_stop_signal()
            break
        run_no += 1
        log(f"{BAR}")
        log(f"PROCESS RUN {run_no}/{total}")
        log(f"{BAR}")
        try:
            rc = runner(args)
        except KeyboardInterrupt:
            log("Interrupted by user; stopping repeat loop.")
            break
        results.append(rc)
        log(f"process run {run_no} finished with exit code {rc}.")
        if _STOP_STATE["stopped"]:
            log("Stop signal was honored; exiting the --repeat loop.")
            break

    passed = sum(1 for rc in results if rc == 0)
    failed = len(results) - passed
    log(f"{BAR}")
    log(f"REPEAT SUMMARY: {len(results)} run(s) \u2014 {passed} passed, {failed} failed.")
    log(f"{BAR}")
    # Exit 0 only if every process run converged/passed.
    return 0 if failed == 0 and results else 1


def run_process(args) -> int:
    """Run the entire build -> test -> AI-debug -> re-test process once.
    Returns an exit code (0 = all tests passed or converged, non-zero otherwise).
    Each call starts fresh: its own initial build, iteration counter, and stall tracking.
    Project-context priming (PRIME_STATE) is intentionally shared across repeats so the
    bootstrap docs are read at most once."""
    # Build the test harness up front so the first run uses current binaries.
    # On build failure, the same agent-fix flow applies (up to MAX_BUILD_ATTEMPTS).
    if not args.skip_build:
        log("initial build to ensure the test harness is up to date ...")
        if not build_with_agent_fix(args.agent, args.agent_timeout):
            log("Initial build could not be completed; stopping.")
            return 1

    # --only defines a persistent focus set: the loop stays constrained to these
    # test IDs for its whole lifetime (see the --all branch and the rerun
    # intersection below). Normalized so '5' and '05' compare equal.
    focus_ids = [pad_id(s) for s in args.only.split(",") if s.strip()] if args.only else None
    only_ids: list[str] | None = list(focus_ids) if focus_ids else None
    prev_failed: frozenset[str] | None = None  # for no-progress (stall) detection
    stall = 0

    for iteration in range(1, args.max_iterations + 1):
        log(f"{BAR}")
        log(f"ITERATION {iteration}/{args.max_iterations}"
            + (f"  (only: {','.join(only_ids)})" if only_ids else "  (full suite)"))
        log(f"{BAR}")

        exit_code, wall, pass_dir = run_tests(only_ids, args.captureetl, args.timeout)

        if pass_dir is None or not (pass_dir / "summary.csv").is_file():
            write_runner_failure(f"tests-run.py exit {exit_code}, no summary.csv produced")
            log("No results to evaluate; stopping.")
            return 1

        rows = read_summary_csv(pass_dir)
        if not rows:
            write_runner_failure(f"summary.csv at {pass_dir} is empty (no tests executed)")
            log("Empty summary — no tests executed; stopping.")
            return 1

        failures = write_testresult(pass_dir, rows, wall)

        if not failures:
            log("All tests passed. Done. \u2705")
            return 0

        failed_ids = [(r.get("test_id") or "?").strip() for r in failures]
        log(f"{len(failures)} failing: {', '.join(failed_ids)}")

        # No-progress guard: if the exact same set keeps failing, the agent isn't
        # making headway — stop before burning every iteration.
        current_failed = frozenset(failed_ids)
        if prev_failed is not None and current_failed == prev_failed:
            stall += 1
            if stall >= args.max_stall:
                log(f"No progress: the same {len(current_failed)} test(s) failed "
                    f"{args.max_stall} iterations in a row. Stopping.")
                return 1
        else:
            stall = 0
        prev_failed = current_failed

        # Hand off to the Copilot agent (blocks until it finishes), then read its result.
        # Clear any stale result first so we only read this iteration's output.
        try:
            AIDEBUG_JSON.unlink()
        except FileNotFoundError:
            pass
        if not run_agent_debug(args.agent, args.agent_timeout):
            log("Copilot agent did not complete cleanly; stopping.")
            return 1

        data = read_aidebug()
        if data is None:
            log("No usable aidebugresult.json from the agent; stopping.")
            return 1

        # Cooperative stop: the signal may have been written during the test run or
        # during this debug pass. Honor it now — the current iteration's failures
        # have been debugged, so finish cleanly without starting another iteration.
        if stop_requested():
            log("Stop signal detected — finished debugging this iteration's "
                "failures; stopping the loop and exiting.")
            clear_stop_signal()
            _STOP_STATE["stopped"] = True
            return 0

        should_rebuild = normalize_rebuild(data)

        if args.all:
            if should_rebuild and not build_with_agent_fix(args.agent, args.agent_timeout):
                log("Build could not be fixed; stopping.")
                return 1
            only_ids = list(focus_ids) if focus_ids else None  # re-run focus set (or everything) next time
            log("--all set: next iteration re-runs "
                + (f"the focus set ({','.join(focus_ids)})." if focus_ids else "the full suite."))
            continue

        rerun = normalize_rerun(data)
        # Keep the loop constrained to the --only focus set: never re-run a test
        # the user didn't ask to focus on, even if the agent lists it.
        if focus_ids:
            rerun = [t for t in rerun if pad_id(t) in focus_ids]
        if not rerun:
            n_platform = count_category(data, "platform_issues")
            n_blocked = count_category(data, "blocked")
            log(f"Agent returned an empty rerun list. Converged; stopping. "
                f"(platform issues logged: {n_platform}, blocked for human triage: {n_blocked})")
            if n_blocked:
                log(f"** {n_blocked} test(s) are BLOCKED and need human help — see the "
                    f"'-blocked' folders under {INVESTIGATIONS}. **")
            return 0

        # The agent changed compiled test code — rebuild the harness before re-running.
        # On build failure, hand the errors back to the agent to fix (up to 3 attempts).
        if should_rebuild and not build_with_agent_fix(args.agent, args.agent_timeout):
            log("Build could not be fixed; stopping.")
            return 1

        only_ids = rerun
        log(f"Next iteration will re-run: {', '.join(only_ids)}"
            + ("  (rebuilt)" if should_rebuild else ""))

    log(f"Reached max iterations ({args.max_iterations}) with failures still present. Stopping.")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())

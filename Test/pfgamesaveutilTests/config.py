#!/usr/bin/env python3
"""
Shared configuration and helpers for the pfgamesaveutil integration tests.

These are *integration* tests: they drive the real `pfgamesaveutil` executable
against a live PlayFab title, so they require credentials supplied via
environment variables.

Required environment variables
------------------------------
- PFSECRETKEY        PlayFab title secret key (from Game Manager). REQUIRED.
                     If it is not set, all cloud tests are skipped with a clear message.

Optional environment variables
------------------------------
- PFTITLEID          PlayFab title ID. Defaults to 'E18D7' (the test title).
- PFPLAYERID         Player's title_player_account entity ID (16 hex chars).
- PFMASTERPLAYERID   Player's master_player_account ID. Use instead of PFPLAYERID.
                     (Exactly one of PFPLAYERID / PFMASTERPLAYERID is required for
                     cloud tests; if neither is set, cloud tests are skipped.)
- PFGSUTIL_EXE       Full path to pfgamesaveutil.exe. Defaults to the published
                     binary under Tools/pfgamesaveutil/bin/publish/.
- PFGS_ALLOW_RESET   Set to '1' to allow the suite to delete the player's cloud
                     save data during cleanup. Off by default (non-destructive).
"""

import os
import subprocess
import sys
from pathlib import Path

# Repo root is three levels up: Test/pfgamesaveutilTests/config.py -> repo root
REPO_ROOT = Path(__file__).resolve().parents[2]
DEFAULT_EXE = REPO_ROOT / "Tools" / "pfgamesaveutil" / "bin" / "publish" / "pfgamesaveutil.exe"

# Default test title (matches the convention used by Utilities/Scripts/tests-run.py).
DEFAULT_TITLE_ID = "E18D7"

# The GameTest harness logs in with this custom id; the inproc<->util transfer tests
# resolve the same title_player_account so pfgamesaveutil acts on the harness player.
HARNESS_CUSTOM_ID = "GameTestHarness"

# GameTest runner + executables (used by the inproc<->util transfer tests).
TESTS_RUN_SCRIPT = REPO_ROOT / "Utilities" / "Scripts" / "tests-run.py"
GET_PLAYER_ID_SCRIPT = REPO_ROOT / "Utilities" / "Scripts" / "get-player-id.py"
CONTROLLER_EXE = REPO_ROOT / "Out" / "x64" / "Debug" / "GameTestController" / "GameTestController.exe"
DEVICE_EXE = REPO_ROOT / "Out" / "x64" / "Debug" / "GameTestAppWindows" / "GameTestAppWindows.exe"

COMMAND_TIMEOUT_SECONDS = 300


def get_secret_key():
    """Return the PlayFab secret key from PFSECRETKEY, or None if unset."""
    return os.environ.get("PFSECRETKEY")


def get_title_id():
    return os.environ.get("PFTITLEID", DEFAULT_TITLE_ID)


def get_title_player_id():
    return os.environ.get("PFPLAYERID")


def get_master_player_id():
    return os.environ.get("PFMASTERPLAYERID")


def get_exe_path():
    """Return the path to pfgamesaveutil.exe (env override or published default)."""
    override = os.environ.get("PFGSUTIL_EXE")
    return Path(override) if override else DEFAULT_EXE


def reset_allowed():
    return os.environ.get("PFGS_ALLOW_RESET") == "1"


def exe_missing_reason():
    """Returns a reason string if the exe is unavailable, else None (for local-only tests)."""
    exe = get_exe_path()
    if not exe.exists():
        return (f"pfgamesaveutil.exe not found at {exe}. "
                f"Build/publish it first (Tools/pfgamesaveutil/publish.cmd) "
                f"or set PFGSUTIL_EXE to its path.")
    return None


def missing_requirements():
    """
    Returns a human-readable reason string if the prerequisites for cloud tests
    are not met, or None if everything required is present.
    """
    if not get_secret_key():
        return ("PFSECRETKEY environment variable is not set. "
                "Set it to the PlayFab title secret key to run the cloud tests, e.g.:\n"
                "    PowerShell:  $env:PFSECRETKEY = '<secret>'\n"
                "    cmd:         set PFSECRETKEY=<secret>")

    if not get_title_player_id() and not get_master_player_id():
        return ("No player specified. Set PFPLAYERID (title_player_account entity ID) "
                "or PFMASTERPLAYERID (master_player_account ID) to run the cloud tests.")

    if get_title_player_id() and get_master_player_id():
        return "Both PFPLAYERID and PFMASTERPLAYERID are set; set only one."

    exe = get_exe_path()
    if not exe.exists():
        return (f"pfgamesaveutil.exe not found at {exe}. "
                f"Build/publish it first (Tools/pfgamesaveutil/publish.cmd) "
                f"or set PFGSUTIL_EXE to its path.")

    return None


def player_args():
    """Return the CLI args identifying the player (title or master id)."""
    title_player = get_title_player_id()
    if title_player:
        return ["--title-player-id", title_player]
    return ["--master-player-id", get_master_player_id()]


def run_util(args, timeout=COMMAND_TIMEOUT_SECONDS, expect_success=True, cwd=None):
    """
    Run pfgamesaveutil.exe with the given argument list.

    Returns the completed process (with .stdout / .stderr captured as text).
    Credentials (--title-id / --secret-key) are NOT injected automatically here;
    callers that need them use run_cloud() below.
    """
    exe = get_exe_path()
    cmd = [str(exe)] + args
    proc = subprocess.run(
        cmd,
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="replace",
        timeout=timeout,
        cwd=str(cwd) if cwd else None,
    )
    if expect_success and proc.returncode != 0:
        # Redact the secret key so it never lands in test logs / failure output.
        redacted_cmd = list(cmd)
        try:
            i = redacted_cmd.index("--secret-key")
            if i + 1 < len(redacted_cmd):
                redacted_cmd[i + 1] = "***REDACTED***"
        except ValueError:
            pass
        raise AssertionError(
            f"Command failed (exit {proc.returncode}): {' '.join(redacted_cmd)}\n"
            f"--- stdout ---\n{proc.stdout}\n--- stderr ---\n{proc.stderr}"
        )
    return proc


def run_cloud(verb, extra_args=None, timeout=COMMAND_TIMEOUT_SECONDS, expect_success=True, cwd=None):
    """
    Run a cloud-facing verb with title-id, secret-key, and player identity injected.

    Example: run_cloud("download", ["--path", "C:\\temp\\dl"])
    """
    args = [
        verb,
        "--title-id", get_title_id(),
        "--secret-key", get_secret_key(),
    ]
    args += player_args()
    if extra_args:
        args += extra_args
    return run_util(args, timeout=timeout, expect_success=expect_success, cwd=cwd)


# ---------------------------------------------------------------------------
# Inproc <-> pfgamesaveutil transfer tests (drive tests-run.py)
# ---------------------------------------------------------------------------

def transfer_tests_missing_reason():
    """
    Returns a reason string if prerequisites for the inproc<->util transfer tests
    are not met, else None. These tests need the title secret, the pfgamesaveutil
    exe, and the built GameTest controller + device executables.
    """
    if not get_secret_key():
        return ("PFSECRETKEY environment variable is not set. Set it to the PlayFab "
                "title secret key to run the inproc<->util transfer tests.")
    exe = get_exe_path()
    if not exe.exists():
        return f"pfgamesaveutil.exe not found at {exe} (build/publish it first)."
    if not CONTROLLER_EXE.exists() or not DEVICE_EXE.exists():
        return (f"GameTest executables not built. Expected:\n  {CONTROLLER_EXE}\n  {DEVICE_EXE}\n"
                "Build the GameTest harness (controller + GameTestAppWindows) first.")
    return None


def extract_player_id_from_logs(log_dir):
    """
    Extract the harness player's title_player_account entity id from a scenario's
    log directory using Utilities/Scripts/get-player-id.py (the same approach
    tests-run.py uses). Returns the entity id string, or None if not found.
    """
    log_dir = Path(log_dir)
    if not GET_PLAYER_ID_SCRIPT.exists() or not log_dir.exists():
        return None
    proc = subprocess.run(
        [sys.executable, str(GET_PLAYER_ID_SCRIPT), str(log_dir)],
        capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=60,
    )
    player_id = (proc.stdout or "").strip()
    return player_id or None


def run_util_for_player(verb, entity_id, extra_args=None, timeout=COMMAND_TIMEOUT_SECONDS,
                        expect_success=True, cwd=None):
    """Run a pfgamesaveutil cloud verb for a specific title_player_account entity id."""
    args = [
        verb,
        "--title-id", get_title_id(),
        "--secret-key", get_secret_key(),
        "--title-player-id", entity_id,
    ]
    if extra_args:
        args += extra_args
    return run_util(args, timeout=timeout, expect_success=expect_success, cwd=cwd)


def run_scenario(only_id, timeout=600):
    """
    Drive Utilities/Scripts/tests-run.py for the 'interactive' suite, running only the
    given scenario id (e.g. '04') on the inproc engine.

    Returns (proc, log_dir) where proc.returncode == 0 means the scenario passed and
    log_dir is the per-scenario log directory (Out/interactive-tests/pass<N>/<id>),
    or None if it could not be parsed from the runner output.
    """
    cmd = [
        sys.executable, str(TESTS_RUN_SCRIPT),
        "interactive",
        "--only", only_id,
        "--engines", "pc-inproc-gamesaves",
        "--timeout", str(timeout),
        "--stop-on-fail",
    ]
    proc = subprocess.run(
        cmd,
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="replace",
        timeout=timeout + 120,
    )

    # Parse "Logs:    <pass_dir>" from the runner output to locate the scenario logs.
    log_dir = None
    for line in (proc.stdout or "").splitlines():
        stripped = line.strip()
        if stripped.startswith("Logs:"):
            pass_dir = stripped.split("Logs:", 1)[1].strip()
            if pass_dir:
                candidate = Path(pass_dir) / only_id
                log_dir = candidate if candidate.exists() else Path(pass_dir)
            break

    return proc, log_dir

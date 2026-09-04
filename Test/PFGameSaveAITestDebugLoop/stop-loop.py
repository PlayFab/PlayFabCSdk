#!/usr/bin/env python3
r"""
stop-loop.py — cooperative stop signal for loop.py / tests-run.py.

Writes a small marker file that a running loop.py (or a standalone tests-run.py)
polls for and honors gracefully:

  * tests-run.py  — finishes the CURRENT test, then stops launching further tests
                    in the suite and returns.
  * loop.py       — lets the current iteration finish AND lets the AI agent debug
                    that iteration's failures, THEN stops looping and exits.

The signal is a file at:

    C:\git\PlayFab.C\Out\stop.txt

loop.py owns the file's lifetime when it is driving tests-run.py (it deletes the
file once it has stopped). A standalone tests-run.py deletes it itself after acting.
This script only writes / clears / reports the file; it does not talk to the running
process directly, so it works no matter how the loop was started.

Usage (from anywhere):
    py C:\git\PlayFab.C\Test\PFGameSaveAITestDebugLoop\stop-loop.py         # request stop
    py stop-loop.py --clear      # remove the signal (cancel a pending stop)
    py stop-loop.py --status     # report whether a stop is currently pending
"""

from __future__ import annotations

import argparse
import sys
from datetime import datetime
from pathlib import Path

# This file lives in <repo>\Test\PFGameSaveAITestDebugLoop\, so the repo root is
# two directories up. STOP_FILE must match loop.py / tests-run.py.
REPO_ROOT = Path(__file__).resolve().parent.parent.parent
STOP_FILE = REPO_ROOT / "Out" / "stop.txt"


def request_stop() -> int:
    STOP_FILE.parent.mkdir(parents=True, exist_ok=True)
    already = STOP_FILE.exists()
    stamp = datetime.now().isoformat(timespec="seconds")
    STOP_FILE.write_text(
        "stop requested at "
        f"{stamp}\n"
        "The running loop will stop after it finishes debugging the current "
        "iteration's failures (tests-run.py stops after the current test).\n",
        encoding="utf-8",
    )
    if already:
        print(f"Stop signal refreshed: {STOP_FILE}")
    else:
        print(f"Stop signal written: {STOP_FILE}")
    print("The loop will stop gracefully after the current iteration's debug pass.")
    return 0


def clear_stop() -> int:
    if STOP_FILE.exists():
        try:
            STOP_FILE.unlink()
        except OSError as e:
            print(f"ERROR: could not delete {STOP_FILE}: {e}", file=sys.stderr)
            return 1
        print(f"Stop signal cleared: {STOP_FILE}")
    else:
        print(f"No stop signal to clear (not present): {STOP_FILE}")
    return 0


def status() -> int:
    if STOP_FILE.exists():
        print(f"STOP PENDING: {STOP_FILE}")
        try:
            print("  " + STOP_FILE.read_text(encoding="utf-8").strip().replace("\n", "\n  "))
        except OSError:
            pass
    else:
        print(f"no stop pending: {STOP_FILE}")
    return 0


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(
        description="Write/clear the cooperative stop signal for loop.py / tests-run.py.")
    group = ap.add_mutually_exclusive_group()
    group.add_argument("--clear", action="store_true",
                       help="Remove the stop signal (cancel a pending stop).")
    group.add_argument("--status", action="store_true",
                       help="Report whether a stop is currently pending, then exit.")
    args = ap.parse_args(argv)

    if args.clear:
        return clear_stop()
    if args.status:
        return status()
    return request_stop()


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
"""
Test runner for the pfgamesaveutil integration tests.

Prints a clean, one-line-per-test result:

    [PASSED]  ClassName.test_name
    [FAILED]  ClassName.test_name
    [SKIPPED] ClassName.test_name - reason

Cloud (end-to-end) tests are SKIPPED unless PFSECRETKEY (and a player id) are set;
local tests (status / collect / reset --local) run without credentials.

Usage:
    py run_tests.py
or via the convenience wrapper:
    run-tests.cmd
"""

import sys
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))

import config  # noqa: E402


def _test_name(test) -> str:
    cls = test.__class__.__name__
    method = getattr(test, "_testMethodName", str(test))
    return f"{cls}.{method}"


class PlainResult(unittest.TestResult):
    """Prints one '[STATUS] name' line per test."""

    def __init__(self, stream):
        super().__init__()
        self.stream = stream

    def addSuccess(self, test):
        super().addSuccess(test)
        self.stream.write(f"[PASSED]  {_test_name(test)}\n")
        self.stream.flush()

    def addFailure(self, test, err):
        super().addFailure(test, err)
        self.stream.write(f"[FAILED]  {_test_name(test)}\n")
        self.stream.flush()

    def addError(self, test, err):
        super().addError(test, err)
        self.stream.write(f"[ERROR]   {_test_name(test)}\n")
        self.stream.flush()

    def addSkip(self, test, reason):
        super().addSkip(test, reason)
        short = (reason or "").splitlines()[0] if reason else ""
        self.stream.write(f"[SKIPPED] {_test_name(test)} - {short}\n")
        self.stream.flush()


def main() -> int:
    out = sys.stdout

    # Up-front note about whether the end-to-end cloud tests will run.
    cloud_reason = config.missing_requirements()
    if cloud_reason:
        out.write("NOTE: cloud (end-to-end upload/download) tests are SKIPPED.\n")
        out.write(f"      {cloud_reason.splitlines()[0]}\n")
        out.write("      Set PFSECRETKEY and PFPLAYERID (or PFMASTERPLAYERID) to run them.\n\n")
    else:
        out.write("Cloud (end-to-end) tests ENABLED.\n\n")

    suite = unittest.TestLoader().discover(start_dir=str(HERE), pattern="test_*.py")
    result = PlainResult(out)
    suite.run(result)

    # Show details for anything that didn't pass.
    for label, items in (("FAILED", result.failures), ("ERROR", result.errors)):
        for test, tb in items:
            out.write(f"\n----- {label}: {_test_name(test)} -----\n{tb}\n")

    total = result.testsRun
    failed = len(result.failures)
    errored = len(result.errors)
    skipped = len(result.skipped)
    passed = total - failed - errored - skipped

    out.write("\n" + "=" * 60 + "\n")
    out.write(
        f"Total: {total}   "
        f"Passed: {passed}   Failed: {failed}   Errors: {errored}   Skipped: {skipped}\n"
    )
    ok = failed == 0 and errored == 0
    out.write("RESULT: PASS\n" if ok else "RESULT: FAIL\n")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())

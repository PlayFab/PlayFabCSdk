#!/usr/bin/env python3
"""
Cross-tool transfer tests: pfgamesaveutil <-> inproc SDK round-trips.

These drive the GameTest harness (via Utilities/Scripts/tests-run.py --only) and
pfgamesaveutil against the SAME player to prove a save uploaded by one side
downloads intact on the other — including files and subfolders.

  test_inproc_upload_util_download   inproc uploads -> pfgamesaveutil downloads & verifies
                                     (scenario gamesave-interactive-05)
  test_util_upload_inproc_download   pfgamesaveutil uploads -> inproc downloads & verifies
                                     (scenario gamesave-interactive-04)

The harness player is identified by reading its title_player_account entity id from
the scenario logs (via get-player-id.py) — the same approach tests-run.py uses — so
pfgamesaveutil always operates on the exact player the inproc scenario used.

Requirements (else skipped with a message):
  - PFSECRETKEY (title secret key)
  - Built pfgamesaveutil.exe
  - Built GameTestController.exe + GameTestAppWindows.exe

The source folders below are the single source of truth shared with the YAML
scenarios (CopyTargetFolderToSaveFolder sourceFolder).
"""

import os
import shutil
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import config  # noqa: E402

# Reuse the hash-tree helpers from the main test module.
from test_upload_download import _hash_tree, _tree_diff_message  # noqa: E402

# These paths MUST match the `sourceFolder` values in the interactive-04/05 YAMLs.
XFER_ROOT = Path(r"C:\temp\pfgs-xfer")
SOURCE_UTIL_TO_INPROC = XFER_ROOT / "util-to-inproc"   # interactive-04
SOURCE_INPROC_TO_UTIL = XFER_ROOT / "inproc-to-util"   # interactive-05

TRANSFER_REASON = config.transfer_tests_missing_reason()


def _make_transfer_dataset(root: Path):
    """Create a save dataset with files at the root and in nested subfolders."""
    if root.exists():
        shutil.rmtree(root, ignore_errors=True)
    (root / "progress").mkdir(parents=True, exist_ok=True)
    (root / "settings" / "audio").mkdir(parents=True, exist_ok=True)
    (root / "DeviceB" / "nested").mkdir(parents=True, exist_ok=True)
    (root / "payload.bin").write_bytes(bytes(range(256)) * 8)
    (root / "progress" / "slot1.sav").write_text("level=12;score=99999", encoding="utf-8")
    (root / "progress" / "slot2.sav").write_text("level=3;score=42", encoding="utf-8")
    (root / "settings" / "config.json").write_text('{"volume":0.8,"lang":"en"}', encoding="utf-8")
    (root / "settings" / "audio" / "mix.bin").write_bytes(b"\x01\x02\x03\x04" * 64)
    (root / "DeviceB" / "nested" / "deep.dat").write_bytes(b"deep-nested-content")


@unittest.skipUnless(TRANSFER_REASON is None, TRANSFER_REASON or "transfer prerequisites not met")
class TransferTests(unittest.TestCase):
    # Discovered once: the harness player's title_player_account entity id.
    entity_id = None

    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory(prefix="pfgsutil_xfer_")
        self.tmp = Path(self._tmp.name)

    def tearDown(self):
        self._tmp.cleanup()

    def _assert_scenario_passed(self, proc, scenario_id):
        if proc.returncode != 0:
            self.fail(
                f"Scenario {scenario_id} did not pass (exit {proc.returncode}).\n"
                f"--- stdout (tail) ---\n{(proc.stdout or '')[-3000:]}\n"
                f"--- stderr (tail) ---\n{(proc.stderr or '')[-1500:]}"
            )

    def _remember_entity(self, log_dir):
        entity = config.extract_player_id_from_logs(log_dir) if log_dir else None
        if entity:
            TransferTests.entity_id = entity
        return entity

    def test_inproc_upload_util_download(self):
        """The inproc scenario uploads a dataset; pfgamesaveutil downloads and verifies it."""
        _make_transfer_dataset(SOURCE_INPROC_TO_UTIL)
        expected = _hash_tree(SOURCE_INPROC_TO_UTIL, ignore_dirs=())

        # 1. The inproc scenario loads the source folder and uploads it.
        proc, log_dir = config.run_scenario("05")
        self._assert_scenario_passed(proc, "interactive-05")

        # 2. Identify the harness player from its logs, then download via pfgamesaveutil.
        entity = self._remember_entity(log_dir)
        self.assertIsNotNone(entity, "Could not determine harness player id from scenario 05 logs.")

        dest = self.tmp / "dl"
        config.run_util_for_player("download", entity, ["--path", str(dest)])

        actual = _hash_tree(dest, ignore_dirs=("cloudsync",))
        self.assertEqual(
            expected, actual,
            "pfgamesaveutil download does not match the inproc-uploaded dataset "
            "(files/subfolders mismatch):\n" + _tree_diff_message(expected, actual),
        )

    def test_util_upload_inproc_download(self):
        """pfgamesaveutil uploads a dataset; the inproc scenario downloads and verifies it."""
        _make_transfer_dataset(SOURCE_UTIL_TO_INPROC)

        # Need the harness player id before uploading. If a prior test already
        # discovered it, reuse it; otherwise run scenario 05 once to learn it.
        entity = TransferTests.entity_id
        if not entity:
            proc, log_dir = config.run_scenario("05")
            self._assert_scenario_passed(proc, "interactive-05 (player discovery)")
            entity = self._remember_entity(log_dir)
        self.assertIsNotNone(entity, "Could not determine harness player id.")

        # 1. pfgamesaveutil uploads the dataset as the player's newest version.
        config.run_util_for_player(
            "upload", entity,
            ["--path", str(SOURCE_UTIL_TO_INPROC), "--force"],
        )

        # 2. The inproc scenario downloads the latest cloud save and verifies (snapshot
        #    compare against the same source folder) that files + subfolders transferred.
        proc, _ = config.run_scenario("04")
        self._assert_scenario_passed(proc, "interactive-04")


if __name__ == "__main__":
    import run_tests
    sys.exit(run_tests.main())

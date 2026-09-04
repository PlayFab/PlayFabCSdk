#!/usr/bin/env python3
"""
Integration tests for pfgamesaveutil — at least one test per verb.

Verbs covered:
  upload       CloudVerbTests.test_upload_download_round_trip
  download     CloudVerbTests.test_upload_download_round_trip
  downloadall  CloudVerbTests.test_downloadall_layout
  info         CloudVerbTests.test_info_json
  compare      CloudVerbTests.test_compare
  reset        CloudVerbTests.test_reset_cloud (cloud, opt-in) +
               LocalVerbTests.test_reset_local (local)
  collect      LocalVerbTests.test_collect_local
  status       LocalVerbTests.test_status_local

Run with:
    py -m unittest discover -s Test/pfgamesaveutilTests -v
or:
    py Test/pfgamesaveutilTests/test_upload_download.py

Cloud tests require PFSECRETKEY (and a player id); if unset they are skipped with
a logged message. Local tests (status / collect / reset --local) only require the
built executable and run without credentials. See config.py for all env vars.
"""

import glob
import hashlib
import json
import os
import re
import sys
import tempfile
import unittest
import zipfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import config  # noqa: E402

# Computed once at import: skip reasons for the two categories of tests.
EXE_REASON = config.exe_missing_reason()
CLOUD_REASON = config.missing_requirements()


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _sha256(path: Path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(65536), b""):
            h.update(block)
    return h.hexdigest()


def _hash_tree(root: Path, ignore_dirs=("cloudsync",)) -> dict:
    """Map of relative-path -> sha256 for every file under root, skipping ignore_dirs."""
    result = {}
    for dirpath, dirnames, filenames in os.walk(root):
        dirnames[:] = [d for d in dirnames if d not in ignore_dirs]
        for name in filenames:
            full = Path(dirpath) / name
            rel = full.relative_to(root).as_posix()
            result[rel] = _sha256(full)
    return result


def _tree_diff_message(expected: dict, actual: dict) -> str:
    """Human-readable description of how two path->hash trees differ."""
    exp_keys, act_keys = set(expected), set(actual)
    missing = sorted(exp_keys - act_keys)
    extra = sorted(act_keys - exp_keys)
    changed = sorted(k for k in (exp_keys & act_keys) if expected[k] != actual[k])
    lines = []
    if missing:
        lines.append(f"  missing in download: {missing}")
    if extra:
        lines.append(f"  unexpected in download: {extra}")
    if changed:
        lines.append(f"  content mismatch: {changed}")
    return "\n".join(lines) if lines else "  (no differences)"


def _write_random_file(path: Path, size_bytes: int, seed: int):
    """Write an incompressible (random) file of the given size."""
    import random
    rng = random.Random(seed)
    path.parent.mkdir(parents=True, exist_ok=True)
    with open(path, "wb") as f:
        remaining = size_bytes
        while remaining > 0:
            chunk = min(remaining, 65536)
            f.write(rng.randbytes(chunk))
            remaining -= chunk


def _make_sample_save(root: Path):
    """Create a small save folder with nested structure and known content."""
    (root / "progress").mkdir(parents=True, exist_ok=True)
    (root / "settings").mkdir(parents=True, exist_ok=True)
    (root / "payload.bin").write_bytes(b"top-level payload\x00\x01\x02")
    (root / "progress" / "slot1.sav").write_text("level=12;score=99999", encoding="utf-8")
    (root / "progress" / "slot2.sav").write_text("level=3;score=42", encoding="utf-8")
    (root / "settings" / "config.json").write_text('{"volume":0.8,"lang":"en"}', encoding="utf-8")


# ---------------------------------------------------------------------------
# Smoke tests — no credentials required
# ---------------------------------------------------------------------------

@unittest.skipUnless(EXE_REASON is None, EXE_REASON or "executable not available")
class SmokeTest(unittest.TestCase):
    def test_exe_present_and_help(self):
        proc = config.run_util(["--help"])
        for verb in ("download", "downloadall", "upload", "info", "compare", "reset", "collect", "status"):
            self.assertIn(verb, proc.stdout, f"Verb '{verb}' missing from --help")

    def test_missing_secret_is_reported(self):
        """With no --secret-key, a cloud verb must report a clear error message."""
        with tempfile.TemporaryDirectory(prefix="pfgsutil_nosecret_") as dest:
            proc = config.run_util(
                ["download", "--title-id", config.get_title_id(),
                 "--title-player-id", "0000000000000000", "--path", dest],
                expect_success=False,
            )
        combined = (proc.stdout + proc.stderr).lower()
        self.assertIn("secret-key", combined)
        self.assertIn("error", combined)


# ---------------------------------------------------------------------------
# Local verbs — no credentials required (only the built exe)
# ---------------------------------------------------------------------------

@unittest.skipUnless(EXE_REASON is None, EXE_REASON or "executable not available")
class LocalVerbTests(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory(prefix="pfgsutil_local_")
        self.tmp = Path(self._tmp.name)

    def tearDown(self):
        self._tmp.cleanup()

    def _make_fake_pgs_folder(self) -> Path:
        """Create a folder resembling a local PGS folder for a title."""
        folder = self.tmp / f"u_1234567890_{config.get_title_id()}"
        (folder / "current").mkdir(parents=True, exist_ok=True)
        (folder / "current" / "save.dat").write_bytes(b"local save bytes")
        (folder / "extended-1-manifest.json").write_text('{"v1":{"Files":[]}}', encoding="utf-8")
        return folder

    def test_status_local(self):
        """status: reads a local folder + registry and prints a status report."""
        folder = self._make_fake_pgs_folder()
        proc = config.run_util([
            "status",
            "--title-id", config.get_title_id(),
            "--path", str(folder),
        ])
        self.assertIn("Local Device Status", proc.stdout)
        self.assertIn("Registry Status", proc.stdout)

    def test_collect_local(self):
        """collect: zips a local PGS folder into a PGS-*.zip in the working dir."""
        folder = self._make_fake_pgs_folder()
        workdir = self.tmp / "work"
        workdir.mkdir()
        proc = config.run_util(
            [
                "collect",
                "--title-id", config.get_title_id(),
                "--path", str(folder),
            ],
            cwd=workdir,
        )
        zips = glob.glob(str(workdir / "PGS-*.zip"))
        self.assertEqual(len(zips), 1, f"Expected exactly one PGS zip, got: {zips}\n{proc.stdout}")
        # The archive should contain our save file and the injected device-status entry.
        with zipfile.ZipFile(zips[0]) as zf:
            names = zf.namelist()
        self.assertTrue(any(n.endswith("save.dat") for n in names), f"save.dat missing from zip: {names}")
        self.assertIn("_device-status.txt", names)

    def test_reset_local(self):
        """reset --local: deletes the targeted local folder."""
        folder = self._make_fake_pgs_folder()
        self.assertTrue(folder.exists())
        config.run_util([
            "reset",
            "--title-id", config.get_title_id(),
            "--path", str(folder),
            "--local",
            "--force",
        ])
        self.assertFalse(folder.exists(), "reset --local did not delete the folder")


# ---------------------------------------------------------------------------
# Cloud verbs — require PFSECRETKEY + player id
# ---------------------------------------------------------------------------

@unittest.skipUnless(CLOUD_REASON is None, CLOUD_REASON or "cloud prerequisites not met")
class CloudVerbTests(unittest.TestCase):
    @classmethod
    def tearDownClass(cls):
        # Opt-in cleanup: the suite creates new save versions; only delete the
        # player's cloud data when explicitly enabled (off by default).
        if config.reset_allowed():
            try:
                config.run_cloud("reset", ["--cloud", "--force"], expect_success=False)
            except Exception as ex:  # cleanup must never fail the suite
                print(f"[pfgamesaveutilTests] cloud reset cleanup skipped: {ex}", file=sys.stderr)

    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory(prefix="pfgsutil_cloud_")
        self.tmp = Path(self._tmp.name)

    def tearDown(self):
        self._tmp.cleanup()

    # -- helpers -----------------------------------------------------------

    def _upload(self, source: Path, extra_args=None):
        """Upload a folder; return (stdout, version:int, chunk_count:int)."""
        args = ["--path", str(source), "--force"]
        if extra_args:
            args += extra_args
        proc = config.run_cloud("upload", args)
        out = proc.stdout

        m_ver = re.search(r"New version:\s*(\d+)", out)
        self.assertIsNotNone(m_ver, f"No new version in upload output:\n{out}")
        m_chunks = re.search(r"in\s+(\d+)\s+chunk\(s\)", out)
        self.assertIsNotNone(m_chunks, f"No chunk count in upload output:\n{out}")
        self.assertIn("Done!", out, f"Upload did not complete:\n{out}")
        return out, int(m_ver.group(1)), int(m_chunks.group(1))

    # -- upload + download (folder-match validation) -----------------------

    def test_upload_download_round_trip(self):
        """upload then download — the downloaded folder must exactly match the upload."""
        source = self.tmp / "save"
        _make_sample_save(source)
        expected = _hash_tree(source)

        _out, version, _chunks = self._upload(source)

        dest = self.tmp / "dl"
        config.run_cloud("download", ["--path", str(dest)])

        actual = _hash_tree(dest, ignore_dirs=("cloudsync",))
        self.assertEqual(
            expected, actual,
            f"Downloaded folder does not match the uploaded folder (version {version}):\n"
            + _tree_diff_message(expected, actual),
        )

    def test_multizip_round_trip(self):
        """upload --max-zip-mb 1 must split into multiple chunks and still round-trip."""
        source = self.tmp / "bigsave"
        source.mkdir(parents=True, exist_ok=True)
        for i in range(4):
            _write_random_file(source / f"blob{i}.bin", 600 * 1024, seed=i + 1)
        expected = _hash_tree(source)

        _out, version, chunks = self._upload(source, extra_args=["--max-zip-mb", "1"])
        self.assertGreater(chunks, 1, f"Expected multiple chunks with --max-zip-mb 1, got {chunks}")

        dest = self.tmp / "dl"
        config.run_cloud("download", ["--path", str(dest)])
        actual = _hash_tree(dest, ignore_dirs=("cloudsync",))
        self.assertEqual(
            expected, actual,
            f"Multi-chunk version {version} did not round-trip:\n" + _tree_diff_message(expected, actual),
        )

    # -- downloadall -------------------------------------------------------

    def test_downloadall_layout(self):
        """downloadall writes list-manifest.json + v{N}/extracted matching the upload."""
        source = self.tmp / "save"
        _make_sample_save(source)
        expected = _hash_tree(source)

        _out, version, _chunks = self._upload(source)

        dest = self.tmp / "dlall"
        config.run_cloud("downloadall", ["--path", str(dest)])

        self.assertTrue((dest / "list-manifest.json").exists(), "missing list-manifest.json")
        version_dir = dest / f"v{version}"
        self.assertTrue(version_dir.is_dir(), f"missing per-version folder {version_dir}")
        extracted = version_dir / "extracted"
        self.assertTrue(extracted.is_dir(), f"missing extracted/ under {version_dir}")

        actual = _hash_tree(extracted, ignore_dirs=())
        self.assertEqual(
            expected, actual,
            f"v{version}/extracted does not match the upload:\n" + _tree_diff_message(expected, actual),
        )

    # -- info --------------------------------------------------------------

    def test_info_json(self):
        """info --json emits parseable JSON describing the player's manifests."""
        # Ensure there is at least one version to report.
        source = self.tmp / "save"
        _make_sample_save(source)
        self._upload(source)

        proc = config.run_cloud("info", ["--json"])
        start = proc.stdout.find("{")
        self.assertGreaterEqual(start, 0, f"No JSON in info output:\n{proc.stdout}")
        data = json.loads(proc.stdout[start:])
        self.assertIn("manifests", data, f"info JSON missing 'manifests': {list(data.keys())}")

    # -- compare -----------------------------------------------------------

    def test_compare(self):
        """compare runs cloud-vs-local and reports a comparison without error."""
        # Produce a known cloud version and a matching local folder (via download).
        source = self.tmp / "save"
        _make_sample_save(source)
        self._upload(source)

        local = self.tmp / "dl"
        config.run_cloud("download", ["--path", str(local)])

        proc = config.run_cloud("compare", ["--path", str(local)])
        self.assertIn("Cloud vs Local Comparison", proc.stdout, f"compare output unexpected:\n{proc.stdout}")

    # -- reset (cloud) — opt-in, destructive -------------------------------

    def test_reset_cloud(self):
        """reset --cloud deletes the player's cloud data (only when PFGS_ALLOW_RESET=1)."""
        if not config.reset_allowed():
            self.skipTest("Set PFGS_ALLOW_RESET=1 to run the destructive cloud reset test.")

        # Upload something, then reset cloud, then confirm info shows no manifests.
        source = self.tmp / "save"
        _make_sample_save(source)
        self._upload(source)

        config.run_cloud("reset", ["--cloud", "--force"])

        proc = config.run_cloud("info", ["--json"])
        start = proc.stdout.find("{")
        self.assertGreaterEqual(start, 0)
        data = json.loads(proc.stdout[start:])
        self.assertEqual(len(data.get("manifests", [])), 0, "Cloud reset did not remove all manifests")


if __name__ == "__main__":
    import run_tests
    sys.exit(run_tests.main())

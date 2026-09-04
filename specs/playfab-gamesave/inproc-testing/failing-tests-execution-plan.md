# Failing Tests — Execution Plan

**Date:** 2026-05-11  
**Author:** Bishop (AI Tester)  
**Source:** `combine-xplat-results.py --since 70 --failing` (61 tests, 114 passes analyzed)

---

## Summary

43 distinct failing tests need verification. Even tests with existing bug reports will be
re-run and re-analyzed using the debugging guide's ETL/log patterns to confirm root causes
are accurately documented.

---

## Approach

Work through failing tests one at a time. For each test:

1. **Design** — Write or fix test YAML based on test 125 patterns
2. **Build & Run** — `py tests-run.py gamesave-pc --only {N} --timeout 300`
3. **Debug** — If test fails unexpectedly, analyze logs + ETL, iterate
4. **Vet** — Capture ETL proof of the specific behavior being tested
5. **Report** — Write result to `status/failing-tests/`:
   - **PASS** → Report with log/ETL proof that behavior is correct
   - **FAIL (expected bug)** → Bug report with evidence chain
   - **FAIL (test issue)** → Fix test and retry
6. **Move on** — Only after hitting the edge (working proof or confirmed bug)

After each test: document key learnings in debugging guide (`debug-failing-tests-plan.md`) and/or `ai-grts-internals.md`.

---

## Tier 1 — Stable Failures, Never Passed (highest value investigation)

| # | Test Name | Rate | Pattern | Notes |
|---|-----------|------|---------|-------|
| 03 | Background Upload | 0% (0/4) | XXXX | Bug filed — re-verify with ETL |
| 112 | Cross-Platform Rollback | 0% (0/7) | XXXXXXX | Never passed, 18-47s runs |
| 121 | Cross-Platform Version Ping-Pong (isWinner) | 0% (0/1) | ???X | isWinner variant, only 1 real run |
| 125 | TakeRemote Next Save isWinner Stomp | 0% (0/1) | X | Known isWinner stomp — server fix coming |

---

## Tier 2 — Stable Failures, Previously Categorized (re-verify with ETL)

| # | Test Name | Rate | Pattern | Previous Category |
|---|-----------|------|---------|------------------|
| 12 | Contention Go Back | 0% (0/1) | X | Cross-engine isolation |
| 13 | Contention Cancel | 0% (0/1) | X | Cross-engine isolation |
| 16 | Conflict Play Offline | 0% (0/3) | XXX | Cross-engine isolation |
| 23 | Delete Both Sides No Conflict | 0% (0/2) | XX | Cross-engine isolation |
| 24 | Multi-Callback Contention Then Conflict | 0% (0/1) | X | Cross-engine isolation |
| 33 | Cancel During Download | 0% (0/3) | XXX | AutoNavigateGameSaveUi unsupported |
| 34 | Cancel During Upload | 0% (0/3) | XXX | AutoNavigateGameSaveUi unsupported |
| 37 | Descriptor Contention | 0% (0/1) | X | GRTS descriptor limitation |
| 38 | Descriptor Conflict | 0% (0/2) | XX | GRTS descriptor limitation |
| 39 | Thumbnail Descriptor | 0% (0/1) | X | GRTS thumbnailUri empty |
| 40 | Get Remaining Quota | 0% (0/1) | X | GRTS quota cache stale |
| 41 | Thumbnail Absent | 0% (0/1) | X | GRTS descriptor limitation |
| 43 | Out-of-Storage Required Bytes | 0% (0/3) | XXX | GRTS storage limitation |
| 44 | Storage Full Init | 0% (0/3) | XXX | Harness can't simulate |
| 45 | Storage Full Download | 0% (0/3) | XXX | Harness can't simulate |
| 48 | Out-of-Storage Cancel | 0% (0/3) | XXX | Harness can't simulate |
| 80 | Network Flapping During Sync | 0% (0/3) | XXX | Timeout/retry failure |
| 87 | Custom File Location Invalid | 0% (0/1) | X | Returns S_OK (no validation) |
| 89 | GRTS Error Codes | 0% (0/2) | XX | E_FAIL instead of specific code |
| 113 | Two-Device SPOP | 0% (0/1) | X | Requires Xbox engine |

---

## Tier 3 — Regressed or Mostly-Fail (worked before, now broken)

| # | Test Name | Rate | Pattern | Notes |
|---|-----------|------|---------|-------|
| 102 | TCUI Dialog Closes on Termination | 33% (1/3) | .XX | Passed pass71, now 190s timeout |
| 109 | Cross-Platform Contention | 14% (1/7) | XXXXXX. | Latest pass130 PASSED |
| 110 | Cross-Platform Conflict | 14% (1/7) | XXXXXX. | Latest pass130 PASSED |

---

## Tier 4 — Flaky (intermittent pass/fail)

| # | Test Name | Rate | Pattern | Notes |
|---|-----------|------|---------|-------|
| 26 | SyncFailed Retry | 30% (3/10) | X?XXXXX... | Latest 3 runs passing |
| 28 | Upload Offline | 50% (1/2) | X. | Latest passing |
| 29 | Get Quota Offline | 50% (1/2) | X. | Latest passing |
| 30 | Return Online No Reinit | 50% (1/2) | X. | Latest passing |
| 31 | SyncFailed Error Specificity | 50% (1/2) | X. | Latest passing |
| 42 | Description Round-Trip Conflict | 33% (3/9) | .XXX.XXX. | Intermittent ~258s timeout |
| 46 | Storage Full Upload | 25% (2/8) | XXX.XXX. | Latest pass120 PASS |
| 47 | Cloud Quota Exhaustion | 25% (2/8) | XXX.XXX. | Latest pass120 PASS |
| 49 | Out-of-Storage Cancel Upload | 33% (3/9) | .XXX.XXX. | Latest pass120 PASS |
| 127 | TakeLocal Server Flip | 50% (1/2) | X. | Conflict block — pass178 PASS |
| 134 | Two GRTS Both TakeRemote | 50% (1/2) | X. | Expected: contention not conflict |
| 143 | Large Version Gap | 50% (1/2) | X. | pass182 PASS with dual-upload |

---

## Tier 5 — Conflict Block Tests (YAML update + re-verify)

| # | Test Name | Status | Action |
|---|-----------|--------|--------|
| 127 | TakeLocal Server Flip | PASS (pass178) | Update YAML → passing, verify |
| 134 | Two GRTS Both TakeRemote | EXPECTED (contention) | Update YAML → ignore, verify |
| 135 | Upload Failure Retry | UNTESTABLE (Rule 15) | Update YAML → ignore, verify |
| 138 | Cross-Container Leak | DESIGN (no leak) | Update YAML → ignore, verify |
| 139 | KeepActive Reinit Conflict | PARTIAL PASS (isWinner) | Keep failing + comment, verify |
| 143 | Large Version Gap | PASS (pass182) | Update YAML → passing, verify |
| 144 | HTTP 409 Handling | EXPECTED (Rule 23) | Keep failing + comment, verify |

---

## Execution Order

1. **Tier 5** — YAML updates for conflict block tests (quick wins, already investigated)
2. **Tier 1** — Test 112 → 121 → 03 → 125 (highest-value stable failures)
3. **Tier 3** — Test 102 → 109/110 (regressions — something changed)
4. **Tier 2** — Re-verify each with ETL. Group by category: cross-engine (12,13,16,23,24), cancel (33,34), descriptor (37-41,43), storage (44,45,48), other (80,87,89,113)
5. **Tier 4** — Flaky tests: confirm stability or triage (26,28-31,42,46,47,49)

After each test: update this plan with results, add learnings to debugging guide / ai-grts-internals.md.

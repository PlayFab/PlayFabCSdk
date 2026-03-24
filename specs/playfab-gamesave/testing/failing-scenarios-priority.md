# Failing Test Scenarios - Priority Analysis

This document lists the 13 test scenarios that are not yet passing and prioritizes them based on importance to user-facing functionality and risk of real bugs.

## High Priority

Core functionality tests that are most likely to catch real bugs affecting users.

| Scenario | Name | Description |
|----------|------|-------------|
| scenario-08 | Multi-Atomic Conflict Decision | Tests all-or-nothing conflict resolution with multiple atomic units |
| scenario-09 | Rollback to Last Known Good | DeviceA publishes baseline, good patch, then bad release - tests recovery |
| scenario-10 | Rollback to Last Conflict | DeviceA seeds baseline, DeviceB downloads, DeviceA publishes cloud update |
| scenario-14 | Concurrent Upload Arbitration | DeviceA holds lock, DeviceB tries to sync and encounters contention |
| scenario-20 | Manifest Corruption Detection | Corrupt local manifest, verify detection and recovery via cloud redownload |

## Medium Priority

Important edge cases that affect user experience but are less common.

| Scenario | Name | Description |
|----------|------|-------------|
| scenario-12 | Out-of-Storage UI Handling | Fill disk, trigger callback, test Retry after freeing space and Cancel |
| scenario-15 | Disk Exhaustion Recovery | Fill disk during download, trigger recovery, verify resume after space freed |
| scenario-17 | Quota Limit Handling | Upload data exceeding title quota, verify graceful failure with quota error |
| scenario-21 | Interrupted Download Resume | Start large download, interrupt mid-transfer, verify resumption |
| scenario-16 | Large Payload Incremental Sync | Upload 10MB, modify single file, verify incremental sync uploads only delta |

## Lower Priority

Stress tests and edge cases that are nice to have but not critical for initial release.

| Scenario | Name | Description |
|----------|------|-------------|
| scenario-18 | High File Count Upload | Upload 5000 small files, verify manifest integrity and download consistency |
| scenario-19 | Filesystem Boundary Validation | Test special characters, long paths, Unicode cross-platform compatibility |
| scenario-25 | Long-Running Sync Soak | 10 iterations of upload/download cycles, verify no memory leaks |

## Recommendations

1. **Start with High Priority**: Focus on scenarios 8, 9, 10, 14 (conflict/contention handling) as these test the most critical multi-device synchronization logic.

2. **Corruption Detection**: Scenario 20 (manifest corruption) is important for data integrity and user trust.

3. **Medium Priority Next**: Once high priority tests pass, tackle storage and quota handling (12, 15, 17) as these affect user experience during edge cases.

4. **Defer Stress Tests**: Scenarios 18, 19, 25 are valuable but time-consuming. Consider running these in nightly builds rather than PR validation.

## How to Mark a Scenario as Passing

Add the `passing` tag to the scenario's `tags` list:

```yaml
tags:
  - passing
  - gamesaves
```

Scenarios with the `passing` tag are included in the automated test runs via `--run-tag passing`.


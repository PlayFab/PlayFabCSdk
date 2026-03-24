# PFGameSave Test Automation Delivery Plan

**Document Version:** 1.0  
**Last Updated:** December 2025  
**Owner:** Jason  
**Status:** Draft for Review

## Executive Summary

This document outlines the delivery plan for the PFGameSave test automation framework. The goal is to deliver a robust, maintainable automation infrastructure that enables comprehensive cross-platform testing of the PlayFab GameSave SDK.

---

## Ordered Work Items

The following work items are listed in priority order. 

---

## Milestone 1 – Mid-December 2025

### Test Controller Application [Complete]
- C# WinForms application with WebSocket server on port 5000
- YAML scenario loading and execution engine
- Device connection management and assignment
- Real-time logging and result collection
- CLI mode for manual API invocation (per Pushpadant's suggestion)

### Test Device Application (Windows) [Complete]
- C++ application receiving commands via LHC WebSocket
- Command handler registry supporting all PFGameSave APIs
- Automatic controller reconnection
- File snapshot and hash verification support
- LHC-based mock injection for network failure simulation for offline/reconnect scenarios
- Bulk of logic in cross-platform `Common/` folder; platform-specific code in `Platform/Windows/`

### Scenario 1: Single-Device Golden Path Sync [Complete] 
- Runs end-to-end on Windows PC with force in-process registry key enabled
- Validates: init → AddUser → write files → upload → restart → download → verify hashes

### Scenario 2: Two-Device Golden Path Sync [Complete] 
- Runs end-to-end on 2 Windows PC both with force in-process registry key enabled
- Device A uploads, Device B downloads and verifies
- Validates cross-device sync with SHA hash comparison

### Scenario 3: Basic Offline-to-Online Reconnect [Complete] 
- Runs end-to-end on Windows PC with force in-process registry key enabled
- Uses HTTP mocks to simulate network failure
- Validates offline mode entry, local save, and reconnect upload

### Scenario 4: Active Device Release and Reacquire [Complete]
- Device A holds active lock, Device B requests via contention callback
- Validates SyncLastSavedData response and lock handoff

### Scenario 5: Large Cloud Dataset Initialization [Complete] 
- ~100 MB existing cloud data, ~1000 files
- Validates bulk download and manifest hydration

### PlayStation Test Device Port Kickoff [Complete]
- Port guide delivered to team with access to PlayStation
- *See [5-linux-port-guide.md](5-linux-port-guide.md) for implementation details*
- *Port work performed by team with access to PlayStation*

### Internal Documentation [Complete]
- Test automation plan 
- Controller and device specification
- PlayStation port guide
- This delivery plan (this document)

---

## Milestone 2 – End of January 2026

### ADO Pipeline for Automated Test Execution
- ADO pipeline running Scenarios 1-5 autonomously without manual intervention
- Pipeline triggered on PR/commit or scheduled nightly runs
- Test results integrated into ADO reporting (pass/fail gating)
- Artifact collection (logs, snapshots) on failure
- *Requirement for handoff: full automation matching PlayFab SDK test quality bar*

---

## Milestone 3 – End of Feb 2026

### Platform Coverage: PC GRTS and Xbox Console
- Fix any issues running test device on PC GRTS (out-of-process service)
- Fix any issues running test device on Xbox Console
- Run Scenarios 1-5 on both platforms
- Cross-platform sync validation: Xbox ↔ Windows PC

### Additional P1 Scenarios (6-15)
- Scenario 6: Conflict Resolution – Local Wins
- Scenario 7: Conflict Resolution – Cloud Wins
- Scenario 8: Multi-Atomic Conflict Decision
- Scenario 9: Rollback to Last Known Good
- Scenario 10: Rollback to Last Conflict
- Scenario 11: Upload Interruption Recovery
- Scenario 12: Out-of-Storage UI Handling
- Scenario 13: User-Canceled Sync/Upload Recovery
- Scenario 14: Concurrent Upload Arbitration
- Scenario 15: Mid-Operation Disk Exhaustion

### Steam Platform Support
- Steam PC scenarios operational
- Steam Deck scenarios

### PlayStation Test Device Port Implementation
- Create platform files in `Platform/PlayStation/` (logging, hashing, user sign-in, app lifecycle)
- Reuse all `Common/` code unchanged
- Validate with Scenarios 1-5 on target hardware
- *Port work performed by team with access to PlayStation*

### Bug Fixes
- Address any SDK or platform issues uncovered by test scenarios

---

## Milestone 4 – End of March 2026

### P2/P3 Scenarios (16-27)
- Scenario 16: Large Payload Incremental Sync
- Scenario 17: Quota Limit Handling
- Scenario 18: High File Count Upload
- Scenario 19: Filesystem Boundary Validation
- Scenario 20: Manifest Corruption Detection
- Scenario 21: Interrupted Download Resume
- Scenario 22: File/Folder Deletion Propagation
- Scenario 23: Mutating During Upload Guard
- Scenario 24: Online-to-Offline Recovery
- Scenario 25: Long-Running Sync Soak
- Scenario 26: Set Save Description
- Scenario 27: Progress Cancel

### Cross-Platform Test Matrix
- Bidirectional testing between all supported platform pairs
- Platform-agnostic scenario execution validated
- Results documented per platform combination

### Bug Fixes
- Address any SDK or platform issues uncovered by test scenarios

### Vendor Resource Integration (UI-Focused Scenarios)
- Required only for scenarios requiring manual UI interaction (e.g., Scenario 12: Out-of-Storage UI Handling)
- Runbook for manual execution of UI-dependent tests
- Log collection and artifact packaging procedures
- Bug filing template with required diagnostic info

---

## Resource Requirements

### Hardware

| Resource | Quantity | Purpose |
|----------|----------|---------|
| Windows PC (controller) | 1 | Runs test controller application |
| Windows PC (device) | 2 | Test device targets |
| Xbox console | 2 | Xbox GDK testing |
| Steam Deck | 1 | Steam Deck in-process testing |
| PlayStation dev kit | 2 | PlayStation port validation |

### Personnel

| Role | Team | Involvement |
|------|------|-------------|
| Framework developer | Jason | Primary development until handoff |
| Handoff owner | Victor's team | Ramp-up and eventual ownership |
| Vendor resources | External | Test execution |
| Xbox Platform team liaison | Kelly's team | Bug triage and Xbox/Windows issues |
| PlayStation port | Victor's team / Turn10 | Port work |

---

## Appendix: Related Documents

- [1-test-automation-plan.md](1-test-automation-plan.md) - Detailed scenario specifications
- [2-test-controller-and-device-spec.md](2-test-controller-and-device-spec.md) - Technical architecture
- [3-pf-gamesave-test-harness.md](3-pf-gamesave-test-harness.md) - Harness implementation details
- [5-linux-port-guide.md](5-linux-port-guide.md) - Platform porting instructions

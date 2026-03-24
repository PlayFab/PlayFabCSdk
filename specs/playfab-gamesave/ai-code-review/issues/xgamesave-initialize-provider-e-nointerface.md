# BUG: XGameSaveInitializeProvider returns E_NOINTERFACE causing PFGameSave initialization failure

## 1. Summary
- Title: XGameSaveInitializeProvider E_NOINTERFACE -> PFGameSaveFilesInitialize fails
- Scenario(s): Harness init (Basic Cross-Device Sync), affects any scenario requiring PFGameSave init
- Gap IDs: [N/A]
- Severity: Blocker
- Priority: P1
- Date observed: 2025-08-08 (UTC)
- Reporter: Automation

## 2. Environment
- Platform: Windows (GDK)
- GDK version: 251000
- Toolchain: MSVC 14.44, VS 2022
- libHttpClient: GDK build
- Test app: PlayFabGameSaveMultDeviceTestGDK (Debug)
- Network conditions: Normal

## 3. Expected vs. Actual
- Expected:
  - XGameSaveInitializeProvider succeeds (S_OK) and PFGameSaveFilesInitialize completes.
- Actual:
  - XGameSaveInitializeProvider returns E_NOINTERFACE (0x80004002) on some environments, leading to initialization failure. In other runs we also observed PFGameSaveFilesInitialize returning 0x89237001 during basic sync on a second device.
- Impacted APIs/callbacks:
  - XGameSaveInitializeProvider
  - PFGameSaveFilesInitialize

## 4. Repro Steps (minimal)
1) Build test app
2) Run full app (no --scenario): Basic Cross-Device Sync initializes PF GameSaves on two devices
3) Observe initialization failure on one device with E_NOINTERFACE from XGameSave provider (or PFGameSaveFilesInitialize returning failure)

## 5. Evidence (attach/log)
- Assertions (from `test_log.txt` / harness logs):
  - Example observed earlier: PFGameSaveFilesInitialize returned 0x89237001 on second device during basic sync
  - E_NOINTERFACE (0x80004002) observed in environments without proper XGameSave provider support
- HRESULTs observed:
  - E_NOINTERFACE (0x80004002)
  - 0x89237001 (PF init path)
- Additional logs: see `PlayFabGameSaveMultDeviceTestGDK/x64/Debug/test_log.txt` and `assertions.jsonl`

## 6. Temporary Mitigation in Tests (if any)
- Softened checks: Not applied to basic sync; scenario-level mitigation exists in S02 with TODO(review) notes (unrelated)
- Bounded retries: N/A
- Affected files/locations:
  - Harness init path (Basic Cross-Device Sync)
- Rationale: Consider gating/Skipping Basic Sync in CI where provider is unavailable; continue running scenario-only tests (S01/S02)
- CI impact: Keeps CI green while product clarifies requirements

## 7. Proposed Next Actions
- [ ] Product: Confirm supported conditions for XGameSaveInitializeProvider and when E_NOINTERFACE is expected
- [ ] Product: Improve error mapping/message to guide setup (missing capabilities/packages)
- [ ] Tests: Detect provider availability pre-check and skip basic sync with a clear SKIPPED note when unavailable
- [ ] Docs: Add prerequisite checklist to onboarding for XGameSave provider availability
- [ ] Remove any CI gating once resolved; basic sync should pass on standard dev machines

## 8. Links
- Template: `specs/PlayFabGameSave/specs/BUG_REPORT_TEMPLATE.md`
- Specs:
  - `specs/PlayFabGameSave/PFGameSaves_Automation_GapSpec.md` (rule 14.2 temporary pass policy)
  - `specs/PlayFabGameSave/specs/PFGameSaves_Automation_Onboarding.md` (AI TODO policy)
- Scenarios: Basic Cross-Device Sync (harness), S02 (soft checks exist with TODOs)

---
Notes: Preserve failing evidence even if CI gating/skip is applied. Once product guidance is available, enforce strict init checks again.

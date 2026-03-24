# PFGameSave Behavior Deviation Bug Report

Use this template to file product issues discovered by automation. Link this report from all related `TODO(review)` comments in scenarios.

## 1. Summary
- Title: <short, specific>
- Scenario(s): Sxx [, Syy]
- Gap IDs: [##, ##]
- Severity: [Blocker | High | Medium | Low]
- Priority: [P1 | P2]
- Date observed: <UTC>
- Reporter: <name/alias>

## 2. Environment
- Platform: Windows (GDK)
- GDK version: <e.g., 251000>
- Toolchain: MSVC <version>, VS <edition>
- libHttpClient: <version/build if known>
- Test app: `PlayFabGameSaveMultDeviceTestGDK` (Debug/Release)
- Network conditions: <Normal | Simulated offline | Fault injection>

## 3. Expected vs. Actual
- Expected behavior:
  - <cite from public spec/headers>
- Actual behavior:
  - <what happened>
- Impacted APIs/callbacks:
  - <e.g., PFGameSaveFilesIsConnectedToCloud, PFGameSaveFilesGetRemainingQuota>

## 4. Repro Steps (minimal)
1) Build test app
2) Run scenario(s): `--scenario=Sxx`
3) Observe <specific assertion/log>

## 5. Evidence (attach/log)
- Assertions (from `assertions.jsonl`):
  - Code(s): <e.g., UNEXPECTED, NOT_OFFLINE, QUOTA_UNVERIFIED>
  - Sample lines:
    - { "scenario":"Sxx", "gaps":[..], "pass":false, "code":"...", "detail":"...", "ts":"..." }
- HRESULTs observed: <list with hex>
- Additional logs/screenshots: <paths or inline>

## 6. Temporary Mitigation in Tests (if any)
- Softened checks: <yes/no>
- Bounded retries added: <yes/no; N attempts, delay>
- Affected file(s)/location(s):
  - `PlayFab.C/Test/PlayFabGameSaveMultDeviceTestGDK/Scenario_Sxx_*.cpp` lines <refs>
- Rationale: <why mitigated>
- CI impact: <test now passes; unexpected states still logged>

## 7. Proposed Next Actions
- [ ] Product confirm contract/spec for <area>
- [ ] Implement product fix OR update public spec
- [ ] Remove test softening and retries; restore strict assertion(s)
- [ ] Close linked TODOs and this bug

## 8. Links
- Commit/PR with TODOs: <sha/PR>
- Related specs:
  - `PFGameSaves_Automation_GapSpec.md` (rule 14.2)
  - `PFGameSaves_Automation_Onboarding.md` (AI TODO policy)
- Related scenarios/tests: <list>

---
Notes: Keep this report concise with direct pointers (file+line, assertion codes). Preserve failing evidence even if the test temporarily passes.

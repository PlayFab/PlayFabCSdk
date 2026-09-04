# PlayFab SDK Debugging Guide

> Reference for SDK engineers debugging PlayFab SDK issues — libHttpClient, PFCore, PFServices, authentication, or service integration problems.
> Covers investigation methodology, hypothesis-driven root cause analysis, error code reference, and timeline reconstruction.

### Quick Navigation

| Section | Topic | Use When |
|---------|-------|----------|
| **§0** | Investigation Methodology | Starting any investigation — read this first |
| **§1** | Data Sources | Understanding what data is available for non-GameSave issues |
| **§2** | Error Code Reference | HRESULT codes, HTTP status, PlayFab error JSON, diagnostic tree |
| **§3** | Timeline Reconstruction | Correlating logs across components |
| **§4** | HTTP Debugging | libHttpClient request/response tracing |
| **§5** | Past Investigation Log | Symptom → root cause lookup from completed investigations |
| **§6** | Known Wrong Turns | Dead-end paths to avoid repeating |

### Local Source Paths

| Component | Path | What's Here |
|-----------|------|-------------|
| **PlayFab SDK Source** | `C:\git\PlayFab.C\Source` | PFCore, PFServices, PFGameSave, PFMultiplayer, etc. |
| **libHttpClient** | `C:\git\libHttpClient` | HTTP layer — request/response pipeline, retry logic, websockets |
| **GRTS (ConnectedStorage)** | `C:\git\ConnectedStorage` | Gaming Runtime Transport Service — OS-level save transport |
| **GameSave Service** | `C:\git\GS.GameSave` | Server-side GameSave service |
| **PlayFab SDK Tests** | `C:\git\PlayFab.C\Test` | Test controller, test devices, test scenarios |
| **PlayFab SDK Specs** | `C:\git\PlayFab.C\specs` | Design specs, test strategies, debugging guides |

### Key Source Locations

| Area | Path | What to Look For |
|------|------|------------------|
| PFCore (init, auth, entity) | `Source\PlayFabCore\Source\` | `PFInitialize`, `PFServiceConfig`, entity token management |
| PFServices (API wrappers) | `Source\PlayFabServices\Source\` | Generated API wrappers, request/response serialization |
| libHttpClient core | `libHttpClient\Source\HTTP\` | `HCHttpCallPerformAsync`, retry logic, mock infrastructure |
| libHttpClient platform | `libHttpClient\Source\HTTP\WinHttp\` | WinHTTP integration, TLS, proxy handling |
| Authentication (XAL) | `Source\PlayFabCore\Source\Authentication\` | Login flows, token refresh, entity token caching |

---

## 0. Investigation Methodology — ALWAYS Follow This Process

> **This section is MANDATORY.** Every investigation — test failure, customer report, or service incident — follows this two-phase process. No exceptions.

### Why Two Phases?

Confirmation bias kills investigations. When you simultaneously collect facts AND guess root causes, you unconsciously filter evidence to fit your first guess. The fix: **separate what happened from why.**

- **Phase 1 — Fact-Finding Report:** Collect ONLY undisputable facts. No interpretation. No speculation. No root cause guessing. This produces a shareable document for the vteam.
- **Phase 2 — Root Cause Analysis:** Using the fact document, form hypotheses and test them against code. Each hypothesis is explicitly tracked as confirmed, rejected, or inconclusive.

### The Iterative Loop

This is NOT waterfall. Phase 2 will expose gaps in your fact document. When that happens:

1. Go back to Phase 1. Add the new facts.
2. Resume Phase 2 with the enriched fact base.

**This is expected and healthy — most investigations require 2–3 iterations.** The fact document is a LIVING document that gets enriched as Phase 2 exposes gaps.

### Investigation Output Files

All investigation artifacts go in a dated directory:

```text
investigations/{YYYY-MM-DD}-{slug}/
├── fact-report.md           # Phase 1 output
└── root-cause-analysis.md   # Phase 2 output
```

Example: `investigations/2026-05-12-lhc-retry-hang/fact-report.md`

### Phase 1: Fact-Finding Report

#### Fact Document Template

Copy this template into `investigations/{date}-{slug}/fact-report.md` and fill in each section. **Do NOT skip sections — mark them "NOT AVAILABLE" if data is missing.**

```markdown
# Investigation: {Brief Title}
**Date:** {date}
**Component:** {PFCore | PFServices | libHttpClient | Authentication | Other}
**Source:** {test failure | customer report | internal report}
**Reporter:** {who reported it}
**Investigator(s):** {who is investigating}

## F1. Bug Report
Rewrite the original bug report into a clear, self-contained description. Do NOT link to external files
or paste raw email/chat — distill it into something anyone can read cold and understand immediately.
Include:
- What the user/test was doing (step by step)
- What was expected to happen
- What actually happened
- Repro steps (if known)
- Error codes, HRESULTs (if any)
- Platform (Windows, Xbox, iOS, Android, Switch)
- SDK version, libHttpClient version

## F2. Timeline
Chronological events from ALL available data sources, correlated by timestamp.
Format: `[HH:MM:SS.mmm] [Source] Event description`
Sources: SDK logs, HTTP traces, test output, service logs

## F3. SDK / Application Logs
Relevant log excerpts. Include:
- File path where logs were found
- Key log lines (with timestamps)
- Any error messages or unexpected state transitions

## F4. HTTP Traces (if applicable)
HTTP request/response details:
- URL, method, headers (redact auth tokens)
- Response status code, body (summarized)
- Timing (request sent → response received)
- Retry attempts and outcomes

## F5. Service-Side Data (if available)
Any service logs, PlayFab dashboard data, or Kusto queries. Include:
- The exact query used
- Key results (summarized)
- Time range queried

## F6. What Worked Correctly
Parts of the pipeline that functioned as expected. This narrows the search space.
Example: "Init succeeded, auth token obtained, first API call returned 200 — failure on second call with stale token"

## F7. What's Missing
Data gaps — things you looked for but couldn't find or weren't available.
This itself is valuable information for Phase 2.
```

### Phase 2: Root Cause Analysis

#### Root Cause Analysis Template

Copy this template into `investigations/{date}-{slug}/root-cause-analysis.md`. Each hypothesis is stated, tested against facts, and explicitly marked.

```markdown
# Root Cause Analysis: {Brief Title}
**Fact Report:** [fact-report.md](./fact-report.md)
**Date:** {date}
**Investigator(s):** {who is investigating}

## R1. Hypotheses

### Hypothesis 1: {Brief description}
**Based on facts:** {which facts from the fact report support investigating this}
**Test:** {how to confirm/reject — code path to check, query to run, test to reproduce}
**Result:** CONFIRMED | REJECTED | INCONCLUSIVE
**Evidence:** {what you found}

### Hypothesis 2: {Brief description}
**Based on facts:** {which facts from the fact report support investigating this}
**Test:** {how to confirm/reject — code path to check, query to run, test to reproduce}
**Result:** CONFIRMED | REJECTED | INCONCLUSIVE
**Evidence:** {what you found}

(Repeat for each hypothesis. Do not stop at one — even if the first looks right, check alternatives.)

## R2. Root Cause
{Only written when a hypothesis is CONFIRMED. Reference the hypothesis number.}

## R3. Recommended Fix
{What should change — code, config, process. Be specific: file paths, function names, behavioral change.}

## R4. Prevention
{How to prevent recurrence — test coverage, monitoring, assertions, telemetry alerts.}
```

### Hypothesis Discipline

- **State each hypothesis explicitly** before testing it. No "I think it might be..." in your head — write it down.
- **Test against facts, not intuition.** Every hypothesis check references specific facts from the Phase 1 document.
- **Mark the result.** CONFIRMED, REJECTED, or INCONCLUSIVE. Inconclusive means you need more facts — loop back to Phase 1.
- **Note what worked correctly.** Narrowing the search space is just as valuable as finding the bug.
- **Don't stop at one hypothesis.** Even if Hypothesis 1 looks right, briefly consider alternatives. Confirmation bias is real.

### When to Loop Back to Phase 1

Return to Phase 1 and add new facts when:

- A hypothesis test reveals you're missing a data source (e.g., you checked client logs but never pulled service logs)
- You find a timeline gap — events jump from success to failure with nothing in between
- The code path you're investigating references state you haven't captured
- Your hypothesis is INCONCLUSIVE because the fact document doesn't cover the relevant area

Add the new facts to the existing fact document (it's a living document), then resume Phase 2.

### Reproduce with a Test

Once a hypothesis is CONFIRMED (or strongly suspected), try to reproduce the bug with a test. A reproducing test is the strongest possible evidence — it proves the failure is real and gives you a regression gate for the fix.

1. **Find existing tests** that exercise the same code path — check `C:\git\PlayFab.C\Test\` for the relevant component
2. **Create a minimal repro** that triggers the exact failure condition
3. **Confirm the failure** — run WITHOUT any fix; the test must fail with the same symptom
4. **After fixing** — verify the test passes and add it as a regression test

---

## 1. Data Sources

Different components have different diagnostic data available:

### libHttpClient

| Source | What It Shows | How to Get It |
|--------|--------------|---------------|
| **HCTraceSetTraceToDebugger** | HTTP request/response lifecycle, retry decisions, mock behavior | Enable in test code; appears in debug output |
| **HTTP mock infrastructure** | Injected responses for testing error paths | `HCMockCallCreate` / `HCMockAddMock` in test code |
| **WinHTTP traces** | Low-level HTTP (TLS handshake, proxy, connection pooling) | Windows ETW: `Microsoft-Windows-WinHTTP` provider |
| **Fiddler / mitmproxy** | Full HTTP request/response capture | Configure proxy; requires TLS interception for HTTPS |

### PFCore / PFServices

| Source | What It Shows | How to Get It |
|--------|--------------|---------------|
| **PlayFab debug logs** | API call lifecycle, serialization, token management | Enable via `PFDebugSetTraceLevel` |
| **PlayFab Admin API** | Server-side view of entity state, title config | PlayFab Game Manager or Admin API calls |
| **Kusto (PlayFabInternal)** | Server-side telemetry for API calls | Query `playfabinternalreader.westus2.kusto.windows.net` |

### Authentication

| Source | What It Shows | How to Get It |
|--------|--------------|---------------|
| **XAL traces** | Token acquisition, refresh, failure | XAL ETW provider or debug logging |
| **Entity token** | Token expiry, claims, entity type | Decode JWT from `PFAuthenticationGetEntityTokenAsync` result |
| **Service auth logs** | Server-side auth validation | Kusto query for auth failures by entity ID |

---

## 2. Error Code Reference: HRESULT Mapping & Diagnostic Paths

### 2.1 Common PlayFab SDK Error Codes

| Code | Name | Meaning | Diagnostic Path |
|------|------|---------|-----------------|
| 0x80004005 | E_FAIL | Generic failure | Check underlying HTTP status or detailed error message |
| 0x80070005 | E_ACCESSDENIED | Permission denied | Check auth token validity; verify entity permissions |
| 0x80070057 | E_INVALIDARG | Invalid argument passed to API | Check API parameters; review docs for constraints |
| 0x800704c7 | ERROR_CANCELLED | Operation was cancelled | Check if caller cancelled, or if timeout triggered |
| 0x80072ee2 | WININET_E_TIMEOUT | HTTP request timed out | Check network connectivity; increase timeout |
| 0x80072f78 | WININET_E_INVALID_SERVER_RESPONSE | Invalid/malformed response from server | Check for proxy/TLS interception, middleboxes, or server misbehavior; capture WinHTTP ETW + full HTTP trace |
| 0x80070780 | ERROR_NOT_FOUND | Resource not found | Verify entity/resource exists on server |
| 0x800705b4 | ERROR_TIMEOUT | Operation timed out | Check client/service timeouts; verify server reachability; investigate network latency |
| 0x800703e4 | ERROR_IO_INCOMPLETE | Overlapped I/O incomplete | Check for interrupted/partial I/O; verify network stability; retry if appropriate |

### 2.2 HTTP Status Codes

| HttpStatus | Meaning | Root Cause | Retry Strategy |
|-----------|---------|-----------|-----------------|
| 200 | OK | Success | N/A |
| 400 | Bad Request | SDK sent malformed request (bug) | DO NOT RETRY; escalate |
| 401 | Unauthorized | Token invalid/expired | RETRY after re-auth; check token endpoint |
| 403 | Forbidden | Permission denied (`NotAuthorized`) OR rate-limited (verify via PlayFab `error`/`errorCode` and/or `Retry-After`) | DO NOT RETRY for permission failures; RETRY with exponential backoff only when a documented rate-limit signal is present |
| 404 | Not Found | Resource doesn't exist | Verify entity/resource ID; check title config |
| 409 | Conflict | Concurrent modification | RETRY with latest state; check for race conditions |
| 429 | Too Many Requests | Rate limited | RETRY with exponential backoff; check call frequency |
| 500 | Internal Server Error | Server bug (transient or persistent) | RETRY with exponential backoff; check service status |
| 503 | Service Unavailable | Server overloaded or maintenance | RETRY with exponential backoff; check scheduled maintenance |

### 2.3 Diagnostic Decision Tree

1. Is it a PlayFab-specific error code (0x8923...)?
   YES: Check PlayFab error documentation for the specific code
   NO: Continue to 2

2. Is it a Windows HRESULT (0x8007... or 0x8002...)?
   YES: Go to 2.1 table; check if it maps to a known HTTP or system issue
   NO: Continue to 3

3. Check HTTP response status:
   - If non-200 status: Go to 2.2 table
   - If no response at all: Network connectivity issue — check DNS, proxy, firewall
   - If timeout: Check network quality, server health, SDK timeout configuration

4. If root cause not obvious from error code or HTTP status:
   - Enable verbose logging (`HCTraceSetTraceToDebugger`, `PFDebugSetTraceLevel`)
   - Capture full HTTP request/response with Fiddler or equivalent
   - Check Kusto for server-side errors matching the timestamp
   - Escalate to service team with error code + timestamp + repro steps

### 2.4 Reading a PlayFab API Error Response

Every PlayFab API error returns a JSON body with a consistent structure. Knowing which field to look at saves time:

```json
{
  "code": 400,
  "status": "BadRequest",
  "error": "InvalidParams",
  "errorCode": 1000,
  "errorMessage": "Invalid input parameters",
  "errorDetails": {
    "Files": ["The field Files must be a string or array type with a maximum length of '10'."]
  }
}
```

| Field | What It Tells You | How to Use It |
|-------|-------------------|---------------|
| `code` | HTTP status code (same as response header) | Use §2.2 table for retry strategy |
| `status` | Human-readable HTTP status name | Informational only |
| `error` | PlayFab error name (e.g., `InvalidParams`, `NotAuthenticated`, `ServiceUnavailable`) | **Primary identifier** — search PlayFab docs or SDK source for this string |
| `errorCode` | Numeric PlayFab error code (e.g., 1000, 1207, 1414) | **Most specific** — unique per error condition; use for programmatic matching |
| `errorMessage` | Human-readable description | Read this first for quick understanding; may contain parameter names or limits |
| `errorDetails` | Per-field validation errors (only on 400s) | Shows exactly which parameter failed and why; not always present |

**Key distinctions:**
- **`error` vs `errorCode`**: `error` is a string category (e.g., `InvalidParams` covers many cases), `errorCode` is the specific numeric code (e.g., 1000 = invalid params, 1205 = entity not found). Always log both.
- **Rate limiting**: Returns `429` with `error: "APIConcurrentRequestLimitExceeded"` or `error: "TooManyRequests"`. The `Retry-After` header (if present) tells you how long to wait.
- **Auth failures**: `401` with `error: "NotAuthenticated"` means the entity token is expired or invalid. `403` with `error: "NotAuthorized"` means the token is valid but lacks permission.
- **No `errorDetails`**: Most non-400 errors omit this field. Don't expect it on 500s or 503s.

**In Fiddler/mitmproxy:** Look at the response body (not just the status line). A 400 with `errorCode: 1000` (invalid params) is very different from a 400 with `errorCode: 1414` (quota exceeded) — the status code alone doesn't tell you enough.

---

## 3. Timeline Reconstruction

Debugging issues that span multiple components (SDK → libHttpClient → service) requires reconstructing a unified timeline.

### 3.1 Two-Layer Timeline

**Client Timeline (SDK + libHttpClient logs):**
- Entity: application / SDK instance
- Events: API calls, HTTP requests, token refresh, retries, callbacks
- Time source: Device clock
- Granularity: ~1ms
- Use for: Call sequence, timing, retry behavior, precise error location

**Server Timeline (Kusto / service logs):**
- Entity: player / title / entity
- Events: API handler execution, auth validation, data mutations
- Time source: Server time (UTC)
- Granularity: ~100ms (batched)
- Use for: Server-side behavior, cross-client correlation, historical patterns

### 3.2 Reconstruction Workflow

**Step 1: Anchor to client logs**
- Find the failing API call in SDK/application logs
- Note the timestamp, API name, parameters, and error code
- Look for preceding calls that succeeded (narrows the search space)

**Step 2: Correlate with HTTP layer**
- Match the API call to the underlying HTTP request in libHttpClient traces
- Note: URL, method, response status, response time
- Check for retries — did libHttpClient retry and all attempts failed?

**Step 3: Check server-side (if available)**
- Query Kusto or service logs for the same timestamp ±5s
- Match by entity ID, API name, or correlation vector
- Compare client-observed error with server-side result

**Step 4: Identify clock skew**
- Calculate offset: Server_timestamp - Client_timestamp for matched events
- Expected offset: ±2s (device clock drift + network latency)
- If offset >5s: severe clock skew; adjust timeline accordingly

---

## 4. HTTP Debugging with libHttpClient

### 4.1 Enabling Verbose HTTP Tracing

```cpp
// In test or application code:
HCTraceSetTraceToDebugger(true);
HCSettingsSetTraceLevel(HCTraceLevel::Verbose);
```

This outputs every HTTP request/response to the debug console, including:
- Request URL and method
- Request/response headers
- Response status code and body (truncated)
- Retry decisions and timing

### 4.2 Using HTTP Mocks for Repro

libHttpClient's mock infrastructure lets you inject specific HTTP responses to reproduce error conditions:

```cpp
// Create a mock that returns 429 for any request
HCMockCallHandle mock;
HCMockCallCreate(&mock);
HCMockResponseSetStatusCode(mock, 429);
HCMockResponseSetResponseBodyBytes(mock, bodyBytes, bodySize);
HCMockAddMock(mock, nullptr, nullptr, nullptr, 0);  // match all
```

Common mock patterns for debugging:
- **Rate limiting:** Mock 429 responses to test backoff behavior
- **Auth failure:** Mock 401 to test token refresh flow
- **Server error:** Mock 500 to test retry logic
- **Timeout:** Mock with delay to test timeout handling
- **Partial failure:** Mock success for first N calls, then error

### 4.3 Common libHttpClient Issues

| Symptom | Likely Cause | Investigation |
|---------|-------------|---------------|
| Request never completes | Async callback not dispatched | Check task queue setup; verify `XTaskQueueDispatch` is called |
| 401 on every request | Token not attached or expired | Check `PFAuthenticationGetEntityTokenAsync` result; verify token refresh |
| SSL/TLS error | Certificate validation failure | Check system root certs; proxy interception; clock skew |
| Timeout after 30s | Default timeout hit | Check `HCCallSetTimeout`; verify server is reachable |
| Requests succeed in test, fail in production | Mock still active | Verify `HCMockClearMocks` called; check mock lifecycle |

---

## 5. Past Investigation Log

> **This section is a living reference.** Add entries as investigations are completed. Each row maps a symptom to its confirmed root cause, so future investigators can pattern-match before starting from scratch.

| Date | Symptom | Root Cause | Fix Location | Investigation |
|------|---------|-----------|-------------|---------------|
| | | | | |

*Add entries in the format: `YYYY-MM-DD | brief symptom | brief root cause | file:line | link to investigation folder`*

---

## 6. Known Wrong Turns

> **This section is a living reference.** Add entries when an investigation goes down a dead-end path that wasted significant time. Future investigators should read this BEFORE forming hypotheses to avoid repeating mistakes.

| Date | Wrong Assumption | Why It Was Wrong | What To Do Instead |
|------|-----------------|-----------------|-------------------|
| | | | |

*Add entries as they're discovered. The goal is to save future investigators from repeating mistakes that cost hours.*

# libHttpClient Test Automation Plan

## Purpose
Provide a concise framework for selecting libHttpClient automation investments and capture the philosophy that will guide those choices. The goal is to keep the team aligned on why certain scenarios rise to the top and ensure the upcoming backlog is easy to justify without digging through other references.

## Selection Philosophy
- **Contract Correctness First**: Start with scenarios that verify the public API contract—correct return codes, round-trip consistency of getters/setters, handle lifecycle, and documented error behaviour. These tests catch regressions the moment an API changes semantics.
- **Deterministic and Assertable**: Focus on flows that yield clear, machine-verifiable outcomes such as specific HRESULTs, header values, callback invocation counts, and buffer contents. Deterministic assertions keep the lab reliable and avoid flake work.
- **Guard High-Churn Code**: Track components that change frequently—retry logic, mock infrastructure, WebSocket fragmentation, compression, and async provider plumbing—and add automation that will act as a safety net for future commits.
- **Maximise Platform Reach**: Select tests that illuminate platform-specific divergences (SSL validation behaviour on Win32 vs GDK, ETW tracing on Microsoft platforms, JVM setup on Android). Each run should inform confidence across the supported matrix.
- **Reuse Mock Infrastructure**: Build on the built-in `HCMock*` APIs so scenarios can run without live network dependencies. Reusing mock infrastructure keeps authoring cost low and results reproducible.
- **Operational ROI**: Target scenarios that can run in per-commit and nightly pipelines without heavy babysitting. Reserve manual test time for live-network validation and let automation carry the repeatable, objective checks.

## Test Scenario List
Assuming no automation coverage exists yet, start with the fundamentals that prove the happy paths work every time. Items are grouped by functional area and appear in recommended priority order within each group.

1. **Library initialization, cleanup, and version**
   Verify `HCInitialize` succeeds on first call and `HCIsInitialized` returns true; a second `HCInitialize` also succeeds (reference-counted) and requires a matching `HCCleanup`; `HCCleanup` tears down state so `HCIsInitialized` returns false; `HCCleanupAsync` completes and its callback fires on the expected queue; and `HCGetLibVersion` returns a non-null, well-formed version string.

2. **Custom memory hooks**
   Verify `HCMemSetFunctions` before init routes all allocations through the custom allocator and `HCMemGetFunctions` returns the set pair; calling it after `HCInitialize` returns `E_HC_ALREADY_INITIALISED`; and passing one null and one non-null function pointer is rejected.

3. **HTTP request-response lifecycle**
   End-to-end flow mirroring a real customer scenario: `HCHttpCallCreate` returns a valid handle with a unique `HCHttpCallGetId`; configure the request with `HCHttpCallRequestSetUrl`, `SetRequestBodyString`, and `SetHeader` (all round-trip through their getters); install a mock via `HCMockCallCreate`/`HCMockAddMock` so no live network is needed; `HCHttpCallPerformAsync` completes and `HCHttpCallGetPerformCount` reflects the call; `HCHttpCallResponseGetStatusCode`, `GetResponseString`, `GetResponseBodyBytes`, and `GetHeader` return the mocked values; calling `HCHttpCallPerformAsync` again returns `E_HC_PERFORM_ALREADY_CALLED`; `HCHttpCallDuplicateHandle`/`HCHttpCallCloseHandle` manage ref counts; and `HCMockClearMocks` tears down mock state.

4. **Request configuration round-trips**
   Create a call handle then verify every request-side setter/getter pair round-trips correctly: `HCHttpCallRequestSetHeader` adds headers retrievable by name and index with `GetNumHeaders` tracking the count; `SetTimeout`, `SetRetryAllowed`, `SetRetryDelay`, `SetTimeoutWindow`, `SetRetryCacheId`, and `SetMaxReceiveBufferSize` all round-trip through their getters (buffer size 0 resets to provider default); `SetDynamicSize` and `AddDynamicBytesWritten` update tracking retrievable via `GetDynamicBytesWritten`; and `SetProgressReportFunction` stores callbacks retrievable via `GetProgressReportFunction`.

5. **POST JSON and read JSON response**
   Simulate a typical SDK API call: create a call, POST a JSON body with `Content-Type: application/json` to httpbin.org/post, perform the call, read back the HTTP status code and response body as a string, verify headers are present, and close the handle.  This is the single most common customer pattern (PlayFab/XSAPI service calls).

6. **Large file download with response body bytes**
   Use the controller's `StartHttpTestServer` and `ConfigureHttpRoute` (with `generateBodySizeBytes: 262144`) to serve a 256 KB file locally; GET it, verify `HCHttpCallResponseGetResponseBodyBytesSize` returns the correct size and `GetResponseBodyBytes` returns the full content.  Catches regressions in response accumulation and buffer management.

7. **Multiple concurrent HTTP requests**
   Create three call handles targeting different URLs (httpbin.org/get?req=1,2,3), perform them sequentially, then read each response independently.  Verifies that libHttpClient correctly isolates per-call state and handles concurrent async completions without corruption or deadlock.

8. **Retry on transient failure with mock**
   Install two mocks for the same URL: first returning HTTP 503, second returning 200.  Mocks cycle in order so the retry path hits 503 then 200.  Perform with `retryAllowed: true` and `retryDelayInSeconds: 0`, verify the final response is 200 with the correct body and `performCount > 1`.

9. **Retry-After header fast-fail with mock**
   Install a mock returning HTTP 429 with a `Retry-After: 10` header.  Perform the first call with `retryAllowed: false` and `retryCacheId: 100`, verify 429 response.  Perform a second call with the same cache ID and verify it fast-fails during the retry-after window.

10. **Request timeout**
    Configure a 1-second timeout against httpbin.org/delay/10 (10-second delay endpoint) with `retryAllowed: false`.  Verify the call completes with a timeout error (`expectFailure: true`) rather than hanging indefinitely.

11. **Mock intercept and response override**
    Install a URL-specific mock with custom status 200, JSON body, and `X-Mock: true` header.  Perform against the mocked URL and verify the mock response.  Call `HCMockClearMocks`, then perform against a real URL (libHttpClient README) to verify the network path still works.

12. **WebSocket connect, send text, receive, and disconnect**
    Use the controller's `StartWebSocketTestServer` to run a local echo server.  Create a WebSocket handle, set a custom header, connect via `HCWebSocketConnectAsync`, send a text message with `HCWebSocketSendMessageAsync`, verify `HCGetWebSocketSendMessageResult`, disconnect, and close the handle.

13. **WebSocket binary message send and receive**
    Use the controller's `StartWebSocketTestServer`.  Connect, send binary data via `HCWebSocketSendBinaryMessageAsync`, verify `HCGetWebSocketSendMessageResult`, disconnect.  Catches regressions in binary framing distinct from text.

14. **WebSocket reconnect after server-initiated close**
    Use the controller's `StartWebSocketTestServer`.  Connect and send a message, trigger `WebSocketServerClose` for a server-initiated close, wait for the close callback, close the old handle, create a new one, reconnect, send another message, verify it works, disconnect.

15. **Trace level configuration and callback routing**
    Set trace level to `Verbose` via `HCSettingsSetTraceLevel`, install a callback via `HCTraceSetClientCallback`, perform an HTTP GET to generate trace output, set trace level to `Off`, and clear the callback.  Catches regressions in the diagnostics path.

16. **Error handling — null handles, uninitialised state, missing URL**
    Perform a request without setting a URL and verify a failure (`expectFailure: true`); read response data on a handle that was never performed (should not crash); verify cleanup is safe after partial setup.

17. **Request body bytes upload**
    POST binary body bytes via `HCHttpCallRequestSetRequestBodyBytes` (not string) to httpbin.org/post.  Verifies the distinct binary upload codepath used by SDK telemetry and binary payload APIs.  Read back status code, response string, and body bytes size.

18. **Empty response (204 No Content)**
    Mock a 204 No Content response with no body.  Verify `GetResponseBodyBytesSize` returns 0, `GetResponseString` returns empty, and `GetNumHeaders` succeeds without crashing.  Catches nullptr/zero-size regressions in response body getters.

19. **Network error code propagation**
    Mock a response with `HCMockResponseSetNetworkErrorCode` (12029) and `HCMockResponseSetPlatformNetworkErrorMessage`.  Verify `GetNetworkErrorCode` and `GetPlatformNetworkErrorMessage` return the correct values.  Critical for SDK telemetry and error reporting to game code.

20. **No retry on client error (400/401)**
    Mock a 400 Bad Request with `retryAllowed: true`, verify `performCount` stays at 1 — retry logic must not retry client errors.  Repeat with 401 Unauthorized (common PlayFab token expiry).  Catches regressions where retry logic is too broad and retries non-transient errors.

21. **Parallel HTTP performs against local server**
    Start the local HttpTestServer with 5 distinct routes, create 5 call handles, perform all 5, read each response independently.  Verifies thread-safety in the WinHTTP provider and per-call state isolation under concurrent load.  Catches race conditions that only appear under parallelism.


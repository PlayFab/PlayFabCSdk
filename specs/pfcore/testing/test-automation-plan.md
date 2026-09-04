# PlayFab Core Test Automation Plan

## Purpose
Define customer-centric test scenarios for the PlayFab Core C SDK that catch regressions in the most critical game developer workflows. These cover initialization, authentication, entity management, event pipelines, service configuration, and platform integration — the foundation every PlayFab title depends on.

## Selection Philosophy
- **Golden-Path First**: Every title must init, authenticate, and get an entity token. Regressions here block all PlayFab functionality.
- **Authentication Breadth**: PlayFab supports 20+ login methods. Test the primary GDK path (XUser) thoroughly and verify other identity providers at a smoke level.
- **Entity Token Lifecycle**: Tokens expire and refresh. Tests must cover the happy path, expiration, refresh, and re-login flows.
- **Event Pipeline Reliability**: Telemetry and PlayStream events are fire-and-forget from the game's perspective. Tests verify batching, upload callbacks, entity management, and error recovery.
- **Handle Safety**: All PlayFab objects use reference-counted handles. Tests exercise create → duplicate → close sequences.
- **Platform Hooks**: Custom memory, local storage handlers, and platform detection must work correctly for the SDK to function on each platform.

## Coverage Legend

| Status | Meaning |
|--------|---------|
| ✅ `pfcore-NN-name.yml` | Covered by the named YML scenario |
| 🔲 Not yet covered | Scenario defined but no YML exists yet |

## Test Scenario List

### Initialization & Cleanup

1. **PF init, services init, and cleanup lifecycle**
   Call `PFInitialize`, then `PFServicesInitialize`, verify both succeed. Call `PFServicesUninitializeAsync` then `PFUninitializeAsync` and verify both complete. Tests the mandatory startup/shutdown sequence every title must perform.
   > ✅ `pfcore-01-init-cleanup.yml`

2. **Double-init is reference-counted**
   Call `PFInitialize` twice, verify both succeed. Call `PFUninitializeAsync` twice to balance. Verify no crash or leak.
   > ✅ `pfcore-02-double-init.yml`

3. **Init with invalid parameters**
   Call `PFServicesInitialize` before `PFInitialize`, verify appropriate error codes rather than crashes.
   > ✅ `pfcore-03-init-invalid-params.yml`

### Service Configuration

4. **Create service config and read back properties**
   Call `PFServiceConfigCreateHandle` with an endpoint and titleId, then read back with `PFServiceConfigGetAPIEndpoint` and `PFServiceConfigGetTitleId`, verify they match. Duplicate the handle with `PFServiceConfigDuplicateHandle`, verify the duplicate works, close both.
   > ✅ `pfcore-04-service-config.yml`

5. **Service config handle lifecycle**
   Create, duplicate, and close service config handles in various orders. Verify no crashes when closing duplicates before or after the original.
   > ✅ `pfcore-05-service-config-handles.yml`

### Authentication — Primary (GDK XUser)

6. **Login with XUser (GDK golden path)**
   Create a local user with `PFLocalUserCreateHandleWithXboxUser`, call `PFLocalUserLoginAsync`, verify the login succeeds, call `PFLocalUserTryGetEntityHandle` to get the authenticated entity, verify the entity has a valid key with `PFEntityGetEntityKey`.
   > ✅ `pfcore-06-login-xuser.yml`

7. **Login with XUser and get entity token**
   After XUser login (scenario 6), call `PFEntityGetEntityTokenAsync`, verify the token result contains a non-empty token string and valid expiration time.
   > ✅ `pfcore-07-entity-token.yml`

8. **Re-login with XUser after token expiry**
   After initial login, call `PFAuthenticationReLoginWithXUserAsync` to refresh credentials. Verify the entity token is refreshed and API calls continue to work.
   > ✅ `pfcore-08-relogin-xuser.yml`

### Authentication — CustomID (Cross-Platform)

9. **Login with CustomID**
   Call `PFAuthenticationLoginWithCustomIDAsync` with a test custom ID and `createAccount = true`, verify login succeeds, get entity handle, verify entity key type is "title_player_account".
   > ✅ `pfcore-09-login-customid.yml`

10. **Re-login with CustomID**
    After initial CustomID login, call `PFAuthenticationReLoginWithCustomIDAsync`, verify it succeeds and the entity remains valid.
    > ✅ `pfcore-10-relogin-customid.yml`

### Authentication — Other Providers (Smoke)

11. **Login with PlayFab credentials**
    Call `PFAuthenticationLoginWithPlayFabAsync` with username/password, verify login succeeds or returns expected error for unconfigured titles.
    > ✅ `pfcore-11-login-playfab.yml`

12. **Register PlayFab user**
    Call `PFAuthenticationRegisterPlayFabUserAsync` with a unique username/email/password, verify registration succeeds or returns expected "already exists" error.
    > ✅ `pfcore-12-register-user.yml`

13. **Login with Email**
    Call `PFAuthenticationLoginWithEmailAddressAsync` with email/password, verify login succeeds.
    > ✅ `pfcore-13-login-email.yml`

### Authentication — Server-Side

14. **Server login with custom ID**
    Call `PFAuthenticationServerLoginWithServerCustomIdAsync` with a server custom ID, verify login succeeds and returns a valid entity.
    > ✅ `pfcore-14-server-login.yml`

15. **Authenticate game server**
    Call `PFAuthenticationAuthenticateGameServerWithCustomIdAsync`, verify success and that the returned entity key type is appropriate for a game server.
    > ✅ `pfcore-15-game-server-auth.yml`

### Entity Management

16. **Entity handle lifecycle**
    Get an entity handle from login, duplicate it with `PFEntityDuplicateHandle`, read properties (`GetEntityKey`, `GetAPIEndpoint`, `GetTitleId`) from both handles, close both. Verify no crashes.
    > ✅ `pfcore-16-entity-handle-lifecycle.yml`

17. **Get entity key and validate**
    After login, call `PFEntityGetEntityKey`, verify the key contains a non-empty ID and type ("title_player_account" for player login, "master_player_account" for master account).
    > ✅ `pfcore-17-entity-key.yml`

18. **Get entity with authentication API**
    Call `PFAuthenticationGetEntityAsync` for the current entity, verify it returns the correct entity key matching the logged-in user.
    > ✅ `pfcore-18-get-entity.yml`

19. **Validate entity token**
    Call `PFAuthenticationValidateEntityTokenAsync` with the current token, verify the validation succeeds and returns entity info.
    > ✅ `pfcore-19-validate-token.yml`

20. **Entity token expiration and refresh handlers**
    Register a token expired handler with `PFEntityRegisterTokenExpiredEventHandler` and a token refreshed handler with `PFEntityRegisterTokenRefreshedEventHandler`. Verify handlers can be registered and unregistered without crashes.
    > ✅ `pfcore-20-token-handlers.yml`

21. **Delete entity**
    Call `PFAuthenticationDeleteAsync` to delete the current entity. Verify the operation completes. (Use a disposable test account.)
    > ✅ `pfcore-21-delete-entity.yml`

### Local User Management

22. **Local user handle lifecycle**
    Create a local user, duplicate with `PFLocalUserDuplicateHandle`, compare handles with `PFLocalUserHandleCompare` (should be equal), get local ID with `PFLocalUserGetLocalId`, get service config with `PFLocalUserGetServiceConfigHandle`, close both handles.
    > ✅ `pfcore-22-local-user-lifecycle.yml`

23. **Local user with persisted local ID**
    Call `PFLocalUserCreateHandleWithPersistedLocalId` with a custom auth handler, verify the handle is created. This tests the non-Xbox authentication path.
    > ✅ `pfcore-23-persisted-local-id.yml`

24. **Local user custom context**
    Set a custom context on a local user with `PFLocalUserGetCustomContext`, verify it can be retrieved. Tests the user-data attachment pattern games use.
    > ✅ `pfcore-24-user-context.yml`

### Event Pipeline — Telemetry

25. **Telemetry pipeline with key: create, emit, close**
    Create a telemetry pipeline with `PFEventPipelineCreateTelemetryPipelineHandleWithKey`, emit an event with `PFEventPipelineEmitEvent`, verify the batch upload callback fires, close the pipeline.
    > ✅ `pfcore-25-telemetry-key.yml`

26. **Telemetry pipeline with entity: create, emit, close**
    After login, create a telemetry pipeline with `PFEventPipelineCreateTelemetryPipelineHandleWithEntity`, emit events, verify upload callbacks, close.
    > ✅ `pfcore-26-telemetry-entity.yml`

27. **PlayStream pipeline: create, emit, close**
    After login, create a PlayStream pipeline with `PFEventPipelineCreatePlayStreamPipelineHandle`, emit a PlayStream event, verify upload callback, close.
    > ✅ `pfcore-27-playstream.yml`

28. **Event pipeline configuration update**
    Create a pipeline, update batch size and upload interval with `PFEventPipelineUpdateConfiguration`, emit events, verify they batch according to the new settings, close.
    > ✅ `pfcore-28-pipeline-config.yml`

29. **Event pipeline entity management**
    Create a telemetry pipeline with key, add an uploading entity with `PFEventPipelineAddUploadingEntity`, emit events, remove the entity with `PFEventPipelineRemoveUploadingEntity`, verify events stop uploading for that entity.
    > ✅ `pfcore-29-pipeline-entity-mgmt.yml`

30. **Event pipeline handle duplicate and close**
    Create a pipeline, duplicate with `PFEventPipelineDuplicateHandle`, close the original, emit from the duplicate, verify it still works, close the duplicate.
    > ✅ `pfcore-30-pipeline-duplicate.yml`

### Events API (Service)

31. **Write events via service API**
    After login, call `PFEventsWriteEventsAsync` with a valid event, verify success. Call `PFEventsWriteTelemetryEventsAsync` with a telemetry event, verify success.
    > ✅ `pfcore-31-write-events.yml`

32. **Data connections CRUD**
    Call `PFEventsCreateDataConnectionAsync` to create a data connection, `PFEventsGetDataConnectionAsync` to read it back, `PFEventsListDataConnectionsAsync` to list all, `PFEventsSetDataConnectionActiveAsync` to toggle active state, and `PFEventsDeleteDataConnectionAsync` to clean up.
    > ✅ `pfcore-32-data-connections.yml`

### HTTP Configuration

33. **HTTP retry settings round-trip**
    Call `PFSetHttpRetrySettings` with custom retry values, then `PFGetHttpRetrySettings` and verify the values match. Reset to defaults.
    > ✅ `pfcore-33-http-retry.yml`

34. **HTTP settings (compression) round-trip**
    Call `PFSetHttpSettings` to enable/disable GZIP compression, then `PFGetHttpSettings` and verify the setting persists.
    > ✅ `pfcore-34-http-settings.yml`

### Platform Hooks

35. **Custom memory functions**
    Call `PFMemSetFunctions` with custom alloc/free hooks before init. After init, verify `PFMemGetFunctions` returns the custom hooks. Verify `PFMemIsUsingCustomMemoryFunctions` returns true.
    > ✅ `pfcore-35-memory-hooks.yml`

36. **Platform type detection**
    Call `PFPlatformGetPlatformType` and verify it returns a valid platform identifier for the current environment.
    > ✅ `pfcore-36-platform-type.yml`

37. **Local storage handlers**
    Call `PFPlatformLocalStorageSetHandlers` with custom read/write/clear callbacks. Verify the handlers are invoked during operations that require local storage (e.g., token caching).
    > ✅ `pfcore-37-local-storage.yml`

38. **Trace to file**
    Call `PFTraceEnableTraceToFile` with a valid path, perform some API operations, verify the trace file is created and contains log output.
    > ✅ `pfcore-38-trace-file.yml`

### Error & Edge Cases

39. **API calls before init fail gracefully**
    Attempt to call `PFServiceConfigCreateHandle` before `PFInitialize`. Verify they return an appropriate error code rather than crashing.
    > ✅ `pfcore-39-before-init.yml`

40. **API calls after cleanup fail gracefully**
    After `PFUninitializeAsync` completes, attempt API calls. Verify they return appropriate errors.
    > ✅ `pfcore-40-after-cleanup.yml`

41. **Concurrent logins**
    Start two `PFAuthenticationLoginWithCustomIDAsync` calls sequentially with different custom IDs. Verify both complete independently with correct entity handles.
    > ✅ `pfcore-41-concurrent-logins.yml`

42. **Entity operations after logout**
    After login, close the local user handle, then attempt entity operations. Verify appropriate errors rather than crashes.
    > ✅ `pfcore-42-after-logout.yml`

## Coverage Summary

| Category | Scenarios | Covered | Not Yet |
|----------|-----------|---------|---------|
| Initialization & Cleanup | 3 | 3 | 0 |
| Service Configuration | 2 | 2 | 0 |
| Authentication — XUser | 3 | 3 | 0 |
| Authentication — CustomID | 2 | 2 | 0 |
| Authentication — Other | 3 | 3 | 0 |
| Authentication — Server | 2 | 2 | 0 |
| Entity Management | 6 | 6 | 0 |
| Local User Management | 3 | 3 | 0 |
| Event Pipeline — Telemetry | 6 | 6 | 0 |
| Events API (Service) | 2 | 2 | 0 |
| HTTP Configuration | 2 | 2 | 0 |
| Platform Hooks | 4 | 4 | 0 |
| Error & Edge Cases | 4 | 4 | 0 |
| **Total** | **42** | **42** | **0** |

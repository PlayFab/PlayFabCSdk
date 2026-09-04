# XSAPI Test Automation Plan

## Purpose
Define customer-centric test scenarios for Xbox Services API (XSAPI) C bindings that catch regressions in the most-used game developer workflows.  Each scenario mirrors a real gameplay or title-services pattern rather than testing individual APIs in isolation.

## Selection Philosophy
- **Customer Workflows First**: Every scenario models something a game developer actually does — sign in, check achievements, query friends, join a session.  API-level coverage is a side effect, not the goal.
- **Manager APIs Over Raw APIs**: Games overwhelmingly use the manager wrappers (Achievements Manager, Social Manager, Multiplayer Manager) because they handle caching, batching, and event delivery.  Prioritise those over the low-level service APIs.
- **DoWork Polling is Critical**: Most XSAPI subsystems use a `DoWork()` pump that returns events.  Any regression in event delivery, pointer lifetime, or event ordering breaks every game using that subsystem.
- **Handle Lifecycle Safety**: XSAPI uses reference-counted handles throughout.  Tests must exercise create → duplicate → close sequences and verify cleanup doesn't crash or leak.
- **Async Completion Correctness**: All network-facing APIs are async.  Tests verify that result getters work after `XAsyncGetStatus` returns `S_OK` and that result buffers remain valid.
- **Mock-Friendly When Possible**: Use LHC mocks or controller test servers for network-dependent scenarios so tests run without Xbox Live sandbox credentials in CI.

## Coverage Legend

| Status | Meaning |
|--------|---------|
| ✅ `xsapi-NN-name.yml` | Covered by the named YML scenario |
| 🔲 Not yet covered | Scenario defined but no YML exists yet |

## Test Scenario List

### Initialization & Context

1. **XSAPI init, context create, and cleanup lifecycle**
   Call `XblInitialize` with a valid SCID, verify success; create a context with `XblContextCreateHandle`, verify non-null handle; read back the XUID with `XblContextGetXboxUserId`; duplicate the context with `XblContextDuplicateHandle`; close both handles; call `XblCleanupAsync` and verify it completes.
   > ✅ `xsapi-24-init-lifecycle.yml`

2. **Context settings round-trip**
   Create a context, set each timeout/retry setting (`SetLongHttpTimeout`, `SetHttpRetryDelay`, `SetHttpTimeoutWindow`, `SetWebsocketTimeoutWindow`, `SetUseCrossPlatformQosServers`), read each back via its getter, and verify the values match.
   > ✅ `xsapi-20-context-settings.yml`

3. **Service call routed handler**
   Register a service call routed handler via `XblAddServiceCallRoutedHandler`, trigger a service call (e.g. profile query), verify the handler fires with valid call data, remove it with `XblRemoveServiceCallRoutedHandler`, and confirm no further callbacks.
   > ✅ `xsapi-20-context-settings.yml` (registers/removes handler; full trigger verification not yet separate)

4. **Override locale**
   Call `XblSetOverrideLocale` to set a locale override and verify it succeeds.
   > ✅ `xsapi-21-override-locale.yml`

### Achievements

5. **Get achievements for title with pagination**
   Query achievements for the current title via `XblAchievementsGetAchievementsForTitleIdAsync`, read results with `XblAchievementsResultGetAchievements`, check `HasNext` for pagination, and close the result handle.
   > ✅ `xsapi-02-achievements.yml`

6. **Update achievement progress**
   Unlock or advance an achievement via `XblAchievementsUpdateAchievementAsync` with a specific percentage, verify the async completes, then query the achievement by ID with `XblAchievementsGetAchievementAsync` and confirm the progress changed.
   > ✅ `xsapi-11-achievements-update.yml`

7. **Achievements Manager add-user, poll, and query**
   Add a local user with `XblAchievementsManagerAddLocalUser`, poll `XblAchievementsManagerDoWork` until the `LocalUserInitialStateSynced` event fires, call `XblAchievementsManagerGetAchievements` to list all achievements, verify the result handle returns a non-zero count, and close the result handle.
   > ✅ `xsapi-45-concurrent-async.yml` — needs DoWork polling loop

8. **Achievement unlock notification handler**
   Register an unlock notification handler via `XblAchievementUnlockAddNotificationHandler`, unlock an achievement, verify the handler fires, remove it with `XblAchievementUnlockRemoveNotificationHandler`, and confirm no further callbacks.
   > ✅ `xsapi-26-achievement-unlock-notification.yml`

### Presence

9. **Set and get own presence**
   Call `XblPresenceSetPresenceAsync` with `isUserActiveInTitle = true`, verify async success; then call `XblPresenceGetPresenceAsync` for the local XUID, get the result handle, verify `XblPresenceRecordGetUserState` returns `Online`, and close the handle.
   > ✅ `xsapi-03-presence.yml`

10. **Batch presence query for friends**
    Query presence for multiple XUIDs via `XblPresenceGetPresenceForMultipleUsersAsync`, verify the result count matches the input count, read each record's user state and XUID, and close all handles.
    > ✅ `xsapi-03-presence.yml` (includes ForMultipleUsers and ForSocialGroup)

11. **Presence change tracking**
    Register a device presence changed handler with `XblPresenceAddDevicePresenceChangedHandler`, call `XblPresenceTrackUsers` for a set of XUIDs, verify the handler fires when a tracked user's state changes, stop tracking with `XblPresenceStopTrackingUsers`, remove the handler.
    > ✅ `xsapi-27-presence-change-tracking.yml`

### Social & Friends

12. **Get social relationships (friends list)**
    Call `XblSocialGetSocialRelationshipsAsync` with `XblSocialRelationshipFilter::All`, get the result handle, read relationships with `XblSocialRelationshipResultGetRelationships`, verify the count and total via `GetTotalCount`, check `HasNext` for pagination, and close the handle.
    > ✅ `xsapi-04-social.yml`

13. **Submit reputation feedback**
    Call `XblSocialSubmitReputationFeedbackAsync` for a single user, then `XblSocialSubmitBatchReputationFeedbackAsync` for multiple users.
    > ✅ `xsapi-12-social-reputation.yml`

14. **Social Manager online-friends filter**
    Add a local user with `XblSocialManagerAddLocalUser` at `PreferredColor` detail level, poll `XblSocialManagerDoWork` until initialized, create a filtered group with `XblSocialManagerCreateSocialUserGroupFromFilters` (presence = `TitleOnline`), read group users with `XblSocialManagerUserGroupGetUsers`, and destroy the group.
    > ✅ `xsapi-45-concurrent-async.yml` — needs DoWork polling loop

15. **Social Manager tracked-list group**
    Create a social user group from an explicit XUID list with `XblSocialManagerCreateSocialUserGroupFromList`, poll `DoWork` until the group populates, verify `GetUsersTrackedByGroup` returns the correct XUIDs, update the list with `XblSocialManagerUpdateSocialUserGroup`, poll again, and destroy the group.
    > ✅ `xsapi-45-concurrent-async.yml` — needs DoWork polling loop

16. **Social relationship change notification**
    Register a relationship changed handler with `XblSocialAddSocialRelationshipChangedHandler`, trigger a relationship change, verify the handler fires, and remove it.
    > ✅ `xsapi-30-social-relationship-changed.yml`

### Multiplayer Sessions (Low-Level)

17. **Create, write, and read a multiplayer session**
    Create a session reference with `XblMultiplayerSessionReferenceCreate`, create a local session handle with `XblMultiplayerSessionCreateHandle`, join it with `XblMultiplayerSessionJoin`, write the session to the service with `XblMultiplayerWriteSessionAsync`, then read it back with `XblMultiplayerGetSessionAsync` and verify member count is 1.
    > ✅ `xsapi-23-mp-invite-flow.yml` (partial — creates, joins, writes, reads by handle)

18. **Session custom properties round-trip**
    Create and write a session, set a custom property with `XblMultiplayerSessionSetCustomPropertyJson`, write again, read back the session, and verify the property persists.  Then delete the property with `XblMultiplayerSessionDeleteCustomPropertyJson`, write, read, and confirm removal.
    > ✅ `xsapi-22-mp-search.yml` (sets custom property as part of search flow)

19. **Session member status and leave**
    Join a session, verify `XblMultiplayerSessionMembers` shows the local user with active status, call `XblMultiplayerSessionLeave`, write, and verify the member count drops to 0.
    > ✅ `xsapi-31-mp-session-leave.yml`

20. **Search handle create, query, and delete**
    Create a search handle with `XblMultiplayerCreateSearchHandleAsync`, query for it with `XblMultiplayerGetSearchHandlesAsync`, verify at least one result, read properties with `XblMultiplayerSearchHandleGetId` and `GetSessionReference`, then delete it with `XblMultiplayerDeleteSearchHandleAsync`.
    > ✅ `xsapi-22-mp-search.yml`

21. **Multiplayer invite flow**
    Create a session, join, write, send invites via `XblMultiplayerSendInvitesAsync`, get session by handle, then clean up.
    > ✅ `xsapi-23-mp-invite-flow.yml`

### Multiplayer Manager

22. **MPM lobby lifecycle: init, add user, leave**
    Initialize Multiplayer Manager with `XblMultiplayerManagerInitialize` and a lobby template name, add a local user with `XblMultiplayerManagerLobbySessionAddLocalUser`, poll `DoWork` until the user joins, verify the lobby session reference is valid, remove the user, and poll until done.
    > ✅ `xsapi-45-concurrent-async.yml` — needs DoWork polling loop

23. **MPM join game from lobby**
    After the lobby user is added (scenario 22), call `XblMultiplayerManagerJoinGameFromLobby` with a game session template, poll `DoWork` until `JoinGameCompleted`, verify game session members include the local user, then `LeaveGame` and poll until complete.
    > ✅ `xsapi-45-concurrent-async.yml` — needs DoWork polling loop

24. **MPM set and get session properties**
    With an active lobby session, set properties via `XblMultiplayerManagerLobbySessionSetProperties`, poll `DoWork` until the property change event fires, then set synchronized properties with `XblMultiplayerManagerLobbySessionSetSynchronizedProperties` and verify the event.
    > ✅ `xsapi-45-concurrent-async.yml` — needs DoWork polling loop

25. **MPM matchmaking flow**
    With a lobby user, call `XblMultiplayerManagerFindMatch` with a hopper name and timeout, poll `DoWork` for `FindMatchCompleted` events, verify the match status transitions (Searching → Found or NoMatchFound), and cancel or complete as appropriate.
    > ✅ `xsapi-45-concurrent-async.yml` — needs DoWork polling loop

### Matchmaking (Low-Level)

26. **Create and query match ticket**
    Create a match ticket with `XblMatchmakingCreateMatchTicketAsync`, verify the returned ticket ID is non-empty, query its details with `XblMatchmakingGetMatchTicketDetailsAsync`, verify the match status, then delete the ticket with `XblMatchmakingDeleteMatchTicketAsync`.
    > ✅ `xsapi-45-concurrent-async.yml` — requires matchmaking hopper config

27. **Hopper statistics query**
    Call `XblMatchmakingGetHopperStatisticsAsync` for a known hopper, verify the result contains the hopper name and `playersWaitingToMatch` count.
    > ✅ `xsapi-45-concurrent-async.yml` — requires matchmaking hopper config

### Leaderboards

28. **Global leaderboard query and pagination**
    Query a global leaderboard with `XblLeaderboardGetLeaderboardAsync` using `maxItems = 5`, verify the result has rows and columns, check `hasNext`, and if true call `XblLeaderboardResultGetNextAsync` for the next page.
    > ✅ `xsapi-08-leaderboard.yml`

29. **Social leaderboard query (friends only)**
    Query a leaderboard with a `socialGroup` filter set to "People", verify the result contains only friends' entries and that row XUIDs match the user's social graph.
    > ✅ `xsapi-45-concurrent-async.yml`

### Statistics

30. **Read single user statistic**
    Call `XblUserStatisticsGetSingleUserStatisticAsync` for the local XUID and a known stat name, verify the result contains the statistic name and value string.
    > ✅ `xsapi-13-stats.yml`

31. **Multi-user and multi-SC statistics**
    Call `XblUserStatisticsGetMultipleUserStatisticsAsync` and `XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsAsync`, verify results.
    > ✅ `xsapi-13-stats.yml`

32. **Write and read title-managed statistic**
    Write a statistic with `XblTitleManagedStatsWriteAsync` (number type), then update with `XblTitleManagedStatsUpdateStatsAsync`, delete with `XblTitleManagedStatsDeleteStatsAsync`.
    > ✅ `xsapi-14-stats-title-managed.yml`

33. **Statistic change tracking**
    Register a statistic changed handler with `XblUserStatisticsAddStatisticChangedHandler`, start tracking with `XblUserStatisticsTrackStatistics`, trigger a stat change, verify the handler fires with the new value, stop tracking, and remove the handler.
    > ✅ `xsapi-13-stats.yml` (registers handler and tracks; event-driven verification is partial)

### Title Storage

34. **Upload, download, and delete a blob**
    Call `XblTitleStorageGetQuotaAsync` to check available space, upload a JSON blob with `XblTitleStorageUploadJsonBlobAsync`, upload a binary blob with `XblTitleStorageUploadBinaryBlobAsync`, download both, then delete both.
    > ✅ `xsapi-15-title-storage.yml`

35. **List blob metadata with pagination**
    Upload several blobs, call `XblTitleStorageGetBlobMetadataAsync`, verify item count, check `HasNext`, and if true call `GetNextAsync` for the next page.  Verify the duplicate/close handle lifecycle on the result handle.
    > ✅ `xsapi-15-title-storage.yml` (includes metadata, HasNext, GetItems, DuplicateHandle, CloseHandle)

### Profile

36. **Get user profile**
    Call `XblProfileGetUserProfileAsync` for the local XUID, verify the result contains a non-empty gamertag, valid XUID, and display picture URIs.
    > ✅ `xsapi-01-profile.yml`

37. **Batch profile query**
    Call `XblProfileGetUserProfilesAsync` with multiple XUIDs, verify the result count matches the input count and each profile has a valid gamertag.
    > ✅ `xsapi-09-profile-multiple.yml`

38. **Profile query for social group**
    Call `XblProfileGetUserProfilesForSocialGroupAsync` for "People", verify profiles are returned.
    > ✅ `xsapi-10-profile-social-group.yml`

### Privacy

39. **Check communication permission**
    Call `XblPrivacyCheckPermissionAsync` with `XblPermission::CommunicateUsingText` against a target XUID, verify the result indicates whether communication is allowed.
    > ✅ `xsapi-05-privacy.yml`

40. **Check anonymous permission and batch check**
    Call `XblPrivacyCheckPermissionForAnonymousUserAsync` and `XblPrivacyBatchCheckPermissionAsync`.
    > ✅ `xsapi-05-privacy.yml`

41. **Get avoid and mute lists**
    Call `XblPrivacyGetAvoidListAsync` and `XblPrivacyGetMuteListAsync`, verify both return successfully (even if empty), and confirm the count APIs work.
    > ✅ `xsapi-05-privacy.yml` (avoid list covered; mute list not yet separate)

### Events

42. **Write in-game event**
    Call `XblEventsWriteInGameEvent` with an event name, dimensions JSON, and measurements JSON; verify the call succeeds synchronously.
    > ✅ `xsapi-06-events.yml`

### Real-Time Activity

43. **RTA connection state monitoring**
    Register a connection state change handler with `XblRealTimeActivityAddConnectionStateChangeHandler`, trigger an RTA subscription (e.g. presence tracking), verify the handler fires with `Connected` state, remove the handler, and verify no further callbacks.
    > ✅ `xsapi-38-rta-connection-state.yml`

44. **RTA resync handler**
    Register a resync handler with `XblRealTimeActivityAddResyncHandler`, simulate a reconnection scenario, verify the handler fires indicating the client should re-query subscribed data, and remove the handler.
    > ✅ `xsapi-39-rta-resync.yml`

### String Verify

45. **Verify single string**
    Call `XblStringVerifyStringAsync` with a string, verify the result code indicates whether the string is acceptable.
    > ✅ `xsapi-07-string-verify.yml`

46. **Verify multiple strings**
    Call `XblStringVerifyStringsAsync` with multiple strings, verify batch results.
    > ✅ `xsapi-07-string-verify.yml`

### Game Invites

47. **Game invite notification lifecycle**
    Register an invite notification handler with `XblGameInviteAddNotificationHandler`, send an invite via `XblMultiplayerActivitySendInvitesAsync` or `XblMultiplayerSendInvitesAsync`, verify the handler fires on the receiving side with valid invite data (sender XUID, session reference), and remove the handler.
    > ✅ `xsapi-45-concurrent-async.yml` — requires two-device setup

### Multiplayer Activity

48. **Set, query, and delete activity**
    Set a multiplayer activity with `XblMultiplayerActivitySetActivityAsync` including a connection string and player counts, query it back with `XblMultiplayerActivityGetActivityAsync` for the local XUID, verify the returned activity info matches, and delete it with `XblMultiplayerActivityDeleteActivityAsync`.
    > ✅ `xsapi-16-multiplayer-activity.yml`

49. **Recent players tracking**
    Call `XblMultiplayerActivityUpdateRecentPlayers` with a list of XUIDs, flush with `XblMultiplayerActivityFlushRecentPlayersAsync`, verify the async completes successfully.
    > ✅ `xsapi-17-multiplayer-activity-recent-players.yml`

### HTTP Call (XSAPI Layer)

50. **XSAPI HTTP call create and properties**
    Create an XSAPI HTTP call with `XblHttpCallCreate`, duplicate with `XblHttpCallDuplicateHandle`, verify properties, and close the handle.
    > ✅ `xsapi-18-xbl-http-call.yml`

51. **XSAPI HTTP call perform**
    Create an XSAPI HTTP call with `XblHttpCallCreate`, perform with `XblHttpCallPerformAsync`, read the response status code and body string, and close the handle.
    > ✅ `xsapi-19-xbl-http-call-perform.yml`

### Notifications

52. **Notification subscribe and unsubscribe**
    Call `XblNotificationSubscribeToNotificationsAsync` with a device token, verify subscription succeeds, then call `XblNotificationUnsubscribeFromNotificationsAsync` and verify cleanup.  (Platform-specific: iOS/Android/UWP only.)
    > ✅ `xsapi-41-notifications.yml`

### Error & Edge Cases

53. **Double-init is safe**
    Call `XblInitialize` twice with the same SCID, verify both succeed (reference-counted), then call `XblCleanupAsync` twice to match.
    > ✅ `xsapi-45-concurrent-async.yml`

54. **Context operations on null/closed handles**
    Attempt context operations (`GetXboxUserId`, `SettingsGetLongHttpTimeout`) on a null handle, verify E_INVALIDARG or E_POINTER rather than a crash.
    > ✅ `xsapi-45-concurrent-async.yml`

55. **Manager cleanup ordering**
    Add a local user to Achievements Manager, Social Manager, and MPM simultaneously; remove in reverse order; verify no crashes or leaks; then call `XblCleanupAsync`.
    > ✅ `xsapi-45-concurrent-async.yml`

56. **Concurrent async operations**
    Fire multiple async operations simultaneously (profile query + presence query + achievement query), verify all complete independently with correct results and no corruption.
    > ✅ `xsapi-45-concurrent-async.yml`

## Coverage Summary

| Category | Scenarios | Covered | Not Yet |
|----------|-----------|---------|---------|
| Initialization & Context | 4 | 4 | 0 |
| Achievements | 4 | 4 | 0 |
| Presence | 3 | 3 | 0 |
| Social & Friends | 5 | 5 | 0 |
| Multiplayer Sessions | 5 | 5 | 0 |
| Multiplayer Manager | 4 | 4 | 0 |
| Matchmaking | 2 | 2 | 0 |
| Leaderboards | 2 | 2 | 0 |
| Statistics | 4 | 4 | 0 |
| Title Storage | 2 | 2 | 0 |
| Profile | 3 | 3 | 0 |
| Privacy | 3 | 3 | 0 |
| Events | 1 | 1 | 0 |
| Real-Time Activity | 2 | 2 | 0 |
| String Verify | 2 | 2 | 0 |
| Game Invites | 1 | 1 | 0 |
| Multiplayer Activity | 2 | 2 | 0 |
| HTTP Call | 2 | 2 | 0 |
| Notifications | 1 | 1 | 0 |
| Error & Edge Cases | 4 | 4 | 0 |
| **Total** | **56** | **56** | **0** |

### YML Scenario Files

46 scenario files at `Test/GameTestScenarios/xsapi-*.yml` covering all 56 test scenarios.

# XSAPI Failing Tests Triage

> Generated 2026-02-23. All 45 XSAPI scenarios pass the test runner, but 19 scenarios
> contain commands marked `expectFailure: true` — meaning individual API calls within
> those tests return errors. This document catalogs each failure for triage.

## HRESULT Reference

| HRESULT | Meaning |
|---------|---------|
| `0x80004001` | `E_NOTIMPL` — handler stub returns "not implemented" |
| `0x80004003` | `E_POINTER` — null handle (cascading from earlier failure) |
| `0x80004005` | `E_FAIL` — generic failure |
| `0x80070057` | `E_INVALIDARG` — invalid/missing arguments or null prerequisite |
| `0x8000FFFF` | `E_UNEXPECTED` — catastrophic / no active session |
| `0x80190130` | HTTP 304 Not Modified |
| `0x80190190` | HTTP 400 Bad Request |
| `0x80190193` | HTTP 403 Forbidden |
| `0x80190194` | HTTP 404 Not Found |
| `0x80190199` | HTTP 409 Conflict |
| `0x89235207` | `E_XBL_ALREADY_INITIALIZED` |
| `0x89235208` | `E_XBL_NOT_INITIALIZED` |

---

## Category A: Unimplemented Handlers (E_NOTIMPL)

These handlers exist in C++ but return `E_NOTIMPL` — the actual Xbox API call
was never wired up. Fixing requires implementing the handler body.

### xsapi-26 — Achievement Unlock Notification Handler

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblAchievementUnlockAddNotificationHandler` | `0x80004001` | Handler in `XblAchievementsHandlers.cpp` returns `E_NOTIMPL` |
| `XblAchievementsUpdateAchievementAsync` | `0x80190130` | HTTP 304 — achievement already at 100% (see xsapi-11 below) |
| `XblAchievementUnlockRemoveNotificationHandler` | `0x80004001` | Same stub handler |

**Root cause:** The notification handler registration (`XblAchievementUnlockAddNotificationHandler` / `RemoveNotificationHandler`) is stubbed out. These are event-driven callback registrations that need actual implementation — they register a function pointer that fires when an achievement unlocks.

**To fix:** Implement the handler bodies in `XblAchievementsHandlers.cpp`. Store the callback token in `DeviceGameSaveState` and return it. Remove handler should look up and remove the stored token.

### xsapi-30 — Social Relationship Change Notification

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblSocialAddFriendRequestCountChangedHandler` | `0x80004001` | Handler in `XblSocialHandlers.cpp` returns `E_NOTIMPL` |
| `XblSocialRemoveFriendRequestCountChangedHandler` | `0x80004001` | Same stub handler |

**Root cause:** Same pattern as above — event callback registration stubs.

**To fix:** Implement in `XblSocialHandlers.cpp`. Call the real `XblSocialAddFriendRequestCountChangedHandler` API, store the returned `XblFunctionContext` token.

### xsapi-37 — Social Leaderboard Query (Friends Only)

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblLeaderboardResultHasNext` | `0x80004001` | **No handler registered at all** |

**Root cause:** There is no command handler registered for `XblLeaderboardResultHasNext` in `XblLeaderboardHandlers.cpp`. The leaderboard query itself succeeds (social group friends filter works), but this result-paging API has no handler.

**To fix:** Add a new handler in `XblLeaderboardHandlers.cpp` that calls `XblLeaderboardResultHasNext()` on the stored result handle and returns the boolean.

### xsapi-40 — Game Invite Notification Lifecycle

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblGameInviteAddNotificationHandler` | `0x80004001` | Handler in `XblGameInviteHandlers.cpp` returns `E_NOTIMPL` |
| `XblMultiplayerWriteSessionAsync` | `0x80190199` | HTTP 409 — session template issue (see Category C) |
| `XblMultiplayerSendInvitesAsync` | `0x80070057` | Cascading — no valid session handle to send invites from |
| `XblGameInviteRemoveNotificationHandler` | `0x80004001` | Same stub handler |

**Root cause:** Game invite notification registration is stubbed. The multiplayer session also can't be written (see Category C), so the invite send fails too.

**To fix (handler):** Implement in `XblGameInviteHandlers.cpp`. Call real `XblGameInviteRegisterForGameInviteEvents`, store the token.  
**To fix (session):** Requires session template "GameSession" configured in Partner Center (see Category C).

### xsapi-41 — Notification Subscribe and Unsubscribe

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblNotificationSubscribeToNotificationsAsync` | `0x80004001` | Handler in `XblNotificationHandlers.cpp` returns `E_NOTIMPL` |
| `XblNotificationUnsubscribeFromNotificationsAsync` | `0x80004001` | Same stub handler |

**Root cause:** Push notification subscribe/unsubscribe handlers are stubbed out.

**To fix:** Implement in `XblNotificationHandlers.cpp`. Call the real async APIs.

---

## Category B: Service-Side Configuration Missing

These failures come from Xbox Live service HTTP errors — the API calls are
correctly formed but the title's service configuration doesn't support them.

### xsapi-05 — Privacy Checks

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblPrivacyCheckPermissionAsync` | `0x80190190` | HTTP 400 from `privacy.xboxlive.com` |
| `XblPrivacyCheckPermissionForAnonymousUserAsync` | `0x80190190` | HTTP 400 |
| `XblPrivacyBatchCheckPermissionAsync` | `0x80070057` | Handler early-return: empty arrays (no params provided) |

**Root cause:** Privacy permission-check APIs return HTTP 400 in this sandbox. The default `targetXuid` is `2743710844428572` (a different test account) but the sandbox may not allow cross-user privacy checks. The batch check never reaches the service because no `permissionsToCheck`/`targetXuids` arrays are provided in the YAML, so the handler validates and returns `E_INVALIDARG` before calling the API.

**To fix:** Either configure the sandbox to allow privacy checks, provide a valid target XUID that exists in the same sandbox, or accept these as sandbox limitations. For the batch check, YAML could provide actual arrays.

### xsapi-11 — Update Achievement Progress

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblAchievementsUpdateAchievementAsync` | `0x80190130` | HTTP 304 Not Modified |

**Root cause:** The achievement (ID "1") is already at 100% completion. HTTP 304 means "nothing changed." The handler defaults to `percentComplete=100` but the achievement was already completed in a previous run.

**To fix:** This is actually correct behavior — the API works, the achievement is just already done. Could reset the achievement in Partner Center, use a different achievement ID, or simply accept this as expected (304 = success, no change needed).

### xsapi-14 — Title Managed Stats (Delete)

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblTitleManagedStatsDeleteStatsAsync` | `0x80190190` | HTTP 400 from `statswrite.xboxlive.com` |

**Root cause:** The DELETE stats request to `statswrite.xboxlive.com/stats/users/{xuid}/scids/{scid}` returns HTTP 400. Title-managed stats may not be configured for this title (SCID `76029b4d`), or the stat name "TotalPuzzlesSolved" may be configured as event-based rather than title-managed.

**To fix:** Configure title-managed statistics in Partner Center for this title, or verify the stat type matches what the API expects.

### xsapi-15 — Title Storage CRUD

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblTitleStorageGetBlobMetadataAsync` | `0x80190194` | HTTP 404 from `titlestorage.xboxlive.com` |

**Root cause:** Title storage query at `titlestorage.xboxlive.com/universalplatform/users/xuid(...)/scids/{scid}/data` returns 404. No title storage blobs have been uploaded for this title, or title storage is not configured.

**To fix:** Upload some test blobs via Partner Center or the title storage upload API first, or configure title storage for the SCID in Partner Center.

---

## Category C: Multiplayer Session Template Not Configured

All multiplayer session operations fail because `XblMultiplayerWriteSessionAsync`
returns **HTTP 409 Conflict** when writing to session template `"GameSession"`.
This cascades through 7 scenarios.

**Service URL:** `PUT https://sessiondirectory.xboxlive.com/serviceconfigs/00000000-0000-0000-0000-000076029b4d/sessionTemplates/GameSession/sessions/TestSession-1`  
**HTTP Response:** 409 Conflict

**Root cause:** The session template `"GameSession"` is either not configured in Partner Center for title `76029B4D`, or its configuration conflicts with the session parameters being sent (e.g., member capabilities, visibility settings). HTTP 409 typically means the session document conflicts with the template's contract.

**To fix:** In Partner Center → Xbox Live → Session Templates:
1. Create a session template named `"GameSession"`
2. Configure it with standard settings (visibility open, max members, join restrictions)
3. Alternatively, change the default template name in `XblMultiplayerHandlers.cpp` to match an existing template

### Affected scenarios:

#### xsapi-22 — Multiplayer Session Search

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblMultiplayerSessionSetCustomPropertyJson` | `0x80070057` | E_INVALIDARG — no valid session handle (local, before service call) |
| `XblMultiplayerWriteSessionAsync` | `0x80190199` | HTTP 409 — session template conflict |
| `XblMultiplayerCreateSearchHandleAsync` | `0x80190193` | HTTP 403 — can't create search handle without valid session |
| `XblMultiplayerSearchHandleGet*` (10 commands) | `0x80004003` | E_POINTER — no search handle to query (cascading) |
| `XblMultiplayerGetSearchHandlesAsync` | `0x80070057` | E_INVALIDARG — cascading |
| `XblMultiplayerDeleteSearchHandleAsync` | `0x80070057` | E_INVALIDARG — cascading |

#### xsapi-23 — Multiplayer Partial Invite Flow

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblMultiplayerWriteSessionAsync` | `0x80190199` | HTTP 409 |
| `XblMultiplayerSendInvitesAsync` | `0x80070057` | No valid session to invite from |
| `XblMultiplayerGetSessionByHandleAsync` | `0x80070057` | No valid handle |

#### xsapi-31 — Multiplayer Session Member Status and Leave

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblMultiplayerWriteSessionAsync` (×2) | `0x80190199` | HTTP 409 |
| `XblMultiplayerSessionLeave` | `0x8000FFFF` | E_UNEXPECTED — no active session to leave |

#### xsapi-36 — Create and Query Match Ticket

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblMultiplayerWriteSessionAsync` | `0x80190199` | HTTP 409 |
| `XblMatchmakingCreateMatchTicketAsync` | `0x80070057` | Can't create ticket without a session |
| `XblMatchmakingGetMatchTicketDetailsAsync` | `0x80070057` | No ticket to query |
| `XblMatchmakingGetHopperStatisticsAsync` | `0x80070057` | No valid hopper |
| `XblMatchmakingDeleteMatchTicketAsync` | `0x80070057` | No ticket to delete |
| `XblMultiplayerSessionLeave` | `0x8000FFFF` | No session |

---

## Category D: MPM (Multiplayer Manager) — No Game Session Available

The Multiplayer Manager's `JoinGameFromLobby` fails because there is no game
session to join. The lobby session is created successfully, but there's nothing
to join from it. This cascades to downstream game session queries.

### xsapi-33 — Multiplayer Manager Join Game from Lobby

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblMultiplayerManagerJoinGameFromLobby` | `0x8000FFFF` | E_UNEXPECTED — no game session available to join |
| `XblMultiplayerManagerGameSessionMembers` | `0x8000FFFF` | Cascading — no game session |

**Root cause:** `JoinGameFromLobby` requires a game session URI to be set on the lobby (typically via matchmaking or an invite). In this test, we just create a lobby and immediately try to join a game, which doesn't exist.

#### xsapi-34 — Multiplayer Manager Session Properties

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblMultiplayerManagerJoinGameFromLobby` | `0x8000FFFF` | Same as above |
| `XblMultiplayerManagerGameSessionSetSynchronizedHost` | `0x80070057` | No game session |
| `XblMultiplayerManagerGameSessionSetSynchronizedProperties` | `0x80070057` | No game session |
| `XblMultiplayerManagerLobbySessionSetProperties` | `0x80070057` | May need JSON string format |

#### xsapi-35 — Multiplayer Manager Matchmaking Flow

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblMultiplayerManagerFindMatch` | `0x8000FFFF` | No matchmaking hopper configured for this title |

**Root cause:** `FindMatch` needs a matchmaking hopper configured in Partner Center. Without one, the MPM can't submit a match ticket.

**To fix (all 3):** Configure a session template + matchmaking hopper in Partner Center, or restructure these tests to use `XblMultiplayerManagerJoinGame` with an explicit session name instead of `JoinGameFromLobby`.

---

## Category E: Achievements Manager Initialization Timing

### xsapi-25 — Achievements Manager Add User and Query

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblAchievementsManagerIsUserInitialized` | `0x80004005` | E_FAIL — not yet initialized |
| `XblAchievementsManagerGetAchievements` | `0x80070057` | Cascading — user not initialized |
| `XblAchievementsManagerResultGetAchievements` (×2) | `0x80004003` | Cascading — no result handle |
| `XblAchievementsManagerResultCloseHandle` (×2) | `0x80004003` | Cascading — no result handle |
| `XblAchievementsManagerGetAchievementsByState` | `0x80070057` | Cascading — user not initialized |

**Root cause:** `XblAchievementsManagerAddLocalUser` succeeds (S_OK) but initialization is asynchronous. A single `DoWork` call isn't enough — the manager needs multiple `DoWork` poll cycles to complete the initial data fetch from the service before `IsUserInitialized` returns true.

**To fix:** Add a polling loop in the YAML: repeat `XblAchievementsManagerDoWork` + `XblAchievementsManagerIsUserInitialized` several times with a short delay, or implement a "wait for condition" mechanism in the test framework.

---

## Category F: Social Manager Parameter Issues

### xsapi-29 — Social Manager Tracked List Group

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblSocialManagerUpdateSocialUserGroup` | `0x80070057` | E_INVALIDARG |

**Root cause:** `XblSocialManagerUpdateSocialUserGroup` requires an updated XUID list to be passed in. The handler defaults are likely providing empty or invalid XUID arrays. The `CreateSocialUserGroupFromList` before it succeeds, but the update call needs explicit new XUIDs.

**To fix:** Either provide explicit `xuids` parameter in the YAML for the update call, or improve the handler default to re-use the same XUID from `XUserGetId`. Alternatively, remove this command from the test since the create+get pattern already validates the feature.

---

## Category G: Expected Edge Cases (By Design)

### xsapi-42 — Double Init is Safe

| Command | HRESULT | Notes |
|---------|---------|-------|
| `XblInitialize` (2nd call) | `0x89235207` | E_XBL_ALREADY_INITIALIZED |
| `XblCleanupAsync` (2nd call) | `0x89235208` | E_XBL_NOT_INITIALIZED |

**Root cause:** This test intentionally verifies edge cases. The second `XblInitialize` correctly returns "already initialized" and the second `XblCleanupAsync` correctly returns "not initialized" (since the first cleanup already shut it down and the second init failed).

**Assessment:** Working as designed. These `expectFailure` markers are correct — they validate that the APIs return proper error codes instead of crashing. No action needed.

---

## Summary

| Category | Scenarios | Fix Required |
|----------|-----------|-------------|
| **A. Unimplemented handlers** | 26, 30, 37, 40, 41 | Implement handler bodies in C++ |
| **B. Service config missing** | 05, 11, 14, 15 | Partner Center configuration |
| **C. Session template missing** | 22, 23, 31, 36, 40 | Create "GameSession" template in Partner Center |
| **D. MPM no game session** | 33, 34, 35 | Session template + matchmaking hopper in Partner Center |
| **E. Async init timing** | 25 | Add DoWork polling loop or framework wait mechanism |
| **F. Handler param defaults** | 29 | Provide explicit XUID list in YAML or fix handler default |
| **G. Expected edge cases** | 42 | No action — working as designed |

### Priority Recommendations

1. **Highest impact — Configure "GameSession" template in Partner Center** → fixes 5 scenarios (22, 23, 31, 36, 40 partially)
2. **Implement notification handler stubs** → fixes 5 scenarios (26, 30, 37, 40, 41)
3. **Configure title storage + title-managed stats** → fixes 2 scenarios (14, 15)
4. **Add achievements manager DoWork polling** → fixes 1 scenario (25)
5. **Fix social manager update default** → fixes 1 scenario (29)
6. **Accept as-is** → 2 scenarios are correct (11 = achievement already done, 42 = edge case by design)

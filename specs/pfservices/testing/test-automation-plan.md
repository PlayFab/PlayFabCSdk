# PlayFab Services Test Automation Plan

## Purpose
Define customer-centric test scenarios for the PlayFab Services C SDK that catch regressions in the service APIs game developers depend on most. These cover catalog & economy (inventory, leaderboards, statistics), player data, social (friends, groups), profiles, cloud script, multiplayer servers, and title management — the features that power live-service games.

## Selection Philosophy
- **Economy First**: Catalog, inventory, leaderboards, and statistics are the backbone of most F2P and live-service titles. Test full CRUD lifecycles.
- **Player Data**: User data read/write is in nearly every title. Cover client and server paths, publisher vs. title scoping.
- **Social**: Friends and groups drive engagement. Test add/remove flows and group membership lifecycle.
- **Cloud Script**: Custom server logic is how titles extend PlayFab. Test client, server, and Azure Functions execution.
- **Profiles**: Player identity display and cross-title lookups. Test get/set patterns.
- **Account Management**: Link/unlink identity providers, display name updates, bans. Focus on the most common GDK paths.
- **Title Management**: Title data, news, and publisher data are read by every game client at startup.
- **Edge Cases**: Invalid parameters, concurrent operations, large payloads.

## Coverage Legend

| Status | Meaning |
|--------|---------|
| ✅ `pfservices-NN-name.yml` | Covered by the named YML scenario |
| ✅ `pfservices-01-catalog-draft-crud.yml` | Scenario defined but no YML exists yet |

## Test Scenario List

### Catalog

1. **Create, get, and delete draft item**
   Create a draft catalog item with `PFCatalogCreateDraftItemAsync`, retrieve it with `PFCatalogGetDraftItemAsync`, verify properties match, then delete with `PFCatalogDeleteItemAsync`.
   > ✅ `pfservices-02-catalog-update-draft.yml`

2. **Update draft item**
   Create a draft item, update it with `PFCatalogUpdateDraftItemAsync` (change title/description), retrieve again and verify changes persisted.
   > ✅ `pfservices-03-catalog-publish.yml`

3. **Publish and get published item**
   Create a draft item, publish with `PFCatalogPublishDraftItemAsync`, then retrieve the published item with `PFCatalogGetItemAsync`. Verify it's publicly visible.
   > ✅ `pfservices-04-catalog-search.yml`

4. **Search catalog items**
   After publishing items, call `PFCatalogSearchItemsAsync` with a filter or search query. Verify results contain expected items.
   > ✅ `pfservices-05-catalog-config.yml`

5. **Catalog config get/update**
   Call `PFCatalogGetCatalogConfigAsync` to read current config, update with `PFCatalogUpdateCatalogConfigAsync`, read again to verify.
   > ✅ `pfservices-06-catalog-reviews.yml`

6. **Item reviews lifecycle**
   Create and publish an item, submit a review with `PFCatalogReviewItemAsync`, get the review with `PFCatalogGetEntityItemReviewAsync`, get the review summary, vote on it, then clean up.
   > ✅ `pfservices-07-catalog-upload-urls.yml`

7. **Create upload URLs**
   Call `PFCatalogCreateUploadUrlsAsync` to get pre-signed URLs for asset upload. Verify URLs are returned with valid structure.
   > ✅ `pfservices-08-catalog-bulk-get.yml`

8. **Get multiple items (bulk)**
   Call `PFCatalogGetItemsAsync` with multiple item IDs. Verify all requested items are returned.
   > ✅ `pfservices-09-catalog-entity-drafts.yml`

9. **Get entity draft items**
   Call `PFCatalogGetEntityDraftItemsAsync` to list all draft items for the current entity. Verify the list contains previously created drafts.
   > ✅ `pfservices-10-catalog-moderation.yml`

10. **Item moderation state**
    Create an item, get its moderation state with `PFCatalogGetItemModerationStateAsync`, set moderation with `PFCatalogSetItemModerationStateAsync`, verify the change.
    > ✅ `pfservices-11-inventory-add-get.yml`

### Inventory

11. **Add and get inventory items**
    Call `PFInventoryAddInventoryItemsAsync` to add items to a player's inventory, then `PFInventoryGetInventoryItemsAsync` to verify they appear.
    > ✅ `pfservices-12-inventory-subtract.yml`

12. **Subtract inventory items**
    After adding items, call `PFInventorySubtractInventoryItemsAsync` to reduce quantity. Verify the updated inventory.
    > ✅ `pfservices-13-inventory-delete.yml`

13. **Delete inventory items**
    Add items, then delete specific items with `PFInventoryDeleteInventoryItemsAsync`. Verify they're removed from inventory.
    > ✅ `pfservices-14-inventory-update.yml`

14. **Update inventory items**
    Add items, update properties with `PFInventoryUpdateInventoryItemsAsync`. Retrieve and verify updated values.
    > ✅ `pfservices-15-inventory-purchase.yml`

15. **Purchase inventory items**
    Call `PFInventoryPurchaseInventoryItemsAsync` to buy an item using virtual currency. Verify the item is added and currency deducted.
    > ✅ `pfservices-16-inventory-batch-ops.yml`

16. **Execute inventory operations (batch)**
    Call `PFInventoryExecuteInventoryOperationsAsync` with multiple operations (add + subtract) in a single batch. Verify all operations complete atomically.
    > ✅ `pfservices-17-inventory-transfer.yml`

17. **Transfer inventory items**
    Call `PFInventoryTransferInventoryItemsAsync` to move items between collections or entities. Verify source decremented, target incremented.
    > ✅ `pfservices-18-inventory-transactions.yml`

18. **Get transaction history**
    After performing inventory operations, call `PFInventoryGetTransactionHistoryAsync` and verify the transactions are recorded.
    > ✅ `pfservices-19-inventory-collections.yml`

19. **Inventory collection management**
    Call `PFInventoryGetInventoryCollectionIdsAsync` to list collections, then `PFInventoryDeleteInventoryCollectionAsync` to delete a test collection.
    > ✅ `pfservices-20-inventory-op-status.yml`

20. **Get inventory operation status**
    Start a long-running inventory operation, then poll with `PFInventoryGetInventoryOperationStatusAsync` to check completion.
    > ✅ `pfservices-21-inventory-ms-store.yml`

21. **Redeem Microsoft Store items**
    Call `PFInventoryGetMicrosoftStoreAccessTokensAsync` to get tokens, then `PFInventoryRedeemMicrosoftStoreInventoryItemsAsync`. Verify redemption flow.
    > ✅ `pfservices-22-leaderboard-def-crud.yml`

### Leaderboards

22. **Create and delete leaderboard definition**
    Call `PFLeaderboardsCreateLeaderboardDefinitionAsync` with a test leaderboard, verify with `PFLeaderboardsGetLeaderboardDefinitionAsync`, then delete with `PFLeaderboardsDeleteLeaderboardDefinitionAsync`.
    > ✅ `pfservices-23-leaderboard-update-get.yml`

23. **Update leaderboard entries and get rankings**
    Update entries with `PFLeaderboardsUpdateLeaderboardEntriesAsync`, then get the leaderboard with `PFLeaderboardsGetLeaderboardAsync`. Verify rankings are correct.
    > ✅ `pfservices-24-leaderboard-around.yml`

24. **Get leaderboard around entity**
    After posting a score, call `PFLeaderboardsGetLeaderboardAroundEntityAsync` to get nearby rankings. Verify the calling entity appears in results.
    > ✅ `pfservices-25-leaderboard-friends.yml`

25. **Friend leaderboard**
    Call `PFLeaderboardsGetFriendLeaderboardForEntityAsync` to get a leaderboard filtered to friends. Verify results only include friend entities.
    > ✅ `pfservices-26-leaderboard-entities.yml`

26. **Get leaderboard for specific entities**
    Call `PFLeaderboardsGetLeaderboardForEntitiesAsync` with specific entity IDs. Verify only those entities' scores are returned.
    > ✅ `pfservices-27-leaderboard-list-defs.yml`

27. **List leaderboard definitions**
    Call `PFLeaderboardsListLeaderboardDefinitionsAsync` and verify it returns all configured leaderboards including the test one.
    > ✅ `pfservices-28-leaderboard-version.yml`

28. **Increment leaderboard version**
    Call `PFLeaderboardsIncrementLeaderboardVersionAsync` to reset the leaderboard. Verify the version incremented and old entries are cleared.
    > ✅ `pfservices-29-leaderboard-delete-entries.yml`

29. **Delete leaderboard entries**
    Post entries, then call `PFLeaderboardsDeleteLeaderboardEntriesAsync` to remove specific entries. Verify they're gone.
    > ✅ `pfservices-30-stats-def-crud.yml`

### Statistics

30. **Create, get, and delete statistic definition**
    Call `PFStatisticsCreateStatisticDefinitionAsync`, read back with `PFStatisticsGetStatisticDefinitionAsync`, then delete with `PFStatisticsDeleteStatisticDefinitionAsync`.
    > ✅ `pfservices-31-stats-update-get.yml`

31. **Update and get statistics for entity**
    Call `PFStatisticsUpdateStatisticsAsync` to set stats for a player, then `PFStatisticsGetStatisticsAsync` to read them back. Verify values match.
    > ✅ `pfservices-32-stats-multi-entity.yml`

32. **Get statistics for multiple entities**
    Call `PFStatisticsGetStatisticsForEntitiesAsync` with multiple entity IDs. Verify stats are returned for all requested entities.
    > ✅ `pfservices-33-stats-delete.yml`

33. **Delete statistics**
    After setting stats, call `PFStatisticsDeleteStatisticsAsync` to remove them. Verify they're gone.
    > ✅ `pfservices-34-stats-list-defs.yml`

34. **List statistic definitions**
    Call `PFStatisticsListStatisticDefinitionsAsync` and verify it returns all configured statistics.
    > ✅ `pfservices-35-stats-version.yml`

35. **Increment statistic version**
    Call `PFStatisticsIncrementStatisticVersionAsync` to reset a statistic. Verify the version incremented.
    > ✅ `pfservices-36-playerdata-client-roundtrip.yml`

### Player Data Management

36. **Client user data round-trip**
    Call `PFPlayerDataManagementClientUpdateUserDataAsync` to set key/value data, then `PFPlayerDataManagementClientGetUserDataAsync` to read it back. Verify values match.
    > ✅ `pfservices-37-playerdata-publisher.yml`

37. **Client publisher data read**
    Call `PFPlayerDataManagementClientGetUserPublisherDataAsync` to read publisher-scoped data. Verify data is returned.
    > ✅ `pfservices-38-playerdata-readonly.yml`

38. **Client read-only data**
    Call `PFPlayerDataManagementClientGetUserReadOnlyDataAsync` to verify read-only data is accessible but not writable from client.
    > ✅ `pfservices-39-playerdata-server-update.yml`

39. **Server user data update**
    Call `PFPlayerDataManagementServerUpdateUserDataAsync` with server credentials to set data, then read it back from client to verify cross-path consistency.
    > ✅ `pfservices-40-playerdata-server-publisher.yml`

40. **Server publisher data round-trip**
    Call `PFPlayerDataManagementServerUpdateUserPublisherDataAsync` to set data, then `PFPlayerDataManagementServerGetUserPublisherDataAsync` to read. Verify values.
    > ✅ `pfservices-41-playerdata-custom-props.yml`

41. **Player custom properties CRUD**
    Call `PFPlayerDataManagementClientUpdatePlayerCustomPropertiesAsync` to set custom properties, list with `PFPlayerDataManagementClientListPlayerCustomPropertiesAsync`, get specific with `PFPlayerDataManagementClientGetPlayerCustomPropertyAsync`, delete with `PFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync`.
    > ✅ `pfservices-42-friends-add-list-remove.yml`

### Friends

42. **Add, list, and remove friend**
    Call `PFFriendsClientAddFriendAsync` to add a friend, `PFFriendsClientGetFriendsListAsync` to verify they appear, then `PFFriendsClientRemoveFriendAsync` to remove. Verify removal.
    > ✅ `pfservices-43-friends-tags.yml`

43. **Set friend tags**
    Add a friend, call `PFFriendsClientSetFriendTagsAsync` to tag them, get friends list and verify tags are present.
    > ✅ `pfservices-44-friends-server.yml`

44. **Server friends management**
    Call `PFFriendsServerAddFriendAsync`, `PFFriendsServerGetFriendsListAsync`, and `PFFriendsServerRemoveFriendAsync` to test server-side friend operations.
    > ✅ `pfservices-45-groups-crud.yml`

### Groups

45. **Create, get, and delete group**
    Call `PFGroupsCreateGroupAsync`, verify with `PFGroupsGetGroupAsync`, then `PFGroupsDeleteGroupAsync`. Full lifecycle test.
    > ✅ `pfservices-46-groups-apply-accept.yml`

46. **Group membership: apply and accept**
    Create a group, have another entity apply with `PFGroupsApplyToGroupAsync`, list applications with `PFGroupsListGroupApplicationsAsync`, accept with `PFGroupsAcceptGroupApplicationAsync`, verify membership with `PFGroupsListGroupMembersAsync`.
    > ✅ `pfservices-47-groups-invite.yml`

47. **Group invitation flow**
    Create a group, invite an entity with `PFGroupsInviteToGroupAsync`, list invitations, accept the invitation with `PFGroupsAcceptGroupInvitationAsync`, verify membership.
    > ✅ `pfservices-48-groups-roles.yml`

48. **Group roles**
    Create a group, create a role with `PFGroupsCreateRoleAsync`, change member role with `PFGroupsChangeMemberRoleAsync`, update role with `PFGroupsUpdateRoleAsync`, delete role with `PFGroupsDeleteRoleAsync`.
    > ✅ `pfservices-49-groups-block.yml`

49. **Group block/unblock**
    Create a group, block an entity with `PFGroupsBlockEntityAsync`, list blocks with `PFGroupsListGroupBlocksAsync`, unblock with `PFGroupsUnblockEntityAsync`.
    > ✅ `pfservices-50-groups-membership.yml`

50. **Is member and list membership**
    After adding members, call `PFGroupsIsMemberAsync` to check membership, `PFGroupsListMembershipAsync` to list groups the entity belongs to.
    > ✅ `pfservices-51-profiles-get.yml`

### Profiles

51. **Get player profile**
    Call `PFProfilesGetProfileAsync` for the current entity, verify display name, avatar URL, and other profile fields.
    > ✅ `pfservices-52-profiles-get-multi.yml`

52. **Get multiple profiles**
    Call `PFProfilesGetProfilesAsync` with multiple entity keys. Verify all requested profiles are returned.
    > ✅ `pfservices-53-profiles-language.yml`

53. **Set profile language**
    Call `PFProfilesSetProfileLanguageAsync` to change the profile language, get profile again and verify the language updated.
    > ✅ `pfservices-54-profiles-title-players.yml`

54. **Get title players from master player account**
    Call `PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsAsync` to look up title-specific players from master accounts. Verify the mapping.
    > ✅ `pfservices-55-profiles-policy.yml`

55. **Set profile policy**
    Call `PFProfilesSetProfilePolicyAsync` to update access policies, verify the change took effect.
    > ✅ `pfservices-56-account-info.yml`

### Account Management

56. **Get account info**
    Call `PFAccountManagementClientGetAccountInfoAsync` for the current player. Verify it returns valid account data.
    > ✅ `pfservices-57-account-combined-info.yml`

57. **Get player combined info**
    Call `PFAccountManagementClientGetPlayerCombinedInfoAsync` to get all player info in one call. Verify account, data, and inventory sections.
    > ✅ `pfservices-58-account-display-name.yml`

58. **Update display name**
    Call `PFAccountManagementClientUpdateUserTitleDisplayNameAsync` with a new name, verify success. Also test `PFAccountManagementSetDisplayNameAsync` (entity API).
    > ✅ `pfservices-59-account-link-customid.yml`

59. **Link and unlink custom ID**
    Call `PFAccountManagementClientLinkCustomIDAsync` to link a custom identity, then `PFAccountManagementClientUnlinkCustomIDAsync` to unlink it.
    > ✅ `pfservices-60-account-link-xbox.yml`

60. **Link and unlink Xbox account**
    Call `PFAccountManagementClientLinkXboxAccountAsync` to link Xbox credentials, then `PFAccountManagementClientUnlinkXboxAccountAsync`. GDK-specific test.
    > ✅ `pfservices-61-account-pfid-xbox.yml`

61. **Get PlayFab IDs from Xbox Live IDs**
    Call `PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync` with known Xbox Live IDs. Verify PlayFab IDs are returned.
    > ✅ `pfservices-62-account-titleplayers-xbox.yml`

62. **Get title players from Xbox Live IDs**
    Call `PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync` to look up title players by Xbox Live ID. Verify mapping.
    > ✅ `pfservices-63-account-report.yml`

63. **Report player**
    Call `PFAccountManagementClientReportPlayerAsync` to report another player. Verify the report is accepted.
    > ✅ `pfservices-64-account-avatar.yml`

64. **Update avatar URL**
    Call `PFAccountManagementClientUpdateAvatarUrlAsync` to set a new avatar URL. Verify via profile retrieval.
    > ✅ `pfservices-65-account-bans.yml`

65. **Server ban/unban lifecycle**
    Call `PFAccountManagementServerBanUsersAsync` to ban a test user, `PFAccountManagementServerGetUserBansAsync` to verify, `PFAccountManagementServerRevokeAllBansForUserAsync` to unban.
    > ✅ `pfservices-66-account-add-credentials.yml`

66. **Add username and password**
    Call `PFAccountManagementClientAddUsernamePasswordAsync` to add credentials to an account created via anonymous login.
    > ✅ `pfservices-67-cloudscript-client.yml`

### Cloud Script

67. **Client execute cloud script**
    Call `PFCloudScriptClientExecuteCloudScriptAsync` with a known function name and parameters. Verify the result contains expected return values.
    > ✅ `pfservices-68-cloudscript-entity.yml`

68. **Execute entity cloud script**
    Call `PFCloudScriptExecuteEntityCloudScriptAsync` to run cloud script in entity context. Verify result.
    > ✅ `pfservices-69-cloudscript-function.yml`

69. **Execute Azure Function**
    Call `PFCloudScriptExecuteFunctionAsync` to invoke an Azure Function. Verify the function result.
    > ✅ `pfservices-70-cloudscript-server.yml`

70. **Server execute cloud script**
    Call `PFCloudScriptServerExecuteCloudScriptAsync` with server credentials. Verify result.
    > ✅ `pfservices-71-data-file-upload.yml`

### Data (Entity Files & Objects)

71. **File upload lifecycle**
    Call `PFDataInitiateFileUploadsAsync` to start upload, upload file data, call `PFDataFinalizeFileUploadsAsync` to complete. Verify with `PFDataGetFilesAsync`.
    > ✅ `pfservices-72-data-file-delete.yml`

72. **File delete**
    After uploading files, call `PFDataDeleteFilesAsync` to remove them. Verify they're gone.
    > ✅ `pfservices-73-data-file-abort.yml`

73. **Abort file upload**
    Start a file upload with `PFDataInitiateFileUploadsAsync`, then abort with `PFDataAbortFileUploadsAsync`. Verify cleanup.
    > ✅ `pfservices-74-data-objects.yml`

74. **Entity objects set and get**
    Call `PFDataSetObjectsAsync` to store JSON objects on an entity, then `PFDataGetObjectsAsync` to retrieve. Verify data matches.
    > ✅ `pfservices-75-titledata-get.yml`

### Title Data Management

75. **Get title data**
    Call `PFTitleDataManagementClientGetTitleDataAsync` to read title configuration data. Verify expected keys exist.
    > ✅ `pfservices-76-titledata-news.yml`

76. **Get title news**
    Call `PFTitleDataManagementClientGetTitleNewsAsync` to read news entries. Verify structure and content.
    > ✅ `pfservices-77-titledata-time.yml`

77. **Get server time**
    Call `PFTitleDataManagementClientGetTimeAsync` to get the server timestamp. Verify it's a valid recent datetime.
    > ✅ `pfservices-78-titledata-publisher.yml`

78. **Publisher data round-trip**
    Call `PFTitleDataManagementServerSetPublisherDataAsync` to set data, `PFTitleDataManagementClientGetPublisherDataAsync` to read from client, verify consistency.
    > ✅ `pfservices-79-titledata-server.yml`

79. **Server title data round-trip**
    Call `PFTitleDataManagementServerSetTitleDataAsync` to set data, `PFTitleDataManagementServerGetTitleDataAsync` to read it back.
    > ✅ `pfservices-80-titledata-internal.yml`

80. **Server title internal data**
    Call `PFTitleDataManagementServerSetTitleInternalDataAsync` to set internal-only data, `PFTitleDataManagementServerGetTitleInternalDataAsync` to read. Verify client cannot access it.
    > ✅ `pfservices-81-mps-build-aliases.yml`

### Multiplayer Servers

81. **List build aliases**
    Call `PFMultiplayerServerListBuildAliasesAsync` to enumerate build aliases. Verify structure.
    > ✅ `pfservices-82-mps-build-summaries.yml`

82. **List build summaries**
    Call `PFMultiplayerServerListBuildSummariesV2Async` to get build information. Verify build details.
    > ✅ `pfservices-83-mps-qos.yml`

83. **List QoS servers**
    Call `PFMultiplayerServerListQosServersForTitleAsync` to get QoS server endpoints. Verify at least one server is returned.
    > ✅ `pfservices-84-mps-request-server.yml`

84. **Request multiplayer server**
    Call `PFMultiplayerServerRequestMultiplayerServerAsync` to allocate a game server. Verify server details in response.
    > ✅ `pfservices-85-mps-secrets.yml`

85. **Secrets management**
    Call `PFMultiplayerServerUploadSecretAsync` to store a secret, `PFMultiplayerServerListSecretSummariesAsync` to list, `PFMultiplayerServerDeleteSecretAsync` to remove.
    > ✅ `pfservices-86-segments-get.yml`

### Segments

86. **Get player segments**
    Call `PFSegmentsClientGetPlayerSegmentsAsync` to get segments the current player belongs to. Verify "All Players" segment exists.
    > ✅ `pfservices-87-segments-tags.yml`

87. **Player tags lifecycle**
    Call `PFSegmentsServerAddPlayerTagAsync` to tag a player, `PFSegmentsClientGetPlayerTagsAsync` to verify, `PFSegmentsServerRemovePlayerTagAsync` to remove.
    > ✅ `pfservices-88-segments-players-in.yml`

88. **Get players in segment (server)**
    Call `PFSegmentsServerGetPlayersInSegmentAsync` for a known segment. Verify player list is returned.
    > ✅ `pfservices-89-experimentation-treatment.yml`

### Experimentation

89. **Get treatment assignment**
    Call `PFExperimentationGetTreatmentAssignmentAsync` to get the player's A/B test assignment. Verify valid treatment info is returned.
    > ✅ `pfservices-90-localization-languages.yml`

### Localization

90. **Get language list**
    Call `PFLocalizationGetLanguageListAsync` to retrieve supported languages. Verify a substantial list is returned (expected ~384 languages).
    > ✅ `pfservices-91-push-send.yml`

### Push Notifications (Server)

91. **Send push notification**
    Call `PFPushNotificationsServerSendPushNotificationAsync` to send a test notification. Verify success.
    > ✅ `pfservices-92-push-template.yml`

92. **Send push from template**
    Call `PFPushNotificationsServerSendPushNotificationFromTemplateAsync` to send using a predefined template. Verify success.
    > ✅ `pfservices-93-edge-large-payload.yml`

### Cross-Cutting & Edge Cases

93. **Large payload data write**
    Write a large JSON object (near size limit) via `PFDataSetObjectsAsync`. Verify the data is stored and retrievable without truncation.
    > ✅ `pfservices-94-edge-concurrent-search.yml`

94. **Concurrent catalog operations**
    Start multiple `PFCatalogSearchItemsAsync` calls in parallel. Verify all complete without errors or data corruption.
    > ✅ `pfservices-95-edge-inventory-idempotency.yml`

95. **Inventory idempotency**
    Execute the same `PFInventoryExecuteInventoryOperationsAsync` with the same idempotency key twice. Verify the second call doesn't duplicate the operation.
    > ✅ `pfservices-95-edge-inventory-idempotency.yml`

## Coverage Summary

| Category | Scenarios | Covered | Not Yet |
|----------|-----------|---------|---------|
| Catalog | 10 | 10 | 0 |
| Inventory | 11 | 11 | 0 |
| Leaderboards | 8 | 8 | 0 |
| Statistics | 6 | 6 | 0 |
| Player Data Management | 6 | 6 | 0 |
| Friends | 3 | 3 | 0 |
| Groups | 6 | 6 | 0 |
| Profiles | 5 | 5 | 0 |
| Account Management | 11 | 11 | 0 |
| Cloud Script | 4 | 4 | 0 |
| Data (Files & Objects) | 4 | 4 | 0 |
| Title Data Management | 6 | 6 | 0 |
| Multiplayer Servers | 5 | 5 | 0 |
| Segments | 3 | 3 | 0 |
| Experimentation | 1 | 1 | 0 |
| Localization | 1 | 1 | 0 |
| Push Notifications | 2 | 2 | 0 |
| Cross-Cutting & Edge Cases | 3 | 3 | 0 |
| **Total** | **95** | **95** | **0** |

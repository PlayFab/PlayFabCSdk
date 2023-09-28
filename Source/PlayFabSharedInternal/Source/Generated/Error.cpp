#include "pch.h"
#include "Error.h"
#include <playfab/core/PFErrors.h>

namespace PlayFab
{ 

HRESULT ServiceErrorToHR(ServiceErrorCode errorCode)
{
    switch (errorCode)
    {
    case ServiceErrorCode::Success: return S_OK;
    case ServiceErrorCode::UnkownError: return E_PF_UNKOWN_ERROR;
    case ServiceErrorCode::InvalidParams: return E_PF_INVALID_PARAMS;
    case ServiceErrorCode::AccountNotFound: return E_PF_ACCOUNT_NOT_FOUND;
    case ServiceErrorCode::AccountBanned: return E_PF_ACCOUNT_BANNED;
    case ServiceErrorCode::InvalidUsernameOrPassword: return E_PF_INVALID_USERNAME_OR_PASSWORD;
    case ServiceErrorCode::InvalidTitleId: return E_PF_INVALID_TITLE_ID;
    case ServiceErrorCode::InvalidEmailAddress: return E_PF_INVALID_EMAIL_ADDRESS;
    case ServiceErrorCode::EmailAddressNotAvailable: return E_PF_EMAIL_ADDRESS_NOT_AVAILABLE;
    case ServiceErrorCode::InvalidUsername: return E_PF_INVALID_USERNAME;
    case ServiceErrorCode::InvalidPassword: return E_PF_INVALID_PASSWORD;
    case ServiceErrorCode::UsernameNotAvailable: return E_PF_USERNAME_NOT_AVAILABLE;
    case ServiceErrorCode::InvalidSteamTicket: return E_PF_INVALID_STEAM_TICKET;
    case ServiceErrorCode::AccountAlreadyLinked: return E_PF_ACCOUNT_ALREADY_LINKED;
    case ServiceErrorCode::LinkedAccountAlreadyClaimed: return E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED;
    case ServiceErrorCode::InvalidFacebookToken: return E_PF_INVALID_FACEBOOK_TOKEN;
    case ServiceErrorCode::AccountNotLinked: return E_PF_ACCOUNT_NOT_LINKED;
    case ServiceErrorCode::FailedByPaymentProvider: return E_PF_FAILED_BY_PAYMENT_PROVIDER;
    case ServiceErrorCode::CouponCodeNotFound: return E_PF_COUPON_CODE_NOT_FOUND;
    case ServiceErrorCode::InvalidContainerItem: return E_PF_INVALID_CONTAINER_ITEM;
    case ServiceErrorCode::ContainerNotOwned: return E_PF_CONTAINER_NOT_OWNED;
    case ServiceErrorCode::KeyNotOwned: return E_PF_KEY_NOT_OWNED;
    case ServiceErrorCode::InvalidItemIdInTable: return E_PF_INVALID_ITEM_ID_IN_TABLE;
    case ServiceErrorCode::InvalidReceipt: return E_PF_INVALID_RECEIPT;
    case ServiceErrorCode::ReceiptAlreadyUsed: return E_PF_RECEIPT_ALREADY_USED;
    case ServiceErrorCode::ReceiptCancelled: return E_PF_RECEIPT_CANCELLED;
    case ServiceErrorCode::GameNotFound: return E_PF_GAME_NOT_FOUND;
    case ServiceErrorCode::GameModeNotFound: return E_PF_GAME_MODE_NOT_FOUND;
    case ServiceErrorCode::InvalidGoogleToken: return E_PF_INVALID_GOOGLE_TOKEN;
    case ServiceErrorCode::UserIsNotPartOfDeveloper: return E_PF_USER_IS_NOT_PART_OF_DEVELOPER;
    case ServiceErrorCode::InvalidTitleForDeveloper: return E_PF_INVALID_TITLE_FOR_DEVELOPER;
    case ServiceErrorCode::TitleNameConflicts: return E_PF_TITLE_NAME_CONFLICTS;
    case ServiceErrorCode::UserisNotValid: return E_PF_USERIS_NOT_VALID;
    case ServiceErrorCode::ValueAlreadyExists: return E_PF_VALUE_ALREADY_EXISTS;
    case ServiceErrorCode::BuildNotFound: return E_PF_BUILD_NOT_FOUND;
    case ServiceErrorCode::PlayerNotInGame: return E_PF_PLAYER_NOT_IN_GAME;
    case ServiceErrorCode::InvalidTicket: return E_PF_INVALID_TICKET;
    case ServiceErrorCode::InvalidDeveloper: return E_PF_INVALID_DEVELOPER;
    case ServiceErrorCode::InvalidOrderInfo: return E_PF_INVALID_ORDER_INFO;
    case ServiceErrorCode::RegistrationIncomplete: return E_PF_REGISTRATION_INCOMPLETE;
    case ServiceErrorCode::InvalidPlatform: return E_PF_INVALID_PLATFORM;
    case ServiceErrorCode::UnknownError: return E_PF_UNKNOWN_ERROR;
    case ServiceErrorCode::SteamApplicationNotOwned: return E_PF_STEAM_APPLICATION_NOT_OWNED;
    case ServiceErrorCode::WrongSteamAccount: return E_PF_WRONG_STEAM_ACCOUNT;
    case ServiceErrorCode::TitleNotActivated: return E_PF_TITLE_NOT_ACTIVATED;
    case ServiceErrorCode::RegistrationSessionNotFound: return E_PF_REGISTRATION_SESSION_NOT_FOUND;
    case ServiceErrorCode::NoSuchMod: return E_PF_NO_SUCH_MOD;
    case ServiceErrorCode::FileNotFound: return E_PF_FILE_NOT_FOUND;
    case ServiceErrorCode::DuplicateEmail: return E_PF_DUPLICATE_EMAIL;
    case ServiceErrorCode::ItemNotFound: return E_PF_ITEM_NOT_FOUND;
    case ServiceErrorCode::ItemNotOwned: return E_PF_ITEM_NOT_OWNED;
    case ServiceErrorCode::ItemNotRecycleable: return E_PF_ITEM_NOT_RECYCLEABLE;
    case ServiceErrorCode::ItemNotAffordable: return E_PF_ITEM_NOT_AFFORDABLE;
    case ServiceErrorCode::InvalidVirtualCurrency: return E_PF_INVALID_VIRTUAL_CURRENCY;
    case ServiceErrorCode::WrongVirtualCurrency: return E_PF_WRONG_VIRTUAL_CURRENCY;
    case ServiceErrorCode::WrongPrice: return E_PF_WRONG_PRICE;
    case ServiceErrorCode::NonPositiveValue: return E_PF_NON_POSITIVE_VALUE;
    case ServiceErrorCode::InvalidRegion: return E_PF_INVALID_REGION;
    case ServiceErrorCode::RegionAtCapacity: return E_PF_REGION_AT_CAPACITY;
    case ServiceErrorCode::ServerFailedToStart: return E_PF_SERVER_FAILED_TO_START;
    case ServiceErrorCode::NameNotAvailable: return E_PF_NAME_NOT_AVAILABLE;
    case ServiceErrorCode::InsufficientFunds: return E_PF_INSUFFICIENT_FUNDS;
    case ServiceErrorCode::InvalidDeviceID: return E_PF_INVALID_DEVICE_ID;
    case ServiceErrorCode::InvalidPushNotificationToken: return E_PF_INVALID_PUSH_NOTIFICATION_TOKEN;
    case ServiceErrorCode::NoRemainingUses: return E_PF_NO_REMAINING_USES;
    case ServiceErrorCode::InvalidPaymentProvider: return E_PF_INVALID_PAYMENT_PROVIDER;
    case ServiceErrorCode::PurchaseInitializationFailure: return E_PF_PURCHASE_INITIALIZATION_FAILURE;
    case ServiceErrorCode::DuplicateUsername: return E_PF_DUPLICATE_USERNAME;
    case ServiceErrorCode::InvalidBuyerInfo: return E_PF_INVALID_BUYER_INFO;
    case ServiceErrorCode::NoGameModeParamsSet: return E_PF_NO_GAME_MODE_PARAMS_SET;
    case ServiceErrorCode::BodyTooLarge: return E_PF_BODY_TOO_LARGE;
    case ServiceErrorCode::ReservedWordInBody: return E_PF_RESERVED_WORD_IN_BODY;
    case ServiceErrorCode::InvalidTypeInBody: return E_PF_INVALID_TYPE_IN_BODY;
    case ServiceErrorCode::InvalidRequest: return E_PF_INVALID_REQUEST;
    case ServiceErrorCode::ReservedEventName: return E_PF_RESERVED_EVENT_NAME;
    case ServiceErrorCode::InvalidUserStatistics: return E_PF_INVALID_USER_STATISTICS;
    case ServiceErrorCode::NotAuthenticated: return E_PF_NOT_AUTHENTICATED;
    case ServiceErrorCode::StreamAlreadyExists: return E_PF_STREAM_ALREADY_EXISTS;
    case ServiceErrorCode::ErrorCreatingStream: return E_PF_ERROR_CREATING_STREAM;
    case ServiceErrorCode::StreamNotFound: return E_PF_STREAM_NOT_FOUND;
    case ServiceErrorCode::InvalidAccount: return E_PF_INVALID_ACCOUNT;
    case ServiceErrorCode::PurchaseDoesNotExist: return E_PF_PURCHASE_DOES_NOT_EXIST;
    case ServiceErrorCode::InvalidPurchaseTransactionStatus: return E_PF_INVALID_PURCHASE_TRANSACTION_STATUS;
    case ServiceErrorCode::APINotEnabledForGameClientAccess: return E_PF_API_NOT_ENABLED_FOR_GAME_CLIENT_ACCESS;
    case ServiceErrorCode::NoPushNotificationARNForTitle: return E_PF_NO_PUSH_NOTIFICATION_ARN_FOR_TITLE;
    case ServiceErrorCode::BuildAlreadyExists: return E_PF_BUILD_ALREADY_EXISTS;
    case ServiceErrorCode::BuildPackageDoesNotExist: return E_PF_BUILD_PACKAGE_DOES_NOT_EXIST;
    case ServiceErrorCode::CustomAnalyticsEventsNotEnabledForTitle: return E_PF_CUSTOM_ANALYTICS_EVENTS_NOT_ENABLED_FOR_TITLE;
    case ServiceErrorCode::InvalidSharedGroupId: return E_PF_INVALID_SHARED_GROUP_ID;
    case ServiceErrorCode::NotAuthorized: return E_PF_NOT_AUTHORIZED;
    case ServiceErrorCode::MissingTitleGoogleProperties: return E_PF_MISSING_TITLE_GOOGLE_PROPERTIES;
    case ServiceErrorCode::InvalidItemProperties: return E_PF_INVALID_ITEM_PROPERTIES;
    case ServiceErrorCode::InvalidPSNAuthCode: return E_PF_INVALID_PSN_AUTH_CODE;
    case ServiceErrorCode::InvalidItemId: return E_PF_INVALID_ITEM_ID;
    case ServiceErrorCode::PushNotEnabledForAccount: return E_PF_PUSH_NOT_ENABLED_FOR_ACCOUNT;
    case ServiceErrorCode::PushServiceError: return E_PF_PUSH_SERVICE_ERROR;
    case ServiceErrorCode::ReceiptDoesNotContainInAppItems: return E_PF_RECEIPT_DOES_NOT_CONTAIN_IN_APP_ITEMS;
    case ServiceErrorCode::ReceiptContainsMultipleInAppItems: return E_PF_RECEIPT_CONTAINS_MULTIPLE_IN_APP_ITEMS;
    case ServiceErrorCode::InvalidBundleID: return E_PF_INVALID_BUNDLE_ID;
    case ServiceErrorCode::JavascriptException: return E_PF_JAVASCRIPT_EXCEPTION;
    case ServiceErrorCode::InvalidSessionTicket: return E_PF_INVALID_SESSION_TICKET;
    case ServiceErrorCode::UnableToConnectToDatabase: return E_PF_UNABLE_TO_CONNECT_TO_DATABASE;
    case ServiceErrorCode::InternalServerError: return E_PF_INTERNAL_SERVER_ERROR;
    case ServiceErrorCode::InvalidReportDate: return E_PF_INVALID_REPORT_DATE;
    case ServiceErrorCode::ReportNotAvailable: return E_PF_REPORT_NOT_AVAILABLE;
    case ServiceErrorCode::DatabaseThroughputExceeded: return E_PF_DATABASE_THROUGHPUT_EXCEEDED;
    case ServiceErrorCode::InvalidGameTicket: return E_PF_INVALID_GAME_TICKET;
    case ServiceErrorCode::ExpiredGameTicket: return E_PF_EXPIRED_GAME_TICKET;
    case ServiceErrorCode::GameTicketDoesNotMatchLobby: return E_PF_GAME_TICKET_DOES_NOT_MATCH_LOBBY;
    case ServiceErrorCode::LinkedDeviceAlreadyClaimed: return E_PF_LINKED_DEVICE_ALREADY_CLAIMED;
    case ServiceErrorCode::DeviceAlreadyLinked: return E_PF_DEVICE_ALREADY_LINKED;
    case ServiceErrorCode::DeviceNotLinked: return E_PF_DEVICE_NOT_LINKED;
    case ServiceErrorCode::PartialFailure: return E_PF_PARTIAL_FAILURE;
    case ServiceErrorCode::PublisherNotSet: return E_PF_PUBLISHER_NOT_SET;
    case ServiceErrorCode::ServiceUnavailable: return E_PF_SERVICE_UNAVAILABLE;
    case ServiceErrorCode::VersionNotFound: return E_PF_VERSION_NOT_FOUND;
    case ServiceErrorCode::RevisionNotFound: return E_PF_REVISION_NOT_FOUND;
    case ServiceErrorCode::InvalidPublisherId: return E_PF_INVALID_PUBLISHER_ID;
    case ServiceErrorCode::DownstreamServiceUnavailable: return E_PF_DOWNSTREAM_SERVICE_UNAVAILABLE;
    case ServiceErrorCode::APINotIncludedInTitleUsageTier: return E_PF_API_NOT_INCLUDED_IN_TITLE_USAGE_TIER;
    case ServiceErrorCode::DAULimitExceeded: return E_PF_DAU_LIMIT_EXCEEDED;
    case ServiceErrorCode::APIRequestLimitExceeded: return E_PF_API_REQUEST_LIMIT_EXCEEDED;
    case ServiceErrorCode::InvalidAPIEndpoint: return E_PF_INVALID_API_ENDPOINT;
    case ServiceErrorCode::BuildNotAvailable: return E_PF_BUILD_NOT_AVAILABLE;
    case ServiceErrorCode::ConcurrentEditError: return E_PF_CONCURRENT_EDIT_ERROR;
    case ServiceErrorCode::ContentNotFound: return E_PF_CONTENT_NOT_FOUND;
    case ServiceErrorCode::CharacterNotFound: return E_PF_CHARACTER_NOT_FOUND;
    case ServiceErrorCode::CloudScriptNotFound: return E_PF_CLOUD_SCRIPT_NOT_FOUND;
    case ServiceErrorCode::ContentQuotaExceeded: return E_PF_CONTENT_QUOTA_EXCEEDED;
    case ServiceErrorCode::InvalidCharacterStatistics: return E_PF_INVALID_CHARACTER_STATISTICS;
    case ServiceErrorCode::PhotonNotEnabledForTitle: return E_PF_PHOTON_NOT_ENABLED_FOR_TITLE;
    case ServiceErrorCode::PhotonApplicationNotFound: return E_PF_PHOTON_APPLICATION_NOT_FOUND;
    case ServiceErrorCode::PhotonApplicationNotAssociatedWithTitle: return E_PF_PHOTON_APPLICATION_NOT_ASSOCIATED_WITH_TITLE;
    case ServiceErrorCode::InvalidEmailOrPassword: return E_PF_INVALID_EMAIL_OR_PASSWORD;
    case ServiceErrorCode::FacebookAPIError: return E_PF_FACEBOOK_API_ERROR;
    case ServiceErrorCode::InvalidContentType: return E_PF_INVALID_CONTENT_TYPE;
    case ServiceErrorCode::KeyLengthExceeded: return E_PF_KEY_LENGTH_EXCEEDED;
    case ServiceErrorCode::DataLengthExceeded: return E_PF_DATA_LENGTH_EXCEEDED;
    case ServiceErrorCode::TooManyKeys: return E_PF_TOO_MANY_KEYS;
    case ServiceErrorCode::FreeTierCannotHaveVirtualCurrency: return E_PF_FREE_TIER_CANNOT_HAVE_VIRTUAL_CURRENCY;
    case ServiceErrorCode::MissingAmazonSharedKey: return E_PF_MISSING_AMAZON_SHARED_KEY;
    case ServiceErrorCode::AmazonValidationError: return E_PF_AMAZON_VALIDATION_ERROR;
    case ServiceErrorCode::InvalidPSNIssuerId: return E_PF_INVALID_PSN_ISSUER_ID;
    case ServiceErrorCode::PSNInaccessible: return E_PF_PSN_INACCESSIBLE;
    case ServiceErrorCode::ExpiredAuthToken: return E_PF_EXPIRED_AUTH_TOKEN;
    case ServiceErrorCode::FailedToGetEntitlements: return E_PF_FAILED_TO_GET_ENTITLEMENTS;
    case ServiceErrorCode::FailedToConsumeEntitlement: return E_PF_FAILED_TO_CONSUME_ENTITLEMENT;
    case ServiceErrorCode::TradeAcceptingUserNotAllowed: return E_PF_TRADE_ACCEPTING_USER_NOT_ALLOWED;
    case ServiceErrorCode::TradeInventoryItemIsAssignedToCharacter: return E_PF_TRADE_INVENTORY_ITEM_IS_ASSIGNED_TO_CHARACTER;
    case ServiceErrorCode::TradeInventoryItemIsBundle: return E_PF_TRADE_INVENTORY_ITEM_IS_BUNDLE;
    case ServiceErrorCode::TradeStatusNotValidForCancelling: return E_PF_TRADE_STATUS_NOT_VALID_FOR_CANCELLING;
    case ServiceErrorCode::TradeStatusNotValidForAccepting: return E_PF_TRADE_STATUS_NOT_VALID_FOR_ACCEPTING;
    case ServiceErrorCode::TradeDoesNotExist: return E_PF_TRADE_DOES_NOT_EXIST;
    case ServiceErrorCode::TradeCancelled: return E_PF_TRADE_CANCELLED;
    case ServiceErrorCode::TradeAlreadyFilled: return E_PF_TRADE_ALREADY_FILLED;
    case ServiceErrorCode::TradeWaitForStatusTimeout: return E_PF_TRADE_WAIT_FOR_STATUS_TIMEOUT;
    case ServiceErrorCode::TradeInventoryItemExpired: return E_PF_TRADE_INVENTORY_ITEM_EXPIRED;
    case ServiceErrorCode::TradeMissingOfferedAndAcceptedItems: return E_PF_TRADE_MISSING_OFFERED_AND_ACCEPTED_ITEMS;
    case ServiceErrorCode::TradeAcceptedItemIsBundle: return E_PF_TRADE_ACCEPTED_ITEM_IS_BUNDLE;
    case ServiceErrorCode::TradeAcceptedItemIsStackable: return E_PF_TRADE_ACCEPTED_ITEM_IS_STACKABLE;
    case ServiceErrorCode::TradeInventoryItemInvalidStatus: return E_PF_TRADE_INVENTORY_ITEM_INVALID_STATUS;
    case ServiceErrorCode::TradeAcceptedCatalogItemInvalid: return E_PF_TRADE_ACCEPTED_CATALOG_ITEM_INVALID;
    case ServiceErrorCode::TradeAllowedUsersInvalid: return E_PF_TRADE_ALLOWED_USERS_INVALID;
    case ServiceErrorCode::TradeInventoryItemDoesNotExist: return E_PF_TRADE_INVENTORY_ITEM_DOES_NOT_EXIST;
    case ServiceErrorCode::TradeInventoryItemIsConsumed: return E_PF_TRADE_INVENTORY_ITEM_IS_CONSUMED;
    case ServiceErrorCode::TradeInventoryItemIsStackable: return E_PF_TRADE_INVENTORY_ITEM_IS_STACKABLE;
    case ServiceErrorCode::TradeAcceptedItemsMismatch: return E_PF_TRADE_ACCEPTED_ITEMS_MISMATCH;
    case ServiceErrorCode::InvalidKongregateToken: return E_PF_INVALID_KONGREGATE_TOKEN;
    case ServiceErrorCode::FeatureNotConfiguredForTitle: return E_PF_FEATURE_NOT_CONFIGURED_FOR_TITLE;
    case ServiceErrorCode::NoMatchingCatalogItemForReceipt: return E_PF_NO_MATCHING_CATALOG_ITEM_FOR_RECEIPT;
    case ServiceErrorCode::InvalidCurrencyCode: return E_PF_INVALID_CURRENCY_CODE;
    case ServiceErrorCode::NoRealMoneyPriceForCatalogItem: return E_PF_NO_REAL_MONEY_PRICE_FOR_CATALOG_ITEM;
    case ServiceErrorCode::TradeInventoryItemIsNotTradable: return E_PF_TRADE_INVENTORY_ITEM_IS_NOT_TRADABLE;
    case ServiceErrorCode::TradeAcceptedCatalogItemIsNotTradable: return E_PF_TRADE_ACCEPTED_CATALOG_ITEM_IS_NOT_TRADABLE;
    case ServiceErrorCode::UsersAlreadyFriends: return E_PF_USERS_ALREADY_FRIENDS;
    case ServiceErrorCode::LinkedIdentifierAlreadyClaimed: return E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED;
    case ServiceErrorCode::CustomIdNotLinked: return E_PF_CUSTOM_ID_NOT_LINKED;
    case ServiceErrorCode::TotalDataSizeExceeded: return E_PF_TOTAL_DATA_SIZE_EXCEEDED;
    case ServiceErrorCode::DeleteKeyConflict: return E_PF_DELETE_KEY_CONFLICT;
    case ServiceErrorCode::InvalidXboxLiveToken: return E_PF_INVALID_XBOX_LIVE_TOKEN;
    case ServiceErrorCode::ExpiredXboxLiveToken: return E_PF_EXPIRED_XBOX_LIVE_TOKEN;
    case ServiceErrorCode::ResettableStatisticVersionRequired: return E_PF_RESETTABLE_STATISTIC_VERSION_REQUIRED;
    case ServiceErrorCode::NotAuthorizedByTitle: return E_PF_NOT_AUTHORIZED_BY_TITLE;
    case ServiceErrorCode::NoPartnerEnabled: return E_PF_NO_PARTNER_ENABLED;
    case ServiceErrorCode::InvalidPartnerResponse: return E_PF_INVALID_PARTNER_RESPONSE;
    case ServiceErrorCode::APINotEnabledForGameServerAccess: return E_PF_API_NOT_ENABLED_FOR_GAME_SERVER_ACCESS;
    case ServiceErrorCode::StatisticNotFound: return E_PF_STATISTIC_NOT_FOUND;
    case ServiceErrorCode::StatisticNameConflict: return E_PF_STATISTIC_NAME_CONFLICT;
    case ServiceErrorCode::StatisticVersionClosedForWrites: return E_PF_STATISTIC_VERSION_CLOSED_FOR_WRITES;
    case ServiceErrorCode::StatisticVersionInvalid: return E_PF_STATISTIC_VERSION_INVALID;
    case ServiceErrorCode::APIClientRequestRateLimitExceeded: return E_PF_API_CLIENT_REQUEST_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::InvalidJSONContent: return E_PF_INVALID_JSON_CONTENT;
    case ServiceErrorCode::InvalidDropTable: return E_PF_INVALID_DROP_TABLE;
    case ServiceErrorCode::StatisticVersionAlreadyIncrementedForScheduledInterval: return E_PF_STATISTIC_VERSION_ALREADY_INCREMENTED_FOR_SCHEDULED_INTERVAL;
    case ServiceErrorCode::StatisticCountLimitExceeded: return E_PF_STATISTIC_COUNT_LIMIT_EXCEEDED;
    case ServiceErrorCode::StatisticVersionIncrementRateExceeded: return E_PF_STATISTIC_VERSION_INCREMENT_RATE_EXCEEDED;
    case ServiceErrorCode::ContainerKeyInvalid: return E_PF_CONTAINER_KEY_INVALID;
    case ServiceErrorCode::CloudScriptExecutionTimeLimitExceeded: return E_PF_CLOUD_SCRIPT_EXECUTION_TIME_LIMIT_EXCEEDED;
    case ServiceErrorCode::NoWritePermissionsForEvent: return E_PF_NO_WRITE_PERMISSIONS_FOR_EVENT;
    case ServiceErrorCode::CloudScriptFunctionArgumentSizeExceeded: return E_PF_CLOUD_SCRIPT_FUNCTION_ARGUMENT_SIZE_EXCEEDED;
    case ServiceErrorCode::CloudScriptAPIRequestCountExceeded: return E_PF_CLOUD_SCRIPT_API_REQUEST_COUNT_EXCEEDED;
    case ServiceErrorCode::CloudScriptAPIRequestError: return E_PF_CLOUD_SCRIPT_API_REQUEST_ERROR;
    case ServiceErrorCode::CloudScriptHTTPRequestError: return E_PF_CLOUD_SCRIPT_HTTP_REQUEST_ERROR;
    case ServiceErrorCode::InsufficientGuildRole: return E_PF_INSUFFICIENT_GUILD_ROLE;
    case ServiceErrorCode::GuildNotFound: return E_PF_GUILD_NOT_FOUND;
    case ServiceErrorCode::OverLimit: return E_PF_OVER_LIMIT;
    case ServiceErrorCode::EventNotFound: return E_PF_EVENT_NOT_FOUND;
    case ServiceErrorCode::InvalidEventField: return E_PF_INVALID_EVENT_FIELD;
    case ServiceErrorCode::InvalidEventName: return E_PF_INVALID_EVENT_NAME;
    case ServiceErrorCode::CatalogNotConfigured: return E_PF_CATALOG_NOT_CONFIGURED;
    case ServiceErrorCode::OperationNotSupportedForPlatform: return E_PF_OPERATION_NOT_SUPPORTED_FOR_PLATFORM;
    case ServiceErrorCode::SegmentNotFound: return E_PF_SEGMENT_NOT_FOUND;
    case ServiceErrorCode::StoreNotFound: return E_PF_STORE_NOT_FOUND;
    case ServiceErrorCode::InvalidStatisticName: return E_PF_INVALID_STATISTIC_NAME;
    case ServiceErrorCode::TitleNotQualifiedForLimit: return E_PF_TITLE_NOT_QUALIFIED_FOR_LIMIT;
    case ServiceErrorCode::InvalidServiceLimitLevel: return E_PF_INVALID_SERVICE_LIMIT_LEVEL;
    case ServiceErrorCode::ServiceLimitLevelInTransition: return E_PF_SERVICE_LIMIT_LEVEL_IN_TRANSITION;
    case ServiceErrorCode::CouponAlreadyRedeemed: return E_PF_COUPON_ALREADY_REDEEMED;
    case ServiceErrorCode::GameServerBuildSizeLimitExceeded: return E_PF_GAME_SERVER_BUILD_SIZE_LIMIT_EXCEEDED;
    case ServiceErrorCode::GameServerBuildCountLimitExceeded: return E_PF_GAME_SERVER_BUILD_COUNT_LIMIT_EXCEEDED;
    case ServiceErrorCode::VirtualCurrencyCountLimitExceeded: return E_PF_VIRTUAL_CURRENCY_COUNT_LIMIT_EXCEEDED;
    case ServiceErrorCode::VirtualCurrencyCodeExists: return E_PF_VIRTUAL_CURRENCY_CODE_EXISTS;
    case ServiceErrorCode::TitleNewsItemCountLimitExceeded: return E_PF_TITLE_NEWS_ITEM_COUNT_LIMIT_EXCEEDED;
    case ServiceErrorCode::InvalidTwitchToken: return E_PF_INVALID_TWITCH_TOKEN;
    case ServiceErrorCode::TwitchResponseError: return E_PF_TWITCH_RESPONSE_ERROR;
    case ServiceErrorCode::ProfaneDisplayName: return E_PF_PROFANE_DISPLAY_NAME;
    case ServiceErrorCode::UserAlreadyAdded: return E_PF_USER_ALREADY_ADDED;
    case ServiceErrorCode::InvalidVirtualCurrencyCode: return E_PF_INVALID_VIRTUAL_CURRENCY_CODE;
    case ServiceErrorCode::VirtualCurrencyCannotBeDeleted: return E_PF_VIRTUAL_CURRENCY_CANNOT_BE_DELETED;
    case ServiceErrorCode::IdentifierAlreadyClaimed: return E_PF_IDENTIFIER_ALREADY_CLAIMED;
    case ServiceErrorCode::IdentifierNotLinked: return E_PF_IDENTIFIER_NOT_LINKED;
    case ServiceErrorCode::InvalidContinuationToken: return E_PF_INVALID_CONTINUATION_TOKEN;
    case ServiceErrorCode::ExpiredContinuationToken: return E_PF_EXPIRED_CONTINUATION_TOKEN;
    case ServiceErrorCode::InvalidSegment: return E_PF_INVALID_SEGMENT;
    case ServiceErrorCode::InvalidSessionId: return E_PF_INVALID_SESSION_ID;
    case ServiceErrorCode::SessionLogNotFound: return E_PF_SESSION_LOG_NOT_FOUND;
    case ServiceErrorCode::InvalidSearchTerm: return E_PF_INVALID_SEARCH_TERM;
    case ServiceErrorCode::TwoFactorAuthenticationTokenRequired: return E_PF_TWO_FACTOR_AUTHENTICATION_TOKEN_REQUIRED;
    case ServiceErrorCode::GameServerHostCountLimitExceeded: return E_PF_GAME_SERVER_HOST_COUNT_LIMIT_EXCEEDED;
    case ServiceErrorCode::PlayerTagCountLimitExceeded: return E_PF_PLAYER_TAG_COUNT_LIMIT_EXCEEDED;
    case ServiceErrorCode::RequestAlreadyRunning: return E_PF_REQUEST_ALREADY_RUNNING;
    case ServiceErrorCode::ActionGroupNotFound: return E_PF_ACTION_GROUP_NOT_FOUND;
    case ServiceErrorCode::MaximumSegmentBulkActionJobsRunning: return E_PF_MAXIMUM_SEGMENT_BULK_ACTION_JOBS_RUNNING;
    case ServiceErrorCode::NoActionsOnPlayersInSegmentJob: return E_PF_NO_ACTIONS_ON_PLAYERS_IN_SEGMENT_JOB;
    case ServiceErrorCode::DuplicateStatisticName: return E_PF_DUPLICATE_STATISTIC_NAME;
    case ServiceErrorCode::ScheduledTaskNameConflict: return E_PF_SCHEDULED_TASK_NAME_CONFLICT;
    case ServiceErrorCode::ScheduledTaskCreateConflict: return E_PF_SCHEDULED_TASK_CREATE_CONFLICT;
    case ServiceErrorCode::InvalidScheduledTaskName: return E_PF_INVALID_SCHEDULED_TASK_NAME;
    case ServiceErrorCode::InvalidTaskSchedule: return E_PF_INVALID_TASK_SCHEDULE;
    case ServiceErrorCode::SteamNotEnabledForTitle: return E_PF_STEAM_NOT_ENABLED_FOR_TITLE;
    case ServiceErrorCode::LimitNotAnUpgradeOption: return E_PF_LIMIT_NOT_AN_UPGRADE_OPTION;
    case ServiceErrorCode::NoSecretKeyEnabledForCloudScript: return E_PF_NO_SECRET_KEY_ENABLED_FOR_CLOUD_SCRIPT;
    case ServiceErrorCode::TaskNotFound: return E_PF_TASK_NOT_FOUND;
    case ServiceErrorCode::TaskInstanceNotFound: return E_PF_TASK_INSTANCE_NOT_FOUND;
    case ServiceErrorCode::InvalidIdentityProviderId: return E_PF_INVALID_IDENTITY_PROVIDER_ID;
    case ServiceErrorCode::MisconfiguredIdentityProvider: return E_PF_MISCONFIGURED_IDENTITY_PROVIDER;
    case ServiceErrorCode::InvalidScheduledTaskType: return E_PF_INVALID_SCHEDULED_TASK_TYPE;
    case ServiceErrorCode::BillingInformationRequired: return E_PF_BILLING_INFORMATION_REQUIRED;
    case ServiceErrorCode::LimitedEditionItemUnavailable: return E_PF_LIMITED_EDITION_ITEM_UNAVAILABLE;
    case ServiceErrorCode::InvalidAdPlacementAndReward: return E_PF_INVALID_AD_PLACEMENT_AND_REWARD;
    case ServiceErrorCode::AllAdPlacementViewsAlreadyConsumed: return E_PF_ALL_AD_PLACEMENT_VIEWS_ALREADY_CONSUMED;
    case ServiceErrorCode::GoogleOAuthNotConfiguredForTitle: return E_PF_GOOGLE_O_AUTH_NOT_CONFIGURED_FOR_TITLE;
    case ServiceErrorCode::GoogleOAuthError: return E_PF_GOOGLE_O_AUTH_ERROR;
    case ServiceErrorCode::UserNotFriend: return E_PF_USER_NOT_FRIEND;
    case ServiceErrorCode::InvalidSignature: return E_PF_INVALID_SIGNATURE;
    case ServiceErrorCode::InvalidPublicKey: return E_PF_INVALID_PUBLIC_KEY;
    case ServiceErrorCode::GoogleOAuthNoIdTokenIncludedInResponse: return E_PF_GOOGLE_O_AUTH_NO_ID_TOKEN_INCLUDED_IN_RESPONSE;
    case ServiceErrorCode::StatisticUpdateInProgress: return E_PF_STATISTIC_UPDATE_IN_PROGRESS;
    case ServiceErrorCode::LeaderboardVersionNotAvailable: return E_PF_LEADERBOARD_VERSION_NOT_AVAILABLE;
    case ServiceErrorCode::StatisticAlreadyHasPrizeTable: return E_PF_STATISTIC_ALREADY_HAS_PRIZE_TABLE;
    case ServiceErrorCode::PrizeTableHasOverlappingRanks: return E_PF_PRIZE_TABLE_HAS_OVERLAPPING_RANKS;
    case ServiceErrorCode::PrizeTableHasMissingRanks: return E_PF_PRIZE_TABLE_HAS_MISSING_RANKS;
    case ServiceErrorCode::PrizeTableRankStartsAtZero: return E_PF_PRIZE_TABLE_RANK_STARTS_AT_ZERO;
    case ServiceErrorCode::InvalidStatistic: return E_PF_INVALID_STATISTIC;
    case ServiceErrorCode::ExpressionParseFailure: return E_PF_EXPRESSION_PARSE_FAILURE;
    case ServiceErrorCode::ExpressionInvokeFailure: return E_PF_EXPRESSION_INVOKE_FAILURE;
    case ServiceErrorCode::ExpressionTooLong: return E_PF_EXPRESSION_TOO_LONG;
    case ServiceErrorCode::DataUpdateRateExceeded: return E_PF_DATA_UPDATE_RATE_EXCEEDED;
    case ServiceErrorCode::RestrictedEmailDomain: return E_PF_RESTRICTED_EMAIL_DOMAIN;
    case ServiceErrorCode::EncryptionKeyDisabled: return E_PF_ENCRYPTION_KEY_DISABLED;
    case ServiceErrorCode::EncryptionKeyMissing: return E_PF_ENCRYPTION_KEY_MISSING;
    case ServiceErrorCode::EncryptionKeyBroken: return E_PF_ENCRYPTION_KEY_BROKEN;
    case ServiceErrorCode::NoSharedSecretKeyConfigured: return E_PF_NO_SHARED_SECRET_KEY_CONFIGURED;
    case ServiceErrorCode::SecretKeyNotFound: return E_PF_SECRET_KEY_NOT_FOUND;
    case ServiceErrorCode::PlayerSecretAlreadyConfigured: return E_PF_PLAYER_SECRET_ALREADY_CONFIGURED;
    case ServiceErrorCode::APIRequestsDisabledForTitle: return E_PF_API_REQUESTS_DISABLED_FOR_TITLE;
    case ServiceErrorCode::InvalidSharedSecretKey: return E_PF_INVALID_SHARED_SECRET_KEY;
    case ServiceErrorCode::PrizeTableHasNoRanks: return E_PF_PRIZE_TABLE_HAS_NO_RANKS;
    case ServiceErrorCode::ProfileDoesNotExist: return E_PF_PROFILE_DOES_NOT_EXIST;
    case ServiceErrorCode::ContentS3OriginBucketNotConfigured: return E_PF_CONTENT_S_3_ORIGIN_BUCKET_NOT_CONFIGURED;
    case ServiceErrorCode::InvalidEnvironmentForReceipt: return E_PF_INVALID_ENVIRONMENT_FOR_RECEIPT;
    case ServiceErrorCode::EncryptedRequestNotAllowed: return E_PF_ENCRYPTED_REQUEST_NOT_ALLOWED;
    case ServiceErrorCode::SignedRequestNotAllowed: return E_PF_SIGNED_REQUEST_NOT_ALLOWED;
    case ServiceErrorCode::RequestViewConstraintParamsNotAllowed: return E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED;
    case ServiceErrorCode::BadPartnerConfiguration: return E_PF_BAD_PARTNER_CONFIGURATION;
    case ServiceErrorCode::XboxBPCertificateFailure: return E_PF_XBOX_BP_CERTIFICATE_FAILURE;
    case ServiceErrorCode::XboxXASSExchangeFailure: return E_PF_XBOX_XASS_EXCHANGE_FAILURE;
    case ServiceErrorCode::InvalidEntityId: return E_PF_INVALID_ENTITY_ID;
    case ServiceErrorCode::StatisticValueAggregationOverflow: return E_PF_STATISTIC_VALUE_AGGREGATION_OVERFLOW;
    case ServiceErrorCode::EmailMessageFromAddressIsMissing: return E_PF_EMAIL_MESSAGE_FROM_ADDRESS_IS_MISSING;
    case ServiceErrorCode::EmailMessageToAddressIsMissing: return E_PF_EMAIL_MESSAGE_TO_ADDRESS_IS_MISSING;
    case ServiceErrorCode::SmtpServerAuthenticationError: return E_PF_SMTP_SERVER_AUTHENTICATION_ERROR;
    case ServiceErrorCode::SmtpServerLimitExceeded: return E_PF_SMTP_SERVER_LIMIT_EXCEEDED;
    case ServiceErrorCode::SmtpServerInsufficientStorage: return E_PF_SMTP_SERVER_INSUFFICIENT_STORAGE;
    case ServiceErrorCode::SmtpServerCommunicationError: return E_PF_SMTP_SERVER_COMMUNICATION_ERROR;
    case ServiceErrorCode::SmtpServerGeneralFailure: return E_PF_SMTP_SERVER_GENERAL_FAILURE;
    case ServiceErrorCode::EmailClientTimeout: return E_PF_EMAIL_CLIENT_TIMEOUT;
    case ServiceErrorCode::EmailClientCanceledTask: return E_PF_EMAIL_CLIENT_CANCELED_TASK;
    case ServiceErrorCode::EmailTemplateMissing: return E_PF_EMAIL_TEMPLATE_MISSING;
    case ServiceErrorCode::InvalidHostForTitleId: return E_PF_INVALID_HOST_FOR_TITLE_ID;
    case ServiceErrorCode::EmailConfirmationTokenDoesNotExist: return E_PF_EMAIL_CONFIRMATION_TOKEN_DOES_NOT_EXIST;
    case ServiceErrorCode::EmailConfirmationTokenExpired: return E_PF_EMAIL_CONFIRMATION_TOKEN_EXPIRED;
    case ServiceErrorCode::AccountDeleted: return E_PF_ACCOUNT_DELETED;
    case ServiceErrorCode::PlayerSecretNotConfigured: return E_PF_PLAYER_SECRET_NOT_CONFIGURED;
    case ServiceErrorCode::InvalidSignatureTime: return E_PF_INVALID_SIGNATURE_TIME;
    case ServiceErrorCode::NoContactEmailAddressFound: return E_PF_NO_CONTACT_EMAIL_ADDRESS_FOUND;
    case ServiceErrorCode::InvalidAuthToken: return E_PF_INVALID_AUTH_TOKEN;
    case ServiceErrorCode::AuthTokenDoesNotExist: return E_PF_AUTH_TOKEN_DOES_NOT_EXIST;
    case ServiceErrorCode::AuthTokenExpired: return E_PF_AUTH_TOKEN_EXPIRED;
    case ServiceErrorCode::AuthTokenAlreadyUsedToResetPassword: return E_PF_AUTH_TOKEN_ALREADY_USED_TO_RESET_PASSWORD;
    case ServiceErrorCode::MembershipNameTooLong: return E_PF_MEMBERSHIP_NAME_TOO_LONG;
    case ServiceErrorCode::MembershipNotFound: return E_PF_MEMBERSHIP_NOT_FOUND;
    case ServiceErrorCode::GoogleServiceAccountInvalid: return E_PF_GOOGLE_SERVICE_ACCOUNT_INVALID;
    case ServiceErrorCode::GoogleServiceAccountParseFailure: return E_PF_GOOGLE_SERVICE_ACCOUNT_PARSE_FAILURE;
    case ServiceErrorCode::EntityTokenMissing: return E_PF_ENTITY_TOKEN_MISSING;
    case ServiceErrorCode::EntityTokenInvalid: return E_PF_ENTITY_TOKEN_INVALID;
    case ServiceErrorCode::EntityTokenExpired: return E_PF_ENTITY_TOKEN_EXPIRED;
    case ServiceErrorCode::EntityTokenRevoked: return E_PF_ENTITY_TOKEN_REVOKED;
    case ServiceErrorCode::InvalidProductForSubscription: return E_PF_INVALID_PRODUCT_FOR_SUBSCRIPTION;
    case ServiceErrorCode::XboxInaccessible: return E_PF_XBOX_INACCESSIBLE;
    case ServiceErrorCode::SubscriptionAlreadyTaken: return E_PF_SUBSCRIPTION_ALREADY_TAKEN;
    case ServiceErrorCode::SmtpAddonNotEnabled: return E_PF_SMTP_ADDON_NOT_ENABLED;
    case ServiceErrorCode::APIConcurrentRequestLimitExceeded: return E_PF_API_CONCURRENT_REQUEST_LIMIT_EXCEEDED;
    case ServiceErrorCode::XboxRejectedXSTSExchangeRequest: return E_PF_XBOX_REJECTED_XSTS_EXCHANGE_REQUEST;
    case ServiceErrorCode::VariableNotDefined: return E_PF_VARIABLE_NOT_DEFINED;
    case ServiceErrorCode::TemplateVersionNotDefined: return E_PF_TEMPLATE_VERSION_NOT_DEFINED;
    case ServiceErrorCode::FileTooLarge: return E_PF_FILE_TOO_LARGE;
    case ServiceErrorCode::TitleDeleted: return E_PF_TITLE_DELETED;
    case ServiceErrorCode::TitleContainsUserAccounts: return E_PF_TITLE_CONTAINS_USER_ACCOUNTS;
    case ServiceErrorCode::TitleDeletionPlayerCleanupFailure: return E_PF_TITLE_DELETION_PLAYER_CLEANUP_FAILURE;
    case ServiceErrorCode::EntityFileOperationPending: return E_PF_ENTITY_FILE_OPERATION_PENDING;
    case ServiceErrorCode::NoEntityFileOperationPending: return E_PF_NO_ENTITY_FILE_OPERATION_PENDING;
    case ServiceErrorCode::EntityProfileVersionMismatch: return E_PF_ENTITY_PROFILE_VERSION_MISMATCH;
    case ServiceErrorCode::TemplateVersionTooOld: return E_PF_TEMPLATE_VERSION_TOO_OLD;
    case ServiceErrorCode::MembershipDefinitionInUse: return E_PF_MEMBERSHIP_DEFINITION_IN_USE;
    case ServiceErrorCode::PaymentPageNotConfigured: return E_PF_PAYMENT_PAGE_NOT_CONFIGURED;
    case ServiceErrorCode::FailedLoginAttemptRateLimitExceeded: return E_PF_FAILED_LOGIN_ATTEMPT_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::EntityBlockedByGroup: return E_PF_ENTITY_BLOCKED_BY_GROUP;
    case ServiceErrorCode::RoleDoesNotExist: return E_PF_ROLE_DOES_NOT_EXIST;
    case ServiceErrorCode::EntityIsAlreadyMember: return E_PF_ENTITY_IS_ALREADY_MEMBER;
    case ServiceErrorCode::DuplicateRoleId: return E_PF_DUPLICATE_ROLE_ID;
    case ServiceErrorCode::GroupInvitationNotFound: return E_PF_GROUP_INVITATION_NOT_FOUND;
    case ServiceErrorCode::GroupApplicationNotFound: return E_PF_GROUP_APPLICATION_NOT_FOUND;
    case ServiceErrorCode::OutstandingInvitationAcceptedInstead: return E_PF_OUTSTANDING_INVITATION_ACCEPTED_INSTEAD;
    case ServiceErrorCode::OutstandingApplicationAcceptedInstead: return E_PF_OUTSTANDING_APPLICATION_ACCEPTED_INSTEAD;
    case ServiceErrorCode::RoleIsGroupDefaultMember: return E_PF_ROLE_IS_GROUP_DEFAULT_MEMBER;
    case ServiceErrorCode::RoleIsGroupAdmin: return E_PF_ROLE_IS_GROUP_ADMIN;
    case ServiceErrorCode::RoleNameNotAvailable: return E_PF_ROLE_NAME_NOT_AVAILABLE;
    case ServiceErrorCode::GroupNameNotAvailable: return E_PF_GROUP_NAME_NOT_AVAILABLE;
    case ServiceErrorCode::EmailReportAlreadySent: return E_PF_EMAIL_REPORT_ALREADY_SENT;
    case ServiceErrorCode::EmailReportRecipientBlacklisted: return E_PF_EMAIL_REPORT_RECIPIENT_BLACKLISTED;
    case ServiceErrorCode::EventNamespaceNotAllowed: return E_PF_EVENT_NAMESPACE_NOT_ALLOWED;
    case ServiceErrorCode::EventEntityNotAllowed: return E_PF_EVENT_ENTITY_NOT_ALLOWED;
    case ServiceErrorCode::InvalidEntityType: return E_PF_INVALID_ENTITY_TYPE;
    case ServiceErrorCode::NullTokenResultFromAad: return E_PF_NULL_TOKEN_RESULT_FROM_AAD;
    case ServiceErrorCode::InvalidTokenResultFromAad: return E_PF_INVALID_TOKEN_RESULT_FROM_AAD;
    case ServiceErrorCode::NoValidCertificateForAad: return E_PF_NO_VALID_CERTIFICATE_FOR_AAD;
    case ServiceErrorCode::InvalidCertificateForAad: return E_PF_INVALID_CERTIFICATE_FOR_AAD;
    case ServiceErrorCode::DuplicateDropTableId: return E_PF_DUPLICATE_DROP_TABLE_ID;
    case ServiceErrorCode::MultiplayerServerError: return E_PF_MULTIPLAYER_SERVER_ERROR;
    case ServiceErrorCode::MultiplayerServerTooManyRequests: return E_PF_MULTIPLAYER_SERVER_TOO_MANY_REQUESTS;
    case ServiceErrorCode::MultiplayerServerNoContent: return E_PF_MULTIPLAYER_SERVER_NO_CONTENT;
    case ServiceErrorCode::MultiplayerServerBadRequest: return E_PF_MULTIPLAYER_SERVER_BAD_REQUEST;
    case ServiceErrorCode::MultiplayerServerUnauthorized: return E_PF_MULTIPLAYER_SERVER_UNAUTHORIZED;
    case ServiceErrorCode::MultiplayerServerForbidden: return E_PF_MULTIPLAYER_SERVER_FORBIDDEN;
    case ServiceErrorCode::MultiplayerServerNotFound: return E_PF_MULTIPLAYER_SERVER_NOT_FOUND;
    case ServiceErrorCode::MultiplayerServerConflict: return E_PF_MULTIPLAYER_SERVER_CONFLICT;
    case ServiceErrorCode::MultiplayerServerInternalServerError: return E_PF_MULTIPLAYER_SERVER_INTERNAL_SERVER_ERROR;
    case ServiceErrorCode::MultiplayerServerUnavailable: return E_PF_MULTIPLAYER_SERVER_UNAVAILABLE;
    case ServiceErrorCode::ExplicitContentDetected: return E_PF_EXPLICIT_CONTENT_DETECTED;
    case ServiceErrorCode::PIIContentDetected: return E_PF_PII_CONTENT_DETECTED;
    case ServiceErrorCode::InvalidScheduledTaskParameter: return E_PF_INVALID_SCHEDULED_TASK_PARAMETER;
    case ServiceErrorCode::PerEntityEventRateLimitExceeded: return E_PF_PER_ENTITY_EVENT_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::TitleDefaultLanguageNotSet: return E_PF_TITLE_DEFAULT_LANGUAGE_NOT_SET;
    case ServiceErrorCode::EmailTemplateMissingDefaultVersion: return E_PF_EMAIL_TEMPLATE_MISSING_DEFAULT_VERSION;
    case ServiceErrorCode::FacebookInstantGamesIdNotLinked: return E_PF_FACEBOOK_INSTANT_GAMES_ID_NOT_LINKED;
    case ServiceErrorCode::InvalidFacebookInstantGamesSignature: return E_PF_INVALID_FACEBOOK_INSTANT_GAMES_SIGNATURE;
    case ServiceErrorCode::FacebookInstantGamesAuthNotConfiguredForTitle: return E_PF_FACEBOOK_INSTANT_GAMES_AUTH_NOT_CONFIGURED_FOR_TITLE;
    case ServiceErrorCode::EntityProfileConstraintValidationFailed: return E_PF_ENTITY_PROFILE_CONSTRAINT_VALIDATION_FAILED;
    case ServiceErrorCode::TelemetryIngestionKeyPending: return E_PF_TELEMETRY_INGESTION_KEY_PENDING;
    case ServiceErrorCode::TelemetryIngestionKeyNotFound: return E_PF_TELEMETRY_INGESTION_KEY_NOT_FOUND;
    case ServiceErrorCode::StatisticChildNameInvalid: return E_PF_STATISTIC_CHILD_NAME_INVALID;
    case ServiceErrorCode::DataIntegrityError: return E_PF_DATA_INTEGRITY_ERROR;
    case ServiceErrorCode::VirtualCurrencyCannotBeSetToOlderVersion: return E_PF_VIRTUAL_CURRENCY_CANNOT_BE_SET_TO_OLDER_VERSION;
    case ServiceErrorCode::VirtualCurrencyMustBeWithinIntegerRange: return E_PF_VIRTUAL_CURRENCY_MUST_BE_WITHIN_INTEGER_RANGE;
    case ServiceErrorCode::EmailTemplateInvalidSyntax: return E_PF_EMAIL_TEMPLATE_INVALID_SYNTAX;
    case ServiceErrorCode::EmailTemplateMissingCallback: return E_PF_EMAIL_TEMPLATE_MISSING_CALLBACK;
    case ServiceErrorCode::PushNotificationTemplateInvalidPayload: return E_PF_PUSH_NOTIFICATION_TEMPLATE_INVALID_PAYLOAD;
    case ServiceErrorCode::InvalidLocalizedPushNotificationLanguage: return E_PF_INVALID_LOCALIZED_PUSH_NOTIFICATION_LANGUAGE;
    case ServiceErrorCode::MissingLocalizedPushNotificationMessage: return E_PF_MISSING_LOCALIZED_PUSH_NOTIFICATION_MESSAGE;
    case ServiceErrorCode::PushNotificationTemplateMissingPlatformPayload: return E_PF_PUSH_NOTIFICATION_TEMPLATE_MISSING_PLATFORM_PAYLOAD;
    case ServiceErrorCode::PushNotificationTemplatePayloadContainsInvalidJson: return E_PF_PUSH_NOTIFICATION_TEMPLATE_PAYLOAD_CONTAINS_INVALID_JSON;
    case ServiceErrorCode::PushNotificationTemplateContainsInvalidIosPayload: return E_PF_PUSH_NOTIFICATION_TEMPLATE_CONTAINS_INVALID_IOS_PAYLOAD;
    case ServiceErrorCode::PushNotificationTemplateContainsInvalidAndroidPayload: return E_PF_PUSH_NOTIFICATION_TEMPLATE_CONTAINS_INVALID_ANDROID_PAYLOAD;
    case ServiceErrorCode::PushNotificationTemplateIosPayloadMissingNotificationBody: return E_PF_PUSH_NOTIFICATION_TEMPLATE_IOS_PAYLOAD_MISSING_NOTIFICATION_BODY;
    case ServiceErrorCode::PushNotificationTemplateAndroidPayloadMissingNotificationBody: return E_PF_PUSH_NOTIFICATION_TEMPLATE_ANDROID_PAYLOAD_MISSING_NOTIFICATION_BODY;
    case ServiceErrorCode::PushNotificationTemplateNotFound: return E_PF_PUSH_NOTIFICATION_TEMPLATE_NOT_FOUND;
    case ServiceErrorCode::PushNotificationTemplateMissingDefaultVersion: return E_PF_PUSH_NOTIFICATION_TEMPLATE_MISSING_DEFAULT_VERSION;
    case ServiceErrorCode::PushNotificationTemplateInvalidSyntax: return E_PF_PUSH_NOTIFICATION_TEMPLATE_INVALID_SYNTAX;
    case ServiceErrorCode::PushNotificationTemplateNoCustomPayloadForV1: return E_PF_PUSH_NOTIFICATION_TEMPLATE_NO_CUSTOM_PAYLOAD_FOR_V_1;
    case ServiceErrorCode::NoLeaderboardForStatistic: return E_PF_NO_LEADERBOARD_FOR_STATISTIC;
    case ServiceErrorCode::TitleNewsMissingDefaultLanguage: return E_PF_TITLE_NEWS_MISSING_DEFAULT_LANGUAGE;
    case ServiceErrorCode::TitleNewsNotFound: return E_PF_TITLE_NEWS_NOT_FOUND;
    case ServiceErrorCode::TitleNewsDuplicateLanguage: return E_PF_TITLE_NEWS_DUPLICATE_LANGUAGE;
    case ServiceErrorCode::TitleNewsMissingTitleOrBody: return E_PF_TITLE_NEWS_MISSING_TITLE_OR_BODY;
    case ServiceErrorCode::TitleNewsInvalidLanguage: return E_PF_TITLE_NEWS_INVALID_LANGUAGE;
    case ServiceErrorCode::EmailRecipientBlacklisted: return E_PF_EMAIL_RECIPIENT_BLACKLISTED;
    case ServiceErrorCode::InvalidGameCenterAuthRequest: return E_PF_INVALID_GAME_CENTER_AUTH_REQUEST;
    case ServiceErrorCode::GameCenterAuthenticationFailed: return E_PF_GAME_CENTER_AUTHENTICATION_FAILED;
    case ServiceErrorCode::CannotEnablePartiesForTitle: return E_PF_CANNOT_ENABLE_PARTIES_FOR_TITLE;
    case ServiceErrorCode::PartyError: return E_PF_PARTY_ERROR;
    case ServiceErrorCode::PartyRequests: return E_PF_PARTY_REQUESTS;
    case ServiceErrorCode::PartyNoContent: return E_PF_PARTY_NO_CONTENT;
    case ServiceErrorCode::PartyBadRequest: return E_PF_PARTY_BAD_REQUEST;
    case ServiceErrorCode::PartyUnauthorized: return E_PF_PARTY_UNAUTHORIZED;
    case ServiceErrorCode::PartyForbidden: return E_PF_PARTY_FORBIDDEN;
    case ServiceErrorCode::PartyNotFound: return E_PF_PARTY_NOT_FOUND;
    case ServiceErrorCode::PartyConflict: return E_PF_PARTY_CONFLICT;
    case ServiceErrorCode::PartyInternalServerError: return E_PF_PARTY_INTERNAL_SERVER_ERROR;
    case ServiceErrorCode::PartyUnavailable: return E_PF_PARTY_UNAVAILABLE;
    case ServiceErrorCode::PartyTooManyRequests: return E_PF_PARTY_TOO_MANY_REQUESTS;
    case ServiceErrorCode::PushNotificationTemplateMissingName: return E_PF_PUSH_NOTIFICATION_TEMPLATE_MISSING_NAME;
    case ServiceErrorCode::CannotEnableMultiplayerServersForTitle: return E_PF_CANNOT_ENABLE_MULTIPLAYER_SERVERS_FOR_TITLE;
    case ServiceErrorCode::WriteAttemptedDuringExport: return E_PF_WRITE_ATTEMPTED_DURING_EXPORT;
    case ServiceErrorCode::MultiplayerServerTitleQuotaCoresExceeded: return E_PF_MULTIPLAYER_SERVER_TITLE_QUOTA_CORES_EXCEEDED;
    case ServiceErrorCode::AutomationRuleNotFound: return E_PF_AUTOMATION_RULE_NOT_FOUND;
    case ServiceErrorCode::EntityAPIKeyLimitExceeded: return E_PF_ENTITY_API_KEY_LIMIT_EXCEEDED;
    case ServiceErrorCode::EntityAPIKeyNotFound: return E_PF_ENTITY_API_KEY_NOT_FOUND;
    case ServiceErrorCode::EntityAPIKeyOrSecretInvalid: return E_PF_ENTITY_API_KEY_OR_SECRET_INVALID;
    case ServiceErrorCode::EconomyServiceUnavailable: return E_PF_ECONOMY_SERVICE_UNAVAILABLE;
    case ServiceErrorCode::EconomyServiceInternalError: return E_PF_ECONOMY_SERVICE_INTERNAL_ERROR;
    case ServiceErrorCode::QueryRateLimitExceeded: return E_PF_QUERY_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::EntityAPIKeyCreationDisabledForEntity: return E_PF_ENTITY_API_KEY_CREATION_DISABLED_FOR_ENTITY;
    case ServiceErrorCode::ForbiddenByEntityPolicy: return E_PF_FORBIDDEN_BY_ENTITY_POLICY;
    case ServiceErrorCode::UpdateInventoryRateLimitExceeded: return E_PF_UPDATE_INVENTORY_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::StudioCreationRateLimited: return E_PF_STUDIO_CREATION_RATE_LIMITED;
    case ServiceErrorCode::StudioCreationInProgress: return E_PF_STUDIO_CREATION_IN_PROGRESS;
    case ServiceErrorCode::DuplicateStudioName: return E_PF_DUPLICATE_STUDIO_NAME;
    case ServiceErrorCode::StudioNotFound: return E_PF_STUDIO_NOT_FOUND;
    case ServiceErrorCode::StudioDeleted: return E_PF_STUDIO_DELETED;
    case ServiceErrorCode::StudioDeactivated: return E_PF_STUDIO_DEACTIVATED;
    case ServiceErrorCode::StudioActivated: return E_PF_STUDIO_ACTIVATED;
    case ServiceErrorCode::TitleCreationRateLimited: return E_PF_TITLE_CREATION_RATE_LIMITED;
    case ServiceErrorCode::TitleCreationInProgress: return E_PF_TITLE_CREATION_IN_PROGRESS;
    case ServiceErrorCode::DuplicateTitleName: return E_PF_DUPLICATE_TITLE_NAME;
    case ServiceErrorCode::TitleActivationRateLimited: return E_PF_TITLE_ACTIVATION_RATE_LIMITED;
    case ServiceErrorCode::TitleActivationInProgress: return E_PF_TITLE_ACTIVATION_IN_PROGRESS;
    case ServiceErrorCode::TitleDeactivated: return E_PF_TITLE_DEACTIVATED;
    case ServiceErrorCode::TitleActivated: return E_PF_TITLE_ACTIVATED;
    case ServiceErrorCode::CloudScriptAzureFunctionsExecutionTimeLimitExceeded: return E_PF_CLOUD_SCRIPT_AZURE_FUNCTIONS_EXECUTION_TIME_LIMIT_EXCEEDED;
    case ServiceErrorCode::CloudScriptAzureFunctionsArgumentSizeExceeded: return E_PF_CLOUD_SCRIPT_AZURE_FUNCTIONS_ARGUMENT_SIZE_EXCEEDED;
    case ServiceErrorCode::CloudScriptAzureFunctionsReturnSizeExceeded: return E_PF_CLOUD_SCRIPT_AZURE_FUNCTIONS_RETURN_SIZE_EXCEEDED;
    case ServiceErrorCode::CloudScriptAzureFunctionsHTTPRequestError: return E_PF_CLOUD_SCRIPT_AZURE_FUNCTIONS_HTTP_REQUEST_ERROR;
    case ServiceErrorCode::VirtualCurrencyBetaGetError: return E_PF_VIRTUAL_CURRENCY_BETA_GET_ERROR;
    case ServiceErrorCode::VirtualCurrencyBetaCreateError: return E_PF_VIRTUAL_CURRENCY_BETA_CREATE_ERROR;
    case ServiceErrorCode::VirtualCurrencyBetaInitialDepositSaveError: return E_PF_VIRTUAL_CURRENCY_BETA_INITIAL_DEPOSIT_SAVE_ERROR;
    case ServiceErrorCode::VirtualCurrencyBetaSaveError: return E_PF_VIRTUAL_CURRENCY_BETA_SAVE_ERROR;
    case ServiceErrorCode::VirtualCurrencyBetaDeleteError: return E_PF_VIRTUAL_CURRENCY_BETA_DELETE_ERROR;
    case ServiceErrorCode::VirtualCurrencyBetaRestoreError: return E_PF_VIRTUAL_CURRENCY_BETA_RESTORE_ERROR;
    case ServiceErrorCode::VirtualCurrencyBetaSaveConflict: return E_PF_VIRTUAL_CURRENCY_BETA_SAVE_CONFLICT;
    case ServiceErrorCode::VirtualCurrencyBetaUpdateError: return E_PF_VIRTUAL_CURRENCY_BETA_UPDATE_ERROR;
    case ServiceErrorCode::InsightsManagementDatabaseNotFound: return E_PF_INSIGHTS_MANAGEMENT_DATABASE_NOT_FOUND;
    case ServiceErrorCode::InsightsManagementOperationNotFound: return E_PF_INSIGHTS_MANAGEMENT_OPERATION_NOT_FOUND;
    case ServiceErrorCode::InsightsManagementErrorPendingOperationExists: return E_PF_INSIGHTS_MANAGEMENT_ERROR_PENDING_OPERATION_EXISTS;
    case ServiceErrorCode::InsightsManagementSetPerformanceLevelInvalidParameter: return E_PF_INSIGHTS_MANAGEMENT_SET_PERFORMANCE_LEVEL_INVALID_PARAMETER;
    case ServiceErrorCode::InsightsManagementSetStorageRetentionInvalidParameter: return E_PF_INSIGHTS_MANAGEMENT_SET_STORAGE_RETENTION_INVALID_PARAMETER;
    case ServiceErrorCode::InsightsManagementGetStorageUsageInvalidParameter: return E_PF_INSIGHTS_MANAGEMENT_GET_STORAGE_USAGE_INVALID_PARAMETER;
    case ServiceErrorCode::InsightsManagementGetOperationStatusInvalidParameter: return E_PF_INSIGHTS_MANAGEMENT_GET_OPERATION_STATUS_INVALID_PARAMETER;
    case ServiceErrorCode::DuplicatePurchaseTransactionId: return E_PF_DUPLICATE_PURCHASE_TRANSACTION_ID;
    case ServiceErrorCode::EvaluationModePlayerCountExceeded: return E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED;
    case ServiceErrorCode::GetPlayersInSegmentRateLimitExceeded: return E_PF_GET_PLAYERS_IN_SEGMENT_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::CloudScriptFunctionNameSizeExceeded: return E_PF_CLOUD_SCRIPT_FUNCTION_NAME_SIZE_EXCEEDED;
    case ServiceErrorCode::PaidInsightsFeaturesNotEnabled: return E_PF_PAID_INSIGHTS_FEATURES_NOT_ENABLED;
    case ServiceErrorCode::CloudScriptAzureFunctionsQueueRequestError: return E_PF_CLOUD_SCRIPT_AZURE_FUNCTIONS_QUEUE_REQUEST_ERROR;
    case ServiceErrorCode::EvaluationModeTitleCountExceeded: return E_PF_EVALUATION_MODE_TITLE_COUNT_EXCEEDED;
    case ServiceErrorCode::InsightsManagementTitleNotInFlight: return E_PF_INSIGHTS_MANAGEMENT_TITLE_NOT_IN_FLIGHT;
    case ServiceErrorCode::LimitNotFound: return E_PF_LIMIT_NOT_FOUND;
    case ServiceErrorCode::LimitNotAvailableViaAPI: return E_PF_LIMIT_NOT_AVAILABLE_VIA_API;
    case ServiceErrorCode::InsightsManagementSetStorageRetentionBelowMinimum: return E_PF_INSIGHTS_MANAGEMENT_SET_STORAGE_RETENTION_BELOW_MINIMUM;
    case ServiceErrorCode::InsightsManagementSetStorageRetentionAboveMaximum: return E_PF_INSIGHTS_MANAGEMENT_SET_STORAGE_RETENTION_ABOVE_MAXIMUM;
    case ServiceErrorCode::AppleNotEnabledForTitle: return E_PF_APPLE_NOT_ENABLED_FOR_TITLE;
    case ServiceErrorCode::InsightsManagementNewActiveEventExportLimitInvalid: return E_PF_INSIGHTS_MANAGEMENT_NEW_ACTIVE_EVENT_EXPORT_LIMIT_INVALID;
    case ServiceErrorCode::InsightsManagementSetPerformanceRateLimited: return E_PF_INSIGHTS_MANAGEMENT_SET_PERFORMANCE_RATE_LIMITED;
    case ServiceErrorCode::PartyRequestsThrottledFromRateLimiter: return E_PF_PARTY_REQUESTS_THROTTLED_FROM_RATE_LIMITER;
    case ServiceErrorCode::XboxServiceTooManyRequests: return E_PF_XBOX_SERVICE_TOO_MANY_REQUESTS;
    case ServiceErrorCode::NintendoSwitchNotEnabledForTitle: return E_PF_NINTENDO_SWITCH_NOT_ENABLED_FOR_TITLE;
    case ServiceErrorCode::RequestMultiplayerServersThrottledFromRateLimiter: return E_PF_REQUEST_MULTIPLAYER_SERVERS_THROTTLED_FROM_RATE_LIMITER;
    case ServiceErrorCode::TitleDataOverrideNotFound: return E_PF_TITLE_DATA_OVERRIDE_NOT_FOUND;
    case ServiceErrorCode::DuplicateKeys: return E_PF_DUPLICATE_KEYS;
    case ServiceErrorCode::WasNotCreatedWithCloudRoot: return E_PF_WAS_NOT_CREATED_WITH_CLOUD_ROOT;
    case ServiceErrorCode::LegacyMultiplayerServersDeprecated: return E_PF_LEGACY_MULTIPLAYER_SERVERS_DEPRECATED;
    case ServiceErrorCode::VirtualCurrencyCurrentlyUnavailable: return E_PF_VIRTUAL_CURRENCY_CURRENTLY_UNAVAILABLE;
    case ServiceErrorCode::SteamUserNotFound: return E_PF_STEAM_USER_NOT_FOUND;
    case ServiceErrorCode::ElasticSearchOperationFailed: return E_PF_ELASTIC_SEARCH_OPERATION_FAILED;
    case ServiceErrorCode::NotImplemented: return E_PF_NOT_IMPLEMENTED;
    case ServiceErrorCode::PublisherNotFound: return E_PF_PUBLISHER_NOT_FOUND;
    case ServiceErrorCode::PublisherDeleted: return E_PF_PUBLISHER_DELETED;
    case ServiceErrorCode::ApiDisabledForMigration: return E_PF_API_DISABLED_FOR_MIGRATION;
    case ServiceErrorCode::ResourceNameUpdateNotAllowed: return E_PF_RESOURCE_NAME_UPDATE_NOT_ALLOWED;
    case ServiceErrorCode::ApiNotEnabledForTitle: return E_PF_API_NOT_ENABLED_FOR_TITLE;
    case ServiceErrorCode::DuplicateTitleNameForPublisher: return E_PF_DUPLICATE_TITLE_NAME_FOR_PUBLISHER;
    case ServiceErrorCode::AzureTitleCreationInProgress: return E_PF_AZURE_TITLE_CREATION_IN_PROGRESS;
    case ServiceErrorCode::TitleConstraintsPublisherDeletion: return E_PF_TITLE_CONSTRAINTS_PUBLISHER_DELETION;
    case ServiceErrorCode::InvalidPlayerAccountPoolId: return E_PF_INVALID_PLAYER_ACCOUNT_POOL_ID;
    case ServiceErrorCode::PlayerAccountPoolNotFound: return E_PF_PLAYER_ACCOUNT_POOL_NOT_FOUND;
    case ServiceErrorCode::PlayerAccountPoolDeleted: return E_PF_PLAYER_ACCOUNT_POOL_DELETED;
    case ServiceErrorCode::TitleCleanupInProgress: return E_PF_TITLE_CLEANUP_IN_PROGRESS;
    case ServiceErrorCode::AzureResourceConcurrentOperationInProgress: return E_PF_AZURE_RESOURCE_CONCURRENT_OPERATION_IN_PROGRESS;
    case ServiceErrorCode::TitlePublisherUpdateNotAllowed: return E_PF_TITLE_PUBLISHER_UPDATE_NOT_ALLOWED;
    case ServiceErrorCode::AzureResourceManagerNotSupportedInStamp: return E_PF_AZURE_RESOURCE_MANAGER_NOT_SUPPORTED_IN_STAMP;
    case ServiceErrorCode::ApiNotIncludedInAzurePlayFabFeatureSet: return E_PF_API_NOT_INCLUDED_IN_AZURE_PLAY_FAB_FEATURE_SET;
    case ServiceErrorCode::GoogleServiceAccountFailedAuth: return E_PF_GOOGLE_SERVICE_ACCOUNT_FAILED_AUTH;
    case ServiceErrorCode::GoogleAPIServiceUnavailable: return E_PF_GOOGLE_API_SERVICE_UNAVAILABLE;
    case ServiceErrorCode::GoogleAPIServiceUnknownError: return E_PF_GOOGLE_API_SERVICE_UNKNOWN_ERROR;
    case ServiceErrorCode::NoValidIdentityForAad: return E_PF_NO_VALID_IDENTITY_FOR_AAD;
    case ServiceErrorCode::PlayerIdentityLinkNotFound: return E_PF_PLAYER_IDENTITY_LINK_NOT_FOUND;
    case ServiceErrorCode::PhotonApplicationIdAlreadyInUse: return E_PF_PHOTON_APPLICATION_ID_ALREADY_IN_USE;
    case ServiceErrorCode::CloudScriptUnableToDeleteProductionRevision: return E_PF_CLOUD_SCRIPT_UNABLE_TO_DELETE_PRODUCTION_REVISION;
    case ServiceErrorCode::CustomIdNotFound: return E_PF_CUSTOM_ID_NOT_FOUND;
    case ServiceErrorCode::AutomationInvalidInput: return E_PF_AUTOMATION_INVALID_INPUT;
    case ServiceErrorCode::AutomationInvalidRuleName: return E_PF_AUTOMATION_INVALID_RULE_NAME;
    case ServiceErrorCode::AutomationRuleAlreadyExists: return E_PF_AUTOMATION_RULE_ALREADY_EXISTS;
    case ServiceErrorCode::AutomationRuleLimitExceeded: return E_PF_AUTOMATION_RULE_LIMIT_EXCEEDED;
    case ServiceErrorCode::InvalidGooglePlayGamesServerAuthCode: return E_PF_INVALID_GOOGLE_PLAY_GAMES_SERVER_AUTH_CODE;
    case ServiceErrorCode::PlayStreamConnectionFailed: return E_PF_PLAY_STREAM_CONNECTION_FAILED;
    case ServiceErrorCode::InvalidEventContents: return E_PF_INVALID_EVENT_CONTENTS;
    case ServiceErrorCode::InsightsV1Deprecated: return E_PF_INSIGHTS_V_1_DEPRECATED;
    case ServiceErrorCode::AnalysisSubscriptionNotFound: return E_PF_ANALYSIS_SUBSCRIPTION_NOT_FOUND;
    case ServiceErrorCode::AnalysisSubscriptionFailed: return E_PF_ANALYSIS_SUBSCRIPTION_FAILED;
    case ServiceErrorCode::AnalysisSubscriptionFoundAlready: return E_PF_ANALYSIS_SUBSCRIPTION_FOUND_ALREADY;
    case ServiceErrorCode::AnalysisSubscriptionManagementInvalidInput: return E_PF_ANALYSIS_SUBSCRIPTION_MANAGEMENT_INVALID_INPUT;
    case ServiceErrorCode::InvalidGameCenterId: return E_PF_INVALID_GAME_CENTER_ID;
    case ServiceErrorCode::InvalidNintendoSwitchAccountId: return E_PF_INVALID_NINTENDO_SWITCH_ACCOUNT_ID;
    case ServiceErrorCode::EntityAPIKeysNotSupported: return E_PF_ENTITY_API_KEYS_NOT_SUPPORTED;
    case ServiceErrorCode::IpAddressBanned: return E_PF_IP_ADDRESS_BANNED;
    case ServiceErrorCode::EntityLineageBanned: return E_PF_ENTITY_LINEAGE_BANNED;
    case ServiceErrorCode::NamespaceMismatch: return E_PF_NAMESPACE_MISMATCH;
    case ServiceErrorCode::InvalidServiceConfiguration: return E_PF_INVALID_SERVICE_CONFIGURATION;
    case ServiceErrorCode::InvalidNamespaceMismatch: return E_PF_INVALID_NAMESPACE_MISMATCH;
    case ServiceErrorCode::LeaderboardColumnLengthMismatch: return E_PF_LEADERBOARD_COLUMN_LENGTH_MISMATCH;
    case ServiceErrorCode::MatchmakingEntityInvalid: return E_PF_MATCHMAKING_ENTITY_INVALID;
    case ServiceErrorCode::MatchmakingPlayerAttributesInvalid: return E_PF_MATCHMAKING_PLAYER_ATTRIBUTES_INVALID;
    case ServiceErrorCode::MatchmakingQueueNotFound: return E_PF_MATCHMAKING_QUEUE_NOT_FOUND;
    case ServiceErrorCode::MatchmakingMatchNotFound: return E_PF_MATCHMAKING_MATCH_NOT_FOUND;
    case ServiceErrorCode::MatchmakingTicketNotFound: return E_PF_MATCHMAKING_TICKET_NOT_FOUND;
    case ServiceErrorCode::MatchmakingAlreadyJoinedTicket: return E_PF_MATCHMAKING_ALREADY_JOINED_TICKET;
    case ServiceErrorCode::MatchmakingTicketAlreadyCompleted: return E_PF_MATCHMAKING_TICKET_ALREADY_COMPLETED;
    case ServiceErrorCode::MatchmakingQueueConfigInvalid: return E_PF_MATCHMAKING_QUEUE_CONFIG_INVALID;
    case ServiceErrorCode::MatchmakingMemberProfileInvalid: return E_PF_MATCHMAKING_MEMBER_PROFILE_INVALID;
    case ServiceErrorCode::NintendoSwitchDeviceIdNotLinked: return E_PF_NINTENDO_SWITCH_DEVICE_ID_NOT_LINKED;
    case ServiceErrorCode::MatchmakingNotEnabled: return E_PF_MATCHMAKING_NOT_ENABLED;
    case ServiceErrorCode::MatchmakingPlayerAttributesTooLarge: return E_PF_MATCHMAKING_PLAYER_ATTRIBUTES_TOO_LARGE;
    case ServiceErrorCode::MatchmakingNumberOfPlayersInTicketTooLarge: return E_PF_MATCHMAKING_NUMBER_OF_PLAYERS_IN_TICKET_TOO_LARGE;
    case ServiceErrorCode::MatchmakingAttributeInvalid: return E_PF_MATCHMAKING_ATTRIBUTE_INVALID;
    case ServiceErrorCode::MatchmakingPlayerHasNotJoinedTicket: return E_PF_MATCHMAKING_PLAYER_HAS_NOT_JOINED_TICKET;
    case ServiceErrorCode::MatchmakingRateLimitExceeded: return E_PF_MATCHMAKING_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::MatchmakingTicketMembershipLimitExceeded: return E_PF_MATCHMAKING_TICKET_MEMBERSHIP_LIMIT_EXCEEDED;
    case ServiceErrorCode::MatchmakingUnauthorized: return E_PF_MATCHMAKING_UNAUTHORIZED;
    case ServiceErrorCode::MatchmakingQueueLimitExceeded: return E_PF_MATCHMAKING_QUEUE_LIMIT_EXCEEDED;
    case ServiceErrorCode::MatchmakingRequestTypeMismatch: return E_PF_MATCHMAKING_REQUEST_TYPE_MISMATCH;
    case ServiceErrorCode::MatchmakingBadRequest: return E_PF_MATCHMAKING_BAD_REQUEST;
    case ServiceErrorCode::PubSubFeatureNotEnabledForTitle: return E_PF_PUB_SUB_FEATURE_NOT_ENABLED_FOR_TITLE;
    case ServiceErrorCode::PubSubTooManyRequests: return E_PF_PUB_SUB_TOO_MANY_REQUESTS;
    case ServiceErrorCode::PubSubConnectionNotFoundForEntity: return E_PF_PUB_SUB_CONNECTION_NOT_FOUND_FOR_ENTITY;
    case ServiceErrorCode::PubSubConnectionHandleInvalid: return E_PF_PUB_SUB_CONNECTION_HANDLE_INVALID;
    case ServiceErrorCode::PubSubSubscriptionLimitExceeded: return E_PF_PUB_SUB_SUBSCRIPTION_LIMIT_EXCEEDED;
    case ServiceErrorCode::TitleConfigNotFound: return E_PF_TITLE_CONFIG_NOT_FOUND;
    case ServiceErrorCode::TitleConfigUpdateConflict: return E_PF_TITLE_CONFIG_UPDATE_CONFLICT;
    case ServiceErrorCode::TitleConfigSerializationError: return E_PF_TITLE_CONFIG_SERIALIZATION_ERROR;
    case ServiceErrorCode::CatalogApiNotImplemented: return E_PF_CATALOG_API_NOT_IMPLEMENTED;
    case ServiceErrorCode::CatalogEntityInvalid: return E_PF_CATALOG_ENTITY_INVALID;
    case ServiceErrorCode::CatalogTitleIdMissing: return E_PF_CATALOG_TITLE_ID_MISSING;
    case ServiceErrorCode::CatalogPlayerIdMissing: return E_PF_CATALOG_PLAYER_ID_MISSING;
    case ServiceErrorCode::CatalogClientIdentityInvalid: return E_PF_CATALOG_CLIENT_IDENTITY_INVALID;
    case ServiceErrorCode::CatalogOneOrMoreFilesInvalid: return E_PF_CATALOG_ONE_OR_MORE_FILES_INVALID;
    case ServiceErrorCode::CatalogItemMetadataInvalid: return E_PF_CATALOG_ITEM_METADATA_INVALID;
    case ServiceErrorCode::CatalogItemIdInvalid: return E_PF_CATALOG_ITEM_ID_INVALID;
    case ServiceErrorCode::CatalogSearchParameterInvalid: return E_PF_CATALOG_SEARCH_PARAMETER_INVALID;
    case ServiceErrorCode::CatalogFeatureDisabled: return E_PF_CATALOG_FEATURE_DISABLED;
    case ServiceErrorCode::CatalogConfigInvalid: return E_PF_CATALOG_CONFIG_INVALID;
    case ServiceErrorCode::CatalogItemTypeInvalid: return E_PF_CATALOG_ITEM_TYPE_INVALID;
    case ServiceErrorCode::CatalogBadRequest: return E_PF_CATALOG_BAD_REQUEST;
    case ServiceErrorCode::CatalogTooManyRequests: return E_PF_CATALOG_TOO_MANY_REQUESTS;
    case ServiceErrorCode::ExportInvalidStatusUpdate: return E_PF_EXPORT_INVALID_STATUS_UPDATE;
    case ServiceErrorCode::ExportInvalidPrefix: return E_PF_EXPORT_INVALID_PREFIX;
    case ServiceErrorCode::ExportBlobContainerDoesNotExist: return E_PF_EXPORT_BLOB_CONTAINER_DOES_NOT_EXIST;
    case ServiceErrorCode::ExportNotFound: return E_PF_EXPORT_NOT_FOUND;
    case ServiceErrorCode::ExportCouldNotUpdate: return E_PF_EXPORT_COULD_NOT_UPDATE;
    case ServiceErrorCode::ExportInvalidStorageType: return E_PF_EXPORT_INVALID_STORAGE_TYPE;
    case ServiceErrorCode::ExportAmazonBucketDoesNotExist: return E_PF_EXPORT_AMAZON_BUCKET_DOES_NOT_EXIST;
    case ServiceErrorCode::ExportInvalidBlobStorage: return E_PF_EXPORT_INVALID_BLOB_STORAGE;
    case ServiceErrorCode::ExportKustoException: return E_PF_EXPORT_KUSTO_EXCEPTION;
    case ServiceErrorCode::ExportKustoConnectionFailed: return E_PF_EXPORT_KUSTO_CONNECTION_FAILED;
    case ServiceErrorCode::ExportUnknownError: return E_PF_EXPORT_UNKNOWN_ERROR;
    case ServiceErrorCode::ExportCantEditPendingExport: return E_PF_EXPORT_CANT_EDIT_PENDING_EXPORT;
    case ServiceErrorCode::ExportLimitExports: return E_PF_EXPORT_LIMIT_EXPORTS;
    case ServiceErrorCode::ExportLimitEvents: return E_PF_EXPORT_LIMIT_EVENTS;
    case ServiceErrorCode::ExportInvalidPartitionStatusModification: return E_PF_EXPORT_INVALID_PARTITION_STATUS_MODIFICATION;
    case ServiceErrorCode::ExportCouldNotCreate: return E_PF_EXPORT_COULD_NOT_CREATE;
    case ServiceErrorCode::ExportNoBackingDatabaseFound: return E_PF_EXPORT_NO_BACKING_DATABASE_FOUND;
    case ServiceErrorCode::ExportCouldNotDelete: return E_PF_EXPORT_COULD_NOT_DELETE;
    case ServiceErrorCode::ExportCannotDetermineEventQuery: return E_PF_EXPORT_CANNOT_DETERMINE_EVENT_QUERY;
    case ServiceErrorCode::ExportInvalidQuerySchemaModification: return E_PF_EXPORT_INVALID_QUERY_SCHEMA_MODIFICATION;
    case ServiceErrorCode::ExportQuerySchemaMissingRequiredColumns: return E_PF_EXPORT_QUERY_SCHEMA_MISSING_REQUIRED_COLUMNS;
    case ServiceErrorCode::ExportCannotParseQuery: return E_PF_EXPORT_CANNOT_PARSE_QUERY;
    case ServiceErrorCode::ExportControlCommandsNotAllowed: return E_PF_EXPORT_CONTROL_COMMANDS_NOT_ALLOWED;
    case ServiceErrorCode::ExportQueryMissingTableReference: return E_PF_EXPORT_QUERY_MISSING_TABLE_REFERENCE;
    case ServiceErrorCode::ExportInsightsV1Deprecated: return E_PF_EXPORT_INSIGHTS_V_1_DEPRECATED;
    case ServiceErrorCode::ExplorerBasicInvalidQueryName: return E_PF_EXPLORER_BASIC_INVALID_QUERY_NAME;
    case ServiceErrorCode::ExplorerBasicInvalidQueryDescription: return E_PF_EXPLORER_BASIC_INVALID_QUERY_DESCRIPTION;
    case ServiceErrorCode::ExplorerBasicInvalidQueryConditions: return E_PF_EXPLORER_BASIC_INVALID_QUERY_CONDITIONS;
    case ServiceErrorCode::ExplorerBasicInvalidQueryStartDate: return E_PF_EXPLORER_BASIC_INVALID_QUERY_START_DATE;
    case ServiceErrorCode::ExplorerBasicInvalidQueryEndDate: return E_PF_EXPLORER_BASIC_INVALID_QUERY_END_DATE;
    case ServiceErrorCode::ExplorerBasicInvalidQueryGroupBy: return E_PF_EXPLORER_BASIC_INVALID_QUERY_GROUP_BY;
    case ServiceErrorCode::ExplorerBasicInvalidQueryAggregateType: return E_PF_EXPLORER_BASIC_INVALID_QUERY_AGGREGATE_TYPE;
    case ServiceErrorCode::ExplorerBasicInvalidQueryAggregateProperty: return E_PF_EXPLORER_BASIC_INVALID_QUERY_AGGREGATE_PROPERTY;
    case ServiceErrorCode::ExplorerBasicLoadQueriesError: return E_PF_EXPLORER_BASIC_LOAD_QUERIES_ERROR;
    case ServiceErrorCode::ExplorerBasicLoadQueryError: return E_PF_EXPLORER_BASIC_LOAD_QUERY_ERROR;
    case ServiceErrorCode::ExplorerBasicCreateQueryError: return E_PF_EXPLORER_BASIC_CREATE_QUERY_ERROR;
    case ServiceErrorCode::ExplorerBasicDeleteQueryError: return E_PF_EXPLORER_BASIC_DELETE_QUERY_ERROR;
    case ServiceErrorCode::ExplorerBasicUpdateQueryError: return E_PF_EXPLORER_BASIC_UPDATE_QUERY_ERROR;
    case ServiceErrorCode::ExplorerBasicSavedQueriesLimit: return E_PF_EXPLORER_BASIC_SAVED_QUERIES_LIMIT;
    case ServiceErrorCode::ExplorerBasicSavedQueryNotFound: return E_PF_EXPLORER_BASIC_SAVED_QUERY_NOT_FOUND;
    case ServiceErrorCode::TenantShardMapperShardNotFound: return E_PF_TENANT_SHARD_MAPPER_SHARD_NOT_FOUND;
    case ServiceErrorCode::TitleNotEnabledForParty: return E_PF_TITLE_NOT_ENABLED_FOR_PARTY;
    case ServiceErrorCode::PartyVersionNotFound: return E_PF_PARTY_VERSION_NOT_FOUND;
    case ServiceErrorCode::MultiplayerServerBuildReferencedByMatchmakingQueue: return E_PF_MULTIPLAYER_SERVER_BUILD_REFERENCED_BY_MATCHMAKING_QUEUE;
    case ServiceErrorCode::MultiplayerServerBuildReferencedByBuildAlias: return E_PF_MULTIPLAYER_SERVER_BUILD_REFERENCED_BY_BUILD_ALIAS;
    case ServiceErrorCode::MultiplayerServerBuildAliasReferencedByMatchmakingQueue: return E_PF_MULTIPLAYER_SERVER_BUILD_ALIAS_REFERENCED_BY_MATCHMAKING_QUEUE;
    case ServiceErrorCode::PartySerializationError: return E_PF_PARTY_SERIALIZATION_ERROR;
    case ServiceErrorCode::ExperimentationExperimentStopped: return E_PF_EXPERIMENTATION_EXPERIMENT_STOPPED;
    case ServiceErrorCode::ExperimentationExperimentRunning: return E_PF_EXPERIMENTATION_EXPERIMENT_RUNNING;
    case ServiceErrorCode::ExperimentationExperimentNotFound: return E_PF_EXPERIMENTATION_EXPERIMENT_NOT_FOUND;
    case ServiceErrorCode::ExperimentationExperimentNeverStarted: return E_PF_EXPERIMENTATION_EXPERIMENT_NEVER_STARTED;
    case ServiceErrorCode::ExperimentationExperimentDeleted: return E_PF_EXPERIMENTATION_EXPERIMENT_DELETED;
    case ServiceErrorCode::ExperimentationClientTimeout: return E_PF_EXPERIMENTATION_CLIENT_TIMEOUT;
    case ServiceErrorCode::ExperimentationInvalidVariantConfiguration: return E_PF_EXPERIMENTATION_INVALID_VARIANT_CONFIGURATION;
    case ServiceErrorCode::ExperimentationInvalidVariableConfiguration: return E_PF_EXPERIMENTATION_INVALID_VARIABLE_CONFIGURATION;
    case ServiceErrorCode::ExperimentInvalidId: return E_PF_EXPERIMENT_INVALID_ID;
    case ServiceErrorCode::ExperimentationNoScorecard: return E_PF_EXPERIMENTATION_NO_SCORECARD;
    case ServiceErrorCode::ExperimentationTreatmentAssignmentFailed: return E_PF_EXPERIMENTATION_TREATMENT_ASSIGNMENT_FAILED;
    case ServiceErrorCode::ExperimentationTreatmentAssignmentDisabled: return E_PF_EXPERIMENTATION_TREATMENT_ASSIGNMENT_DISABLED;
    case ServiceErrorCode::ExperimentationInvalidDuration: return E_PF_EXPERIMENTATION_INVALID_DURATION;
    case ServiceErrorCode::ExperimentationMaxExperimentsReached: return E_PF_EXPERIMENTATION_MAX_EXPERIMENTS_REACHED;
    case ServiceErrorCode::ExperimentationExperimentSchedulingInProgress: return E_PF_EXPERIMENTATION_EXPERIMENT_SCHEDULING_IN_PROGRESS;
    case ServiceErrorCode::ExperimentationInvalidEndDate: return E_PF_EXPERIMENTATION_INVALID_END_DATE;
    case ServiceErrorCode::ExperimentationInvalidStartDate: return E_PF_EXPERIMENTATION_INVALID_START_DATE;
    case ServiceErrorCode::ExperimentationMaxDurationExceeded: return E_PF_EXPERIMENTATION_MAX_DURATION_EXCEEDED;
    case ServiceErrorCode::ExperimentationExclusionGroupNotFound: return E_PF_EXPERIMENTATION_EXCLUSION_GROUP_NOT_FOUND;
    case ServiceErrorCode::ExperimentationExclusionGroupInsufficientCapacity: return E_PF_EXPERIMENTATION_EXCLUSION_GROUP_INSUFFICIENT_CAPACITY;
    case ServiceErrorCode::ExperimentationExclusionGroupCannotDelete: return E_PF_EXPERIMENTATION_EXCLUSION_GROUP_CANNOT_DELETE;
    case ServiceErrorCode::ExperimentationExclusionGroupInvalidTrafficAllocation: return E_PF_EXPERIMENTATION_EXCLUSION_GROUP_INVALID_TRAFFIC_ALLOCATION;
    case ServiceErrorCode::ExperimentationExclusionGroupInvalidName: return E_PF_EXPERIMENTATION_EXCLUSION_GROUP_INVALID_NAME;
    case ServiceErrorCode::MaxActionDepthExceeded: return E_PF_MAX_ACTION_DEPTH_EXCEEDED;
    case ServiceErrorCode::TitleNotOnUpdatedPricingPlan: return E_PF_TITLE_NOT_ON_UPDATED_PRICING_PLAN;
    case ServiceErrorCode::SegmentManagementTitleNotInFlight: return E_PF_SEGMENT_MANAGEMENT_TITLE_NOT_IN_FLIGHT;
    case ServiceErrorCode::SegmentManagementNoExpressionTree: return E_PF_SEGMENT_MANAGEMENT_NO_EXPRESSION_TREE;
    case ServiceErrorCode::SegmentManagementTriggerActionCountOverLimit: return E_PF_SEGMENT_MANAGEMENT_TRIGGER_ACTION_COUNT_OVER_LIMIT;
    case ServiceErrorCode::SegmentManagementSegmentCountOverLimit: return E_PF_SEGMENT_MANAGEMENT_SEGMENT_COUNT_OVER_LIMIT;
    case ServiceErrorCode::SegmentManagementInvalidSegmentId: return E_PF_SEGMENT_MANAGEMENT_INVALID_SEGMENT_ID;
    case ServiceErrorCode::SegmentManagementInvalidInput: return E_PF_SEGMENT_MANAGEMENT_INVALID_INPUT;
    case ServiceErrorCode::SegmentManagementInvalidSegmentName: return E_PF_SEGMENT_MANAGEMENT_INVALID_SEGMENT_NAME;
    case ServiceErrorCode::DeleteSegmentRateLimitExceeded: return E_PF_DELETE_SEGMENT_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::CreateSegmentRateLimitExceeded: return E_PF_CREATE_SEGMENT_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::UpdateSegmentRateLimitExceeded: return E_PF_UPDATE_SEGMENT_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::GetSegmentsRateLimitExceeded: return E_PF_GET_SEGMENTS_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::AsyncExportNotInFlight: return E_PF_ASYNC_EXPORT_NOT_IN_FLIGHT;
    case ServiceErrorCode::AsyncExportNotFound: return E_PF_ASYNC_EXPORT_NOT_FOUND;
    case ServiceErrorCode::AsyncExportRateLimitExceeded: return E_PF_ASYNC_EXPORT_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::AnalyticsSegmentCountOverLimit: return E_PF_ANALYTICS_SEGMENT_COUNT_OVER_LIMIT;
    case ServiceErrorCode::SnapshotNotFound: return E_PF_SNAPSHOT_NOT_FOUND;
    case ServiceErrorCode::InventoryApiNotImplemented: return E_PF_INVENTORY_API_NOT_IMPLEMENTED;
    case ServiceErrorCode::LobbyDoesNotExist: return E_PF_LOBBY_DOES_NOT_EXIST;
    case ServiceErrorCode::LobbyRateLimitExceeded: return E_PF_LOBBY_RATE_LIMIT_EXCEEDED;
    case ServiceErrorCode::LobbyPlayerAlreadyJoined: return E_PF_LOBBY_PLAYER_ALREADY_JOINED;
    case ServiceErrorCode::LobbyNotJoinable: return E_PF_LOBBY_NOT_JOINABLE;
    case ServiceErrorCode::LobbyMemberCannotRejoin: return E_PF_LOBBY_MEMBER_CANNOT_REJOIN;
    case ServiceErrorCode::LobbyCurrentPlayersMoreThanMaxPlayers: return E_PF_LOBBY_CURRENT_PLAYERS_MORE_THAN_MAX_PLAYERS;
    case ServiceErrorCode::LobbyPlayerNotPresent: return E_PF_LOBBY_PLAYER_NOT_PRESENT;
    case ServiceErrorCode::LobbyBadRequest: return E_PF_LOBBY_BAD_REQUEST;
    case ServiceErrorCode::LobbyPlayerMaxLobbyLimitExceeded: return E_PF_LOBBY_PLAYER_MAX_LOBBY_LIMIT_EXCEEDED;
    case ServiceErrorCode::LobbyNewOwnerMustBeConnected: return E_PF_LOBBY_NEW_OWNER_MUST_BE_CONNECTED;
    case ServiceErrorCode::LobbyCurrentOwnerStillConnected: return E_PF_LOBBY_CURRENT_OWNER_STILL_CONNECTED;
    case ServiceErrorCode::LobbyMemberIsNotOwner: return E_PF_LOBBY_MEMBER_IS_NOT_OWNER;
    case ServiceErrorCode::EventSamplingInvalidRatio: return E_PF_EVENT_SAMPLING_INVALID_RATIO;
    case ServiceErrorCode::EventSamplingInvalidEventNamespace: return E_PF_EVENT_SAMPLING_INVALID_EVENT_NAMESPACE;
    case ServiceErrorCode::EventSamplingInvalidEventName: return E_PF_EVENT_SAMPLING_INVALID_EVENT_NAME;
    case ServiceErrorCode::EventSamplingRatioNotFound: return E_PF_EVENT_SAMPLING_RATIO_NOT_FOUND;
    case ServiceErrorCode::TelemetryKeyNotFound: return E_PF_TELEMETRY_KEY_NOT_FOUND;
    case ServiceErrorCode::TelemetryKeyInvalidName: return E_PF_TELEMETRY_KEY_INVALID_NAME;
    case ServiceErrorCode::TelemetryKeyAlreadyExists: return E_PF_TELEMETRY_KEY_ALREADY_EXISTS;
    case ServiceErrorCode::TelemetryKeyInvalid: return E_PF_TELEMETRY_KEY_INVALID;
    case ServiceErrorCode::TelemetryKeyCountOverLimit: return E_PF_TELEMETRY_KEY_COUNT_OVER_LIMIT;
    case ServiceErrorCode::TelemetryKeyDeactivated: return E_PF_TELEMETRY_KEY_DEACTIVATED;
    case ServiceErrorCode::TelemetryKeyLongInsightsRetentionNotAllowed: return E_PF_TELEMETRY_KEY_LONG_INSIGHTS_RETENTION_NOT_ALLOWED;
    case ServiceErrorCode::EventSinkConnectionInvalid: return E_PF_EVENT_SINK_CONNECTION_INVALID;
    case ServiceErrorCode::EventSinkConnectionUnauthorized: return E_PF_EVENT_SINK_CONNECTION_UNAUTHORIZED;
    case ServiceErrorCode::EventSinkRegionInvalid: return E_PF_EVENT_SINK_REGION_INVALID;
    case ServiceErrorCode::EventSinkLimitExceeded: return E_PF_EVENT_SINK_LIMIT_EXCEEDED;
    case ServiceErrorCode::EventSinkSasTokenInvalid: return E_PF_EVENT_SINK_SAS_TOKEN_INVALID;
    case ServiceErrorCode::EventSinkNotFound: return E_PF_EVENT_SINK_NOT_FOUND;
    case ServiceErrorCode::EventSinkNameInvalid: return E_PF_EVENT_SINK_NAME_INVALID;
    case ServiceErrorCode::EventSinkSasTokenPermissionInvalid: return E_PF_EVENT_SINK_SAS_TOKEN_PERMISSION_INVALID;
    case ServiceErrorCode::EventSinkSecretInvalid: return E_PF_EVENT_SINK_SECRET_INVALID;
    case ServiceErrorCode::EventSinkTenantNotFound: return E_PF_EVENT_SINK_TENANT_NOT_FOUND;
    case ServiceErrorCode::EventSinkAadNotFound: return E_PF_EVENT_SINK_AAD_NOT_FOUND;
    case ServiceErrorCode::EventSinkDatabaseNotFound: return E_PF_EVENT_SINK_DATABASE_NOT_FOUND;
    case ServiceErrorCode::OperationCanceled: return E_PF_OPERATION_CANCELED;
    case ServiceErrorCode::InvalidDisplayNameRandomSuffixLength: return E_PF_INVALID_DISPLAY_NAME_RANDOM_SUFFIX_LENGTH;
    case ServiceErrorCode::AllowNonUniquePlayerDisplayNamesDisableNotAllowed: return E_PF_ALLOW_NON_UNIQUE_PLAYER_DISPLAY_NAMES_DISABLE_NOT_ALLOWED;
    case ServiceErrorCode::PartitionedEventInvalid: return E_PF_PARTITIONED_EVENT_INVALID;
    case ServiceErrorCode::PartitionedEventCountOverLimit: return E_PF_PARTITIONED_EVENT_COUNT_OVER_LIMIT;
    case ServiceErrorCode::ManageEventNamespaceInvalid: return E_PF_MANAGE_EVENT_NAMESPACE_INVALID;
    case ServiceErrorCode::ManageEventNameInvalid: return E_PF_MANAGE_EVENT_NAME_INVALID;
    case ServiceErrorCode::ManagedEventNotFound: return E_PF_MANAGED_EVENT_NOT_FOUND;
    case ServiceErrorCode::ManageEventsInvalidRatio: return E_PF_MANAGE_EVENTS_INVALID_RATIO;
    case ServiceErrorCode::ManagedEventInvalid: return E_PF_MANAGED_EVENT_INVALID;
    case ServiceErrorCode::PlayerCustomPropertiesPropertyNameTooLong: return E_PF_PLAYER_CUSTOM_PROPERTIES_PROPERTY_NAME_TOO_LONG;
    case ServiceErrorCode::PlayerCustomPropertiesPropertyNameIsInvalid: return E_PF_PLAYER_CUSTOM_PROPERTIES_PROPERTY_NAME_IS_INVALID;
    case ServiceErrorCode::PlayerCustomPropertiesStringPropertyValueTooLong: return E_PF_PLAYER_CUSTOM_PROPERTIES_STRING_PROPERTY_VALUE_TOO_LONG;
    case ServiceErrorCode::PlayerCustomPropertiesValueIsInvalidType: return E_PF_PLAYER_CUSTOM_PROPERTIES_VALUE_IS_INVALID_TYPE;
    case ServiceErrorCode::PlayerCustomPropertiesVersionMismatch: return E_PF_PLAYER_CUSTOM_PROPERTIES_VERSION_MISMATCH;
    case ServiceErrorCode::PlayerCustomPropertiesPropertyCountTooHigh: return E_PF_PLAYER_CUSTOM_PROPERTIES_PROPERTY_COUNT_TOO_HIGH;
    case ServiceErrorCode::PlayerCustomPropertiesDuplicatePropertyName: return E_PF_PLAYER_CUSTOM_PROPERTIES_DUPLICATE_PROPERTY_NAME;
    case ServiceErrorCode::PlayerCustomPropertiesPropertyDoesNotExist: return E_PF_PLAYER_CUSTOM_PROPERTIES_PROPERTY_DOES_NOT_EXIST;
    case ServiceErrorCode::AddonAlreadyExists: return E_PF_ADDON_ALREADY_EXISTS;
    case ServiceErrorCode::AddonDoesntExist: return E_PF_ADDON_DOESNT_EXIST;
    default: return E_PF_UNKNOWN_ERROR;
    }
}

#define MAKE_HTTP_HRESULT(httpStatus) MAKE_HRESULT(1, 0x019, httpStatus)

HRESULT HttpStatusToHR(uint32_t httpStatus)
{
    // 2xx are http success codes
    if (httpStatus >= 200 && httpStatus < 300)
    {
        return S_OK;
    }
    else if (httpStatus == 1223)
    {
        // MSXML XHR bug: get_status() returns HTTP/1223 for HTTP/204:
        // http://blogs.msdn.com/b/ieinternals/archive/2009/07/23/the-ie8-native-xmlhttprequest-object.aspx
        // treat it as success code as well
        return S_OK;
    }
    else
    {
        switch (httpStatus)
        {
        case 300: return HTTP_E_STATUS_AMBIGUOUS;
        case 301: return HTTP_E_STATUS_MOVED;
        case 302: return HTTP_E_STATUS_REDIRECT;
        case 303: return HTTP_E_STATUS_REDIRECT_METHOD;
        case 304: return HTTP_E_STATUS_NOT_MODIFIED;
        case 305: return HTTP_E_STATUS_USE_PROXY;
        case 306: return HTTP_E_STATUS_REDIRECT_KEEP_VERB;

        case 400: return HTTP_E_STATUS_BAD_REQUEST;
        case 401: return HTTP_E_STATUS_DENIED;
        case 402: return HTTP_E_STATUS_PAYMENT_REQ;
        case 403: return HTTP_E_STATUS_FORBIDDEN;
        case 404: return HTTP_E_STATUS_NOT_FOUND;
        case 405: return HTTP_E_STATUS_BAD_METHOD;
        case 406: return HTTP_E_STATUS_NONE_ACCEPTABLE;
        case 407: return HTTP_E_STATUS_PROXY_AUTH_REQ;
        case 408: return HTTP_E_STATUS_REQUEST_TIMEOUT;
        case 409: return HTTP_E_STATUS_CONFLICT;
        case 410: return HTTP_E_STATUS_GONE;
        case 411: return HTTP_E_STATUS_LENGTH_REQUIRED;
        case 412: return HTTP_E_STATUS_PRECOND_FAILED;
        case 413: return HTTP_E_STATUS_REQUEST_TOO_LARGE;
        case 414: return HTTP_E_STATUS_URI_TOO_LONG;
        case 415: return HTTP_E_STATUS_UNSUPPORTED_MEDIA;
        case 416: return HTTP_E_STATUS_RANGE_NOT_SATISFIABLE;
        case 417: return HTTP_E_STATUS_EXPECTATION_FAILED;
        case 421: return MAKE_HTTP_HRESULT(421); // Misdirected request
        case 422: return MAKE_HTTP_HRESULT(422); // Unprocessable entity
        case 423: return MAKE_HTTP_HRESULT(423); // Locked
        case 424: return MAKE_HTTP_HRESULT(424); // Failed dependency
        case 426: return MAKE_HTTP_HRESULT(426); // Upgrade required
        case 428: return MAKE_HTTP_HRESULT(428); // Precondition required
        case 429: return MAKE_HTTP_HRESULT(429); // Too many requests
        case 431: return MAKE_HTTP_HRESULT(431); // Request header fields too large
        case 449: return MAKE_HTTP_HRESULT(449); // Retry with
        case 451: return MAKE_HTTP_HRESULT(451); // Unavailable for legal reasons

        case 500: return HTTP_E_STATUS_SERVER_ERROR;
        case 501: return HTTP_E_STATUS_NOT_SUPPORTED;
        case 502: return HTTP_E_STATUS_BAD_GATEWAY;
        case 503: return HTTP_E_STATUS_SERVICE_UNAVAIL;
        case 504: return HTTP_E_STATUS_GATEWAY_TIMEOUT;
        case 505: return HTTP_E_STATUS_VERSION_NOT_SUP;
        case 506: return MAKE_HTTP_HRESULT(506); // Variant also negotiates
        case 507: return MAKE_HTTP_HRESULT(507); // Insufficient storage
        case 508: return MAKE_HTTP_HRESULT(508); // Loop detected
        case 510: return MAKE_HTTP_HRESULT(510); // Not extended
        case 511: return MAKE_HTTP_HRESULT(511); // Network authentication required

        default: return HTTP_E_STATUS_UNEXPECTED;
        }
    }
}

HRESULT CurrentExceptionToHR()
{
    try
    {
        throw;
    }
    // std exceptions
    catch (const std::bad_alloc&) // is an exception
    {
        return E_OUTOFMEMORY;
    }
    catch (const std::bad_cast&) // is an exception
    {
        return E_NOINTERFACE;
    }
    catch (const std::invalid_argument&) // is a logic_error
    {
        return E_INVALIDARG;
    }
    catch (const std::out_of_range&) // is a logic_error
    {
        return E_BOUNDS;
    }
    catch (const std::length_error&) // is a logic_error
    {
        return __HRESULT_FROM_WIN32(ERROR_BAD_LENGTH);
    }
    catch (const std::overflow_error&) // is a runtime_error
    {
        return __HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
    }
    catch (const std::underflow_error&) // is a runtime_error
    {
        return __HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
    }
    catch (const std::range_error&) // is a runtime_error
    {
        return E_BOUNDS;
    }
    catch (const std::system_error& ex) // is a runtime_error
    {
        if (ex.code().category() == std::system_category())
        {
            return __HRESULT_FROM_WIN32(ex.code().value());
        }
        else
        {
            return ex.code().value();
        }
    }
    catch (const std::logic_error&) // is an exception
    {
        return E_UNEXPECTED;
    }
    catch (const std::runtime_error&) // is an exception
    {
        return E_FAIL;
    }
    catch (const std::exception&) // base class for standard C++ exceptions
    {
        return E_FAIL;
    }
    catch (HRESULT exceptionHR)
    {
        return exceptionHR;
    }
    catch (...) // everything else
    {
        return E_FAIL;
    }
}

}

#include "stdafx.h"
#include "SegmentsTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Segments
{

HRESULT GetSegmentResult::FromJson(const JsonValue& input)
{
    String aBTestParent{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ABTestParent", aBTestParent));
    this->SetABTestParent(std::move(aBTestParent));

    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    return S_OK;
}

size_t GetSegmentResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsGetSegmentResult const*> GetSegmentResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetSegmentResult>(&this->Model());
}

size_t GetSegmentResult::RequiredBufferSize(const PFSegmentsGetSegmentResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.aBTestParent)
    {
        requiredSize += (std::strlen(model.aBTestParent) + 1);
    }
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT GetSegmentResult::Copy(const PFSegmentsGetSegmentResult& input, PFSegmentsGetSegmentResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.aBTestParent); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.aBTestParent = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.id); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetPlayerSegmentsResult::FromJson(const JsonValue& input)
{
    ModelVector<GetSegmentResult> segments{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GetSegmentResult>(input, "Segments", segments));
    this->SetSegments(std::move(segments));

    return S_OK;
}

size_t GetPlayerSegmentsResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsGetPlayerSegmentsResult const*> GetPlayerSegmentsResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetPlayerSegmentsResult>(&this->Model());
}

size_t GetPlayerSegmentsResult::RequiredBufferSize(const PFSegmentsGetPlayerSegmentsResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFSegmentsGetSegmentResult*) + sizeof(PFSegmentsGetSegmentResult*) * model.segmentsCount);
    for (size_t i = 0; i < model.segmentsCount; ++i)
    {
        requiredSize += GetSegmentResult::RequiredBufferSize(*model.segments[i]);
    }
    return requiredSize;
}

HRESULT GetPlayerSegmentsResult::Copy(const PFSegmentsGetPlayerSegmentsResult& input, PFSegmentsGetPlayerSegmentsResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<GetSegmentResult>(input.segments, input.segmentsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.segments = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetPlayerTagsRequest::ToJson() const
{
    return GetPlayerTagsRequest::ToJson(this->Model());
}

JsonValue GetPlayerTagsRequest::ToJson(const PFSegmentsGetPlayerTagsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Namespace", input.playfabNamespace);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

HRESULT GetPlayerTagsResult::FromJson(const JsonValue& input)
{
    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    return S_OK;
}

size_t GetPlayerTagsResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsGetPlayerTagsResult const*> GetPlayerTagsResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetPlayerTagsResult>(&this->Model());
}

size_t GetPlayerTagsResult::RequiredBufferSize(const PFSegmentsGetPlayerTagsResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    return requiredSize;
}

HRESULT GetPlayerTagsResult::Copy(const PFSegmentsGetPlayerTagsResult& input, PFSegmentsGetPlayerTagsResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue AddPlayerTagRequest::ToJson() const
{
    return AddPlayerTagRequest::ToJson(this->Model());
}

JsonValue AddPlayerTagRequest::ToJson(const PFSegmentsAddPlayerTagRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMember(output, "TagName", input.tagName);
    return output;
}

HRESULT GetAllSegmentsResult::FromJson(const JsonValue& input)
{
    ModelVector<GetSegmentResult> segments{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GetSegmentResult>(input, "Segments", segments));
    this->SetSegments(std::move(segments));

    return S_OK;
}

size_t GetAllSegmentsResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsGetAllSegmentsResult const*> GetAllSegmentsResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetAllSegmentsResult>(&this->Model());
}

size_t GetAllSegmentsResult::RequiredBufferSize(const PFSegmentsGetAllSegmentsResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFSegmentsGetSegmentResult*) + sizeof(PFSegmentsGetSegmentResult*) * model.segmentsCount);
    for (size_t i = 0; i < model.segmentsCount; ++i)
    {
        requiredSize += GetSegmentResult::RequiredBufferSize(*model.segments[i]);
    }
    return requiredSize;
}

HRESULT GetAllSegmentsResult::Copy(const PFSegmentsGetAllSegmentsResult& input, PFSegmentsGetAllSegmentsResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<GetSegmentResult>(input.segments, input.segmentsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.segments = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetPlayersSegmentsRequest::ToJson() const
{
    return GetPlayersSegmentsRequest::ToJson(this->Model());
}

JsonValue GetPlayersSegmentsRequest::ToJson(const PFSegmentsGetPlayersSegmentsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

JsonValue GetPlayersInSegmentRequest::ToJson() const
{
    return GetPlayersInSegmentRequest::ToJson(this->Model());
}

JsonValue GetPlayersInSegmentRequest::ToJson(const PFSegmentsGetPlayersInSegmentRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "ContinuationToken", input.continuationToken);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "GetProfilesAsync", input.getProfilesAsync);
    JsonUtils::ObjectAddMember(output, "MaxBatchSize", input.maxBatchSize);
    JsonUtils::ObjectAddMember(output, "SecondsToLive", input.secondsToLive);
    JsonUtils::ObjectAddMember(output, "SegmentId", input.segmentId);
    return output;
}

HRESULT AdCampaignAttribution::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "AttributedAt", this->m_model.attributedAt));

    String campaignId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CampaignId", campaignId));
    this->SetCampaignId(std::move(campaignId));

    String platform{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Platform", platform));
    this->SetPlatform(std::move(platform));

    return S_OK;
}

size_t AdCampaignAttribution::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsAdCampaignAttribution const*> AdCampaignAttribution::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<AdCampaignAttribution>(&this->Model());
}

size_t AdCampaignAttribution::RequiredBufferSize(const PFSegmentsAdCampaignAttribution& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.campaignId)
    {
        requiredSize += (std::strlen(model.campaignId) + 1);
    }
    if (model.platform)
    {
        requiredSize += (std::strlen(model.platform) + 1);
    }
    return requiredSize;
}

HRESULT AdCampaignAttribution::Copy(const PFSegmentsAdCampaignAttribution& input, PFSegmentsAdCampaignAttribution& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.campaignId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.campaignId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.platform); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.platform = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ContactEmailInfo::FromJson(const JsonValue& input)
{
    String emailAddress{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EmailAddress", emailAddress));
    this->SetEmailAddress(std::move(emailAddress));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    std::optional<PFEmailVerificationStatus> verificationStatus{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VerificationStatus", verificationStatus));
    this->SetVerificationStatus(std::move(verificationStatus));

    return S_OK;
}

size_t ContactEmailInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsContactEmailInfo const*> ContactEmailInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ContactEmailInfo>(&this->Model());
}

size_t ContactEmailInfo::RequiredBufferSize(const PFSegmentsContactEmailInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.emailAddress)
    {
        requiredSize += (std::strlen(model.emailAddress) + 1);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    if (model.verificationStatus)
    {
        requiredSize += (alignof(PFEmailVerificationStatus) + sizeof(PFEmailVerificationStatus));
    }
    return requiredSize;
}

HRESULT ContactEmailInfo::Copy(const PFSegmentsContactEmailInfo& input, PFSegmentsContactEmailInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.emailAddress); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.emailAddress = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.verificationStatus); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.verificationStatus = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT PlayerLinkedAccount::FromJson(const JsonValue& input)
{
    String email{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Email", email));
    this->SetEmail(std::move(email));

    std::optional<PFLoginIdentityProvider> platform{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Platform", platform));
    this->SetPlatform(std::move(platform));

    String platformUserId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlatformUserId", platformUserId));
    this->SetPlatformUserId(std::move(platformUserId));

    String username{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Username", username));
    this->SetUsername(std::move(username));

    return S_OK;
}

size_t PlayerLinkedAccount::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsPlayerLinkedAccount const*> PlayerLinkedAccount::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<PlayerLinkedAccount>(&this->Model());
}

size_t PlayerLinkedAccount::RequiredBufferSize(const PFSegmentsPlayerLinkedAccount& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.email)
    {
        requiredSize += (std::strlen(model.email) + 1);
    }
    if (model.platform)
    {
        requiredSize += (alignof(PFLoginIdentityProvider) + sizeof(PFLoginIdentityProvider));
    }
    if (model.platformUserId)
    {
        requiredSize += (std::strlen(model.platformUserId) + 1);
    }
    if (model.username)
    {
        requiredSize += (std::strlen(model.username) + 1);
    }
    return requiredSize;
}

HRESULT PlayerLinkedAccount::Copy(const PFSegmentsPlayerLinkedAccount& input, PFSegmentsPlayerLinkedAccount& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.email); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.email = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.platform); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.platform = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.platformUserId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.platformUserId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.username); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.username = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT PlayerLocation::FromJson(const JsonValue& input)
{
    String city{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "City", city));
    this->SetCity(std::move(city));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinentCode", this->m_model.continentCode));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CountryCode", this->m_model.countryCode));

    std::optional<double> latitude{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Latitude", latitude));
    this->SetLatitude(std::move(latitude));

    std::optional<double> longitude{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Longitude", longitude));
    this->SetLongitude(std::move(longitude));

    return S_OK;
}

size_t PlayerLocation::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsPlayerLocation const*> PlayerLocation::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<PlayerLocation>(&this->Model());
}

size_t PlayerLocation::RequiredBufferSize(const PFSegmentsPlayerLocation& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.city)
    {
        requiredSize += (std::strlen(model.city) + 1);
    }
    if (model.latitude)
    {
        requiredSize += (alignof(double) + sizeof(double));
    }
    if (model.longitude)
    {
        requiredSize += (alignof(double) + sizeof(double));
    }
    return requiredSize;
}

HRESULT PlayerLocation::Copy(const PFSegmentsPlayerLocation& input, PFSegmentsPlayerLocation& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.city); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.city = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.latitude); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.latitude = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.longitude); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.longitude = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT PlayerStatistic::FromJson(const JsonValue& input)
{
    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StatisticValue", this->m_model.statisticValue));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StatisticVersion", this->m_model.statisticVersion));

    return S_OK;
}

size_t PlayerStatistic::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsPlayerStatistic const*> PlayerStatistic::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<PlayerStatistic>(&this->Model());
}

size_t PlayerStatistic::RequiredBufferSize(const PFSegmentsPlayerStatistic& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT PlayerStatistic::Copy(const PFSegmentsPlayerStatistic& input, PFSegmentsPlayerStatistic& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.id); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT PushNotificationRegistration::FromJson(const JsonValue& input)
{
    String notificationEndpointARN{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NotificationEndpointARN", notificationEndpointARN));
    this->SetNotificationEndpointARN(std::move(notificationEndpointARN));

    std::optional<PFPushNotificationPlatform> platform{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Platform", platform));
    this->SetPlatform(std::move(platform));

    return S_OK;
}

size_t PushNotificationRegistration::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsPushNotificationRegistration const*> PushNotificationRegistration::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<PushNotificationRegistration>(&this->Model());
}

size_t PushNotificationRegistration::RequiredBufferSize(const PFSegmentsPushNotificationRegistration& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.notificationEndpointARN)
    {
        requiredSize += (std::strlen(model.notificationEndpointARN) + 1);
    }
    if (model.platform)
    {
        requiredSize += (alignof(PFPushNotificationPlatform) + sizeof(PFPushNotificationPlatform));
    }
    return requiredSize;
}

HRESULT PushNotificationRegistration::Copy(const PFSegmentsPushNotificationRegistration& input, PFSegmentsPushNotificationRegistration& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.notificationEndpointARN); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.notificationEndpointARN = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.platform); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.platform = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT PlayerProfile::FromJson(const JsonValue& input)
{
    ModelVector<AdCampaignAttribution> adCampaignAttributions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<AdCampaignAttribution>(input, "AdCampaignAttributions", adCampaignAttributions));
    this->SetAdCampaignAttributions(std::move(adCampaignAttributions));

    String avatarUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AvatarUrl", avatarUrl));
    this->SetAvatarUrl(std::move(avatarUrl));

    std::optional<time_t> bannedUntil{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "BannedUntil", bannedUntil));
    this->SetBannedUntil(std::move(bannedUntil));

    std::optional<PFSegmentsChurnRiskLevel> churnPrediction{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ChurnPrediction", churnPrediction));
    this->SetChurnPrediction(std::move(churnPrediction));

    ModelVector<ContactEmailInfo> contactEmailAddresses{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<ContactEmailInfo>(input, "ContactEmailAddresses", contactEmailAddresses));
    this->SetContactEmailAddresses(std::move(contactEmailAddresses));

    std::optional<time_t> created{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", created));
    this->SetCreated(std::move(created));

    String displayName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisplayName", displayName));
    this->SetDisplayName(std::move(displayName));

    std::optional<time_t> lastLogin{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastLogin", lastLogin));
    this->SetLastLogin(std::move(lastLogin));

    ModelVector<PlayerLinkedAccount> linkedAccounts{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<PlayerLinkedAccount>(input, "LinkedAccounts", linkedAccounts));
    this->SetLinkedAccounts(std::move(linkedAccounts));

    ModelDictionaryEntryVector<PlayerLocation> locations{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<PlayerLocation>(input, "Locations", locations));
    this->SetLocations(std::move(locations));

    std::optional<PFLoginIdentityProvider> origination{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Origination", origination));
    this->SetOrigination(std::move(origination));

    CStringVector playerExperimentVariants{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayerExperimentVariants", playerExperimentVariants));
    this->SetPlayerExperimentVariants(std::move(playerExperimentVariants));

    String playerId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayerId", playerId));
    this->SetPlayerId(std::move(playerId));

    ModelVector<PlayerStatistic> playerStatistics{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<PlayerStatistic>(input, "PlayerStatistics", playerStatistics));
    this->SetPlayerStatistics(std::move(playerStatistics));

    String publisherId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PublisherId", publisherId));
    this->SetPublisherId(std::move(publisherId));

    ModelVector<PushNotificationRegistration> pushNotificationRegistrations{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<PushNotificationRegistration>(input, "PushNotificationRegistrations", pushNotificationRegistrations));
    this->SetPushNotificationRegistrations(std::move(pushNotificationRegistrations));

    DictionaryEntryVector<PFInt32DictionaryEntry> statistics{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Statistics", statistics));
    this->SetStatistics(std::move(statistics));

    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    String titleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitleId", titleId));
    this->SetTitleId(std::move(titleId));

    std::optional<uint32_t> totalValueToDateInUSD{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TotalValueToDateInUSD", totalValueToDateInUSD));
    this->SetTotalValueToDateInUSD(std::move(totalValueToDateInUSD));

    DictionaryEntryVector<PFUint32DictionaryEntry> valuesToDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ValuesToDate", valuesToDate));
    this->SetValuesToDate(std::move(valuesToDate));

    DictionaryEntryVector<PFInt32DictionaryEntry> virtualCurrencyBalances{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VirtualCurrencyBalances", virtualCurrencyBalances));
    this->SetVirtualCurrencyBalances(std::move(virtualCurrencyBalances));

    return S_OK;
}

size_t PlayerProfile::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsPlayerProfile const*> PlayerProfile::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<PlayerProfile>(&this->Model());
}

size_t PlayerProfile::RequiredBufferSize(const PFSegmentsPlayerProfile& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFSegmentsAdCampaignAttribution*) + sizeof(PFSegmentsAdCampaignAttribution*) * model.adCampaignAttributionsCount);
    for (size_t i = 0; i < model.adCampaignAttributionsCount; ++i)
    {
        requiredSize += AdCampaignAttribution::RequiredBufferSize(*model.adCampaignAttributions[i]);
    }
    if (model.avatarUrl)
    {
        requiredSize += (std::strlen(model.avatarUrl) + 1);
    }
    if (model.bannedUntil)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.churnPrediction)
    {
        requiredSize += (alignof(PFSegmentsChurnRiskLevel) + sizeof(PFSegmentsChurnRiskLevel));
    }
    requiredSize += (alignof(PFSegmentsContactEmailInfo*) + sizeof(PFSegmentsContactEmailInfo*) * model.contactEmailAddressesCount);
    for (size_t i = 0; i < model.contactEmailAddressesCount; ++i)
    {
        requiredSize += ContactEmailInfo::RequiredBufferSize(*model.contactEmailAddresses[i]);
    }
    if (model.created)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.displayName)
    {
        requiredSize += (std::strlen(model.displayName) + 1);
    }
    if (model.lastLogin)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    requiredSize += (alignof(PFSegmentsPlayerLinkedAccount*) + sizeof(PFSegmentsPlayerLinkedAccount*) * model.linkedAccountsCount);
    for (size_t i = 0; i < model.linkedAccountsCount; ++i)
    {
        requiredSize += PlayerLinkedAccount::RequiredBufferSize(*model.linkedAccounts[i]);
    }
    requiredSize += (alignof(PFSegmentsPlayerLocationDictionaryEntry) + sizeof(PFSegmentsPlayerLocationDictionaryEntry) * model.locationsCount);
    for (size_t i = 0; i < model.locationsCount; ++i)
    {
        requiredSize += (std::strlen(model.locations[i].key) + 1);
        requiredSize += PlayerLocation::RequiredBufferSize(*model.locations[i].value);
    }
    if (model.origination)
    {
        requiredSize += (alignof(PFLoginIdentityProvider) + sizeof(PFLoginIdentityProvider));
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.playerExperimentVariantsCount);
    for (size_t i = 0; i < model.playerExperimentVariantsCount; ++i)
    {
        requiredSize += (std::strlen(model.playerExperimentVariants[i]) + 1);
    }
    if (model.playerId)
    {
        requiredSize += (std::strlen(model.playerId) + 1);
    }
    requiredSize += (alignof(PFSegmentsPlayerStatistic*) + sizeof(PFSegmentsPlayerStatistic*) * model.playerStatisticsCount);
    for (size_t i = 0; i < model.playerStatisticsCount; ++i)
    {
        requiredSize += PlayerStatistic::RequiredBufferSize(*model.playerStatistics[i]);
    }
    if (model.publisherId)
    {
        requiredSize += (std::strlen(model.publisherId) + 1);
    }
    requiredSize += (alignof(PFSegmentsPushNotificationRegistration*) + sizeof(PFSegmentsPushNotificationRegistration*) * model.pushNotificationRegistrationsCount);
    for (size_t i = 0; i < model.pushNotificationRegistrationsCount; ++i)
    {
        requiredSize += PushNotificationRegistration::RequiredBufferSize(*model.pushNotificationRegistrations[i]);
    }
    requiredSize += (alignof(PFInt32DictionaryEntry) + sizeof(PFInt32DictionaryEntry) * model.statisticsCount);
    for (size_t i = 0; i < model.statisticsCount; ++i)
    {
        requiredSize += (std::strlen(model.statistics[i].key) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    if (model.titleId)
    {
        requiredSize += (std::strlen(model.titleId) + 1);
    }
    if (model.totalValueToDateInUSD)
    {
        requiredSize += (alignof(uint32_t) + sizeof(uint32_t));
    }
    requiredSize += (alignof(PFUint32DictionaryEntry) + sizeof(PFUint32DictionaryEntry) * model.valuesToDateCount);
    for (size_t i = 0; i < model.valuesToDateCount; ++i)
    {
        requiredSize += (std::strlen(model.valuesToDate[i].key) + 1);
    }
    requiredSize += (alignof(PFInt32DictionaryEntry) + sizeof(PFInt32DictionaryEntry) * model.virtualCurrencyBalancesCount);
    for (size_t i = 0; i < model.virtualCurrencyBalancesCount; ++i)
    {
        requiredSize += (std::strlen(model.virtualCurrencyBalances[i].key) + 1);
    }
    return requiredSize;
}

HRESULT PlayerProfile::Copy(const PFSegmentsPlayerProfile& input, PFSegmentsPlayerProfile& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<AdCampaignAttribution>(input.adCampaignAttributions, input.adCampaignAttributionsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.adCampaignAttributions = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.avatarUrl); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.avatarUrl = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.bannedUntil); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.bannedUntil = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.churnPrediction); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.churnPrediction = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<ContactEmailInfo>(input.contactEmailAddresses, input.contactEmailAddressesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.contactEmailAddresses = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.created); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.created = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.displayName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.displayName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.lastLogin); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lastLogin = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<PlayerLinkedAccount>(input.linkedAccounts, input.linkedAccountsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.linkedAccounts = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<PlayerLocation>(input.locations, input.locationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.locations = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.origination); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.origination = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.playerExperimentVariants, input.playerExperimentVariantsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playerExperimentVariants = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.playerId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playerId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<PlayerStatistic>(input.playerStatistics, input.playerStatisticsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playerStatistics = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.publisherId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.publisherId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<PushNotificationRegistration>(input.pushNotificationRegistrations, input.pushNotificationRegistrationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.pushNotificationRegistrations = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.statistics, input.statisticsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.statistics = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.titleId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.totalValueToDateInUSD); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.totalValueToDateInUSD = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.valuesToDate, input.valuesToDateCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.valuesToDate = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.virtualCurrencyBalances, input.virtualCurrencyBalancesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.virtualCurrencyBalances = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetPlayersInSegmentResult::FromJson(const JsonValue& input)
{
    String continuationToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinuationToken", continuationToken));
    this->SetContinuationToken(std::move(continuationToken));

    ModelVector<PlayerProfile> playerProfiles{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<PlayerProfile>(input, "PlayerProfiles", playerProfiles));
    this->SetPlayerProfiles(std::move(playerProfiles));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfilesInSegment", this->m_model.profilesInSegment));

    return S_OK;
}

size_t GetPlayersInSegmentResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsGetPlayersInSegmentResult const*> GetPlayersInSegmentResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetPlayersInSegmentResult>(&this->Model());
}

size_t GetPlayersInSegmentResult::RequiredBufferSize(const PFSegmentsGetPlayersInSegmentResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.continuationToken)
    {
        requiredSize += (std::strlen(model.continuationToken) + 1);
    }
    requiredSize += (alignof(PFSegmentsPlayerProfile*) + sizeof(PFSegmentsPlayerProfile*) * model.playerProfilesCount);
    for (size_t i = 0; i < model.playerProfilesCount; ++i)
    {
        requiredSize += PlayerProfile::RequiredBufferSize(*model.playerProfiles[i]);
    }
    return requiredSize;
}

HRESULT GetPlayersInSegmentResult::Copy(const PFSegmentsGetPlayersInSegmentResult& input, PFSegmentsGetPlayersInSegmentResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.continuationToken); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continuationToken = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<PlayerProfile>(input.playerProfiles, input.playerProfilesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playerProfiles = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue RemovePlayerTagRequest::ToJson() const
{
    return RemovePlayerTagRequest::ToJson(this->Model());
}

JsonValue RemovePlayerTagRequest::ToJson(const PFSegmentsRemovePlayerTagRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMember(output, "TagName", input.tagName);
    return output;
}

} // namespace Segments

// Json serialization helpers

} // namespace PlayFab

#include "stdafx.h"
#include "Types.h"
#include "JsonUtils.h"

namespace PlayFab
{

JsonValue PlayerProfileViewConstraints::ToJson() const
{
    return PlayerProfileViewConstraints::ToJson(this->Model());
}

JsonValue PlayerProfileViewConstraints::ToJson(const PFPlayerProfileViewConstraints& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "ShowAvatarUrl", input.showAvatarUrl);
    JsonUtils::ObjectAddMember(output, "ShowBannedUntil", input.showBannedUntil);
    JsonUtils::ObjectAddMember(output, "ShowCampaignAttributions", input.showCampaignAttributions);
    JsonUtils::ObjectAddMember(output, "ShowContactEmailAddresses", input.showContactEmailAddresses);
    JsonUtils::ObjectAddMember(output, "ShowCreated", input.showCreated);
    JsonUtils::ObjectAddMember(output, "ShowDisplayName", input.showDisplayName);
    JsonUtils::ObjectAddMember(output, "ShowExperimentVariants", input.showExperimentVariants);
    JsonUtils::ObjectAddMember(output, "ShowLastLogin", input.showLastLogin);
    JsonUtils::ObjectAddMember(output, "ShowLinkedAccounts", input.showLinkedAccounts);
    JsonUtils::ObjectAddMember(output, "ShowLocations", input.showLocations);
    JsonUtils::ObjectAddMember(output, "ShowMemberships", input.showMemberships);
    JsonUtils::ObjectAddMember(output, "ShowOrigination", input.showOrigination);
    JsonUtils::ObjectAddMember(output, "ShowPushNotificationRegistrations", input.showPushNotificationRegistrations);
    JsonUtils::ObjectAddMember(output, "ShowStatistics", input.showStatistics);
    JsonUtils::ObjectAddMember(output, "ShowTags", input.showTags);
    JsonUtils::ObjectAddMember(output, "ShowTotalValueToDateInUsd", input.showTotalValueToDateInUsd);
    JsonUtils::ObjectAddMember(output, "ShowValuesToDate", input.showValuesToDate);
    return output;
}

JsonValue GetPlayerCombinedInfoRequestParams::ToJson() const
{
    return GetPlayerCombinedInfoRequestParams::ToJson(this->Model());
}

JsonValue GetPlayerCombinedInfoRequestParams::ToJson(const PFGetPlayerCombinedInfoRequestParams& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "GetCharacterInventories", input.getCharacterInventories);
    JsonUtils::ObjectAddMember(output, "GetCharacterList", input.getCharacterList);
    JsonUtils::ObjectAddMember(output, "GetPlayerProfile", input.getPlayerProfile);
    JsonUtils::ObjectAddMember(output, "GetPlayerStatistics", input.getPlayerStatistics);
    JsonUtils::ObjectAddMember(output, "GetTitleData", input.getTitleData);
    JsonUtils::ObjectAddMember(output, "GetUserAccountInfo", input.getUserAccountInfo);
    JsonUtils::ObjectAddMember(output, "GetUserData", input.getUserData);
    JsonUtils::ObjectAddMember(output, "GetUserInventory", input.getUserInventory);
    JsonUtils::ObjectAddMember(output, "GetUserReadOnlyData", input.getUserReadOnlyData);
    JsonUtils::ObjectAddMember(output, "GetUserVirtualCurrency", input.getUserVirtualCurrency);
    JsonUtils::ObjectAddMemberArray(output, "PlayerStatisticNames", input.playerStatisticNames, input.playerStatisticNamesCount);
    JsonUtils::ObjectAddMember<PlayerProfileViewConstraints>(output, "ProfileConstraints", input.profileConstraints);
    JsonUtils::ObjectAddMemberArray(output, "TitleDataKeys", input.titleDataKeys, input.titleDataKeysCount);
    JsonUtils::ObjectAddMemberArray(output, "UserDataKeys", input.userDataKeys, input.userDataKeysCount);
    JsonUtils::ObjectAddMemberArray(output, "UserReadOnlyDataKeys", input.userReadOnlyDataKeys, input.userReadOnlyDataKeysCount);
    return output;
}

HRESULT UserAndroidDeviceInfo::FromJson(const JsonValue& input)
{
    String androidDeviceId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AndroidDeviceId", androidDeviceId));
    this->SetAndroidDeviceId(std::move(androidDeviceId));

    return S_OK;
}

size_t UserAndroidDeviceInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserAndroidDeviceInfo const*> UserAndroidDeviceInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserAndroidDeviceInfo>(&this->Model());
}

size_t UserAndroidDeviceInfo::RequiredBufferSize(const PFUserAndroidDeviceInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.androidDeviceId)
    {
        requiredSize += (std::strlen(model.androidDeviceId) + 1);
    }
    return requiredSize;
}

HRESULT UserAndroidDeviceInfo::Copy(const PFUserAndroidDeviceInfo& input, PFUserAndroidDeviceInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.androidDeviceId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.androidDeviceId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserAppleIdInfo::FromJson(const JsonValue& input)
{
    String appleSubjectId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AppleSubjectId", appleSubjectId));
    this->SetAppleSubjectId(std::move(appleSubjectId));

    return S_OK;
}

size_t UserAppleIdInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserAppleIdInfo const*> UserAppleIdInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserAppleIdInfo>(&this->Model());
}

size_t UserAppleIdInfo::RequiredBufferSize(const PFUserAppleIdInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.appleSubjectId)
    {
        requiredSize += (std::strlen(model.appleSubjectId) + 1);
    }
    return requiredSize;
}

HRESULT UserAppleIdInfo::Copy(const PFUserAppleIdInfo& input, PFUserAppleIdInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.appleSubjectId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.appleSubjectId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserCustomIdInfo::FromJson(const JsonValue& input)
{
    String customId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CustomId", customId));
    this->SetCustomId(std::move(customId));

    return S_OK;
}

size_t UserCustomIdInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserCustomIdInfo const*> UserCustomIdInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserCustomIdInfo>(&this->Model());
}

size_t UserCustomIdInfo::RequiredBufferSize(const PFUserCustomIdInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.customId)
    {
        requiredSize += (std::strlen(model.customId) + 1);
    }
    return requiredSize;
}

HRESULT UserCustomIdInfo::Copy(const PFUserCustomIdInfo& input, PFUserCustomIdInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.customId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.customId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserFacebookInfo::FromJson(const JsonValue& input)
{
    String facebookId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FacebookId", facebookId));
    this->SetFacebookId(std::move(facebookId));

    String fullName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FullName", fullName));
    this->SetFullName(std::move(fullName));

    return S_OK;
}

size_t UserFacebookInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserFacebookInfo const*> UserFacebookInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserFacebookInfo>(&this->Model());
}

size_t UserFacebookInfo::RequiredBufferSize(const PFUserFacebookInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.facebookId)
    {
        requiredSize += (std::strlen(model.facebookId) + 1);
    }
    if (model.fullName)
    {
        requiredSize += (std::strlen(model.fullName) + 1);
    }
    return requiredSize;
}

HRESULT UserFacebookInfo::Copy(const PFUserFacebookInfo& input, PFUserFacebookInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.facebookId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.facebookId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.fullName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.fullName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserFacebookInstantGamesIdInfo::FromJson(const JsonValue& input)
{
    String facebookInstantGamesId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FacebookInstantGamesId", facebookInstantGamesId));
    this->SetFacebookInstantGamesId(std::move(facebookInstantGamesId));

    return S_OK;
}

size_t UserFacebookInstantGamesIdInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserFacebookInstantGamesIdInfo const*> UserFacebookInstantGamesIdInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserFacebookInstantGamesIdInfo>(&this->Model());
}

size_t UserFacebookInstantGamesIdInfo::RequiredBufferSize(const PFUserFacebookInstantGamesIdInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.facebookInstantGamesId)
    {
        requiredSize += (std::strlen(model.facebookInstantGamesId) + 1);
    }
    return requiredSize;
}

HRESULT UserFacebookInstantGamesIdInfo::Copy(const PFUserFacebookInstantGamesIdInfo& input, PFUserFacebookInstantGamesIdInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.facebookInstantGamesId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.facebookInstantGamesId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserGameCenterInfo::FromJson(const JsonValue& input)
{
    String gameCenterId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GameCenterId", gameCenterId));
    this->SetGameCenterId(std::move(gameCenterId));

    return S_OK;
}

size_t UserGameCenterInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserGameCenterInfo const*> UserGameCenterInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserGameCenterInfo>(&this->Model());
}

size_t UserGameCenterInfo::RequiredBufferSize(const PFUserGameCenterInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.gameCenterId)
    {
        requiredSize += (std::strlen(model.gameCenterId) + 1);
    }
    return requiredSize;
}

HRESULT UserGameCenterInfo::Copy(const PFUserGameCenterInfo& input, PFUserGameCenterInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.gameCenterId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.gameCenterId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserGoogleInfo::FromJson(const JsonValue& input)
{
    String googleEmail{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GoogleEmail", googleEmail));
    this->SetGoogleEmail(std::move(googleEmail));

    String googleGender{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GoogleGender", googleGender));
    this->SetGoogleGender(std::move(googleGender));

    String googleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GoogleId", googleId));
    this->SetGoogleId(std::move(googleId));

    String googleLocale{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GoogleLocale", googleLocale));
    this->SetGoogleLocale(std::move(googleLocale));

    String googleName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GoogleName", googleName));
    this->SetGoogleName(std::move(googleName));

    return S_OK;
}

size_t UserGoogleInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserGoogleInfo const*> UserGoogleInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserGoogleInfo>(&this->Model());
}

size_t UserGoogleInfo::RequiredBufferSize(const PFUserGoogleInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.googleEmail)
    {
        requiredSize += (std::strlen(model.googleEmail) + 1);
    }
    if (model.googleGender)
    {
        requiredSize += (std::strlen(model.googleGender) + 1);
    }
    if (model.googleId)
    {
        requiredSize += (std::strlen(model.googleId) + 1);
    }
    if (model.googleLocale)
    {
        requiredSize += (std::strlen(model.googleLocale) + 1);
    }
    if (model.googleName)
    {
        requiredSize += (std::strlen(model.googleName) + 1);
    }
    return requiredSize;
}

HRESULT UserGoogleInfo::Copy(const PFUserGoogleInfo& input, PFUserGoogleInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.googleEmail); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googleEmail = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.googleGender); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googleGender = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.googleId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.googleLocale); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googleLocale = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.googleName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googleName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserGooglePlayGamesInfo::FromJson(const JsonValue& input)
{
    String googlePlayGamesPlayerAvatarImageUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GooglePlayGamesPlayerAvatarImageUrl", googlePlayGamesPlayerAvatarImageUrl));
    this->SetGooglePlayGamesPlayerAvatarImageUrl(std::move(googlePlayGamesPlayerAvatarImageUrl));

    String googlePlayGamesPlayerDisplayName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GooglePlayGamesPlayerDisplayName", googlePlayGamesPlayerDisplayName));
    this->SetGooglePlayGamesPlayerDisplayName(std::move(googlePlayGamesPlayerDisplayName));

    String googlePlayGamesPlayerId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GooglePlayGamesPlayerId", googlePlayGamesPlayerId));
    this->SetGooglePlayGamesPlayerId(std::move(googlePlayGamesPlayerId));

    return S_OK;
}

size_t UserGooglePlayGamesInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserGooglePlayGamesInfo const*> UserGooglePlayGamesInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserGooglePlayGamesInfo>(&this->Model());
}

size_t UserGooglePlayGamesInfo::RequiredBufferSize(const PFUserGooglePlayGamesInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.googlePlayGamesPlayerAvatarImageUrl)
    {
        requiredSize += (std::strlen(model.googlePlayGamesPlayerAvatarImageUrl) + 1);
    }
    if (model.googlePlayGamesPlayerDisplayName)
    {
        requiredSize += (std::strlen(model.googlePlayGamesPlayerDisplayName) + 1);
    }
    if (model.googlePlayGamesPlayerId)
    {
        requiredSize += (std::strlen(model.googlePlayGamesPlayerId) + 1);
    }
    return requiredSize;
}

HRESULT UserGooglePlayGamesInfo::Copy(const PFUserGooglePlayGamesInfo& input, PFUserGooglePlayGamesInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.googlePlayGamesPlayerAvatarImageUrl); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googlePlayGamesPlayerAvatarImageUrl = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.googlePlayGamesPlayerDisplayName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googlePlayGamesPlayerDisplayName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.googlePlayGamesPlayerId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googlePlayGamesPlayerId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserIosDeviceInfo::FromJson(const JsonValue& input)
{
    String iosDeviceId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IosDeviceId", iosDeviceId));
    this->SetIosDeviceId(std::move(iosDeviceId));

    return S_OK;
}

size_t UserIosDeviceInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserIosDeviceInfo const*> UserIosDeviceInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserIosDeviceInfo>(&this->Model());
}

size_t UserIosDeviceInfo::RequiredBufferSize(const PFUserIosDeviceInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.iosDeviceId)
    {
        requiredSize += (std::strlen(model.iosDeviceId) + 1);
    }
    return requiredSize;
}

HRESULT UserIosDeviceInfo::Copy(const PFUserIosDeviceInfo& input, PFUserIosDeviceInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.iosDeviceId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.iosDeviceId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserKongregateInfo::FromJson(const JsonValue& input)
{
    String kongregateId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "KongregateId", kongregateId));
    this->SetKongregateId(std::move(kongregateId));

    String kongregateName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "KongregateName", kongregateName));
    this->SetKongregateName(std::move(kongregateName));

    return S_OK;
}

size_t UserKongregateInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserKongregateInfo const*> UserKongregateInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserKongregateInfo>(&this->Model());
}

size_t UserKongregateInfo::RequiredBufferSize(const PFUserKongregateInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.kongregateId)
    {
        requiredSize += (std::strlen(model.kongregateId) + 1);
    }
    if (model.kongregateName)
    {
        requiredSize += (std::strlen(model.kongregateName) + 1);
    }
    return requiredSize;
}

HRESULT UserKongregateInfo::Copy(const PFUserKongregateInfo& input, PFUserKongregateInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.kongregateId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.kongregateId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.kongregateName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.kongregateName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserNintendoSwitchAccountIdInfo::FromJson(const JsonValue& input)
{
    String nintendoSwitchAccountSubjectId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NintendoSwitchAccountSubjectId", nintendoSwitchAccountSubjectId));
    this->SetNintendoSwitchAccountSubjectId(std::move(nintendoSwitchAccountSubjectId));

    return S_OK;
}

size_t UserNintendoSwitchAccountIdInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserNintendoSwitchAccountIdInfo const*> UserNintendoSwitchAccountIdInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserNintendoSwitchAccountIdInfo>(&this->Model());
}

size_t UserNintendoSwitchAccountIdInfo::RequiredBufferSize(const PFUserNintendoSwitchAccountIdInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.nintendoSwitchAccountSubjectId)
    {
        requiredSize += (std::strlen(model.nintendoSwitchAccountSubjectId) + 1);
    }
    return requiredSize;
}

HRESULT UserNintendoSwitchAccountIdInfo::Copy(const PFUserNintendoSwitchAccountIdInfo& input, PFUserNintendoSwitchAccountIdInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.nintendoSwitchAccountSubjectId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.nintendoSwitchAccountSubjectId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserNintendoSwitchDeviceIdInfo::FromJson(const JsonValue& input)
{
    String nintendoSwitchDeviceId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NintendoSwitchDeviceId", nintendoSwitchDeviceId));
    this->SetNintendoSwitchDeviceId(std::move(nintendoSwitchDeviceId));

    return S_OK;
}

size_t UserNintendoSwitchDeviceIdInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserNintendoSwitchDeviceIdInfo const*> UserNintendoSwitchDeviceIdInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserNintendoSwitchDeviceIdInfo>(&this->Model());
}

size_t UserNintendoSwitchDeviceIdInfo::RequiredBufferSize(const PFUserNintendoSwitchDeviceIdInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.nintendoSwitchDeviceId)
    {
        requiredSize += (std::strlen(model.nintendoSwitchDeviceId) + 1);
    }
    return requiredSize;
}

HRESULT UserNintendoSwitchDeviceIdInfo::Copy(const PFUserNintendoSwitchDeviceIdInfo& input, PFUserNintendoSwitchDeviceIdInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.nintendoSwitchDeviceId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.nintendoSwitchDeviceId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserOpenIdInfo::FromJson(const JsonValue& input)
{
    String connectionId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ConnectionId", connectionId));
    this->SetConnectionId(std::move(connectionId));

    String issuer{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Issuer", issuer));
    this->SetIssuer(std::move(issuer));

    String subject{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Subject", subject));
    this->SetSubject(std::move(subject));

    return S_OK;
}

size_t UserOpenIdInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserOpenIdInfo const*> UserOpenIdInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserOpenIdInfo>(&this->Model());
}

size_t UserOpenIdInfo::RequiredBufferSize(const PFUserOpenIdInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.connectionId)
    {
        requiredSize += (std::strlen(model.connectionId) + 1);
    }
    if (model.issuer)
    {
        requiredSize += (std::strlen(model.issuer) + 1);
    }
    if (model.subject)
    {
        requiredSize += (std::strlen(model.subject) + 1);
    }
    return requiredSize;
}

HRESULT UserOpenIdInfo::Copy(const PFUserOpenIdInfo& input, PFUserOpenIdInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.connectionId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.connectionId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.issuer); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.issuer = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.subject); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.subject = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserPrivateAccountInfo::FromJson(const JsonValue& input)
{
    String email{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Email", email));
    this->SetEmail(std::move(email));

    return S_OK;
}

size_t UserPrivateAccountInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserPrivateAccountInfo const*> UserPrivateAccountInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserPrivateAccountInfo>(&this->Model());
}

size_t UserPrivateAccountInfo::RequiredBufferSize(const PFUserPrivateAccountInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.email)
    {
        requiredSize += (std::strlen(model.email) + 1);
    }
    return requiredSize;
}

HRESULT UserPrivateAccountInfo::Copy(const PFUserPrivateAccountInfo& input, PFUserPrivateAccountInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.email); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.email = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserPsnInfo::FromJson(const JsonValue& input)
{
    String psnAccountId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PsnAccountId", psnAccountId));
    this->SetPsnAccountId(std::move(psnAccountId));

    String psnOnlineId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PsnOnlineId", psnOnlineId));
    this->SetPsnOnlineId(std::move(psnOnlineId));

    return S_OK;
}

size_t UserPsnInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserPsnInfo const*> UserPsnInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserPsnInfo>(&this->Model());
}

size_t UserPsnInfo::RequiredBufferSize(const PFUserPsnInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.psnAccountId)
    {
        requiredSize += (std::strlen(model.psnAccountId) + 1);
    }
    if (model.psnOnlineId)
    {
        requiredSize += (std::strlen(model.psnOnlineId) + 1);
    }
    return requiredSize;
}

HRESULT UserPsnInfo::Copy(const PFUserPsnInfo& input, PFUserPsnInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.psnAccountId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.psnAccountId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.psnOnlineId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.psnOnlineId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserSteamInfo::FromJson(const JsonValue& input)
{
    std::optional<PFTitleActivationStatus> steamActivationStatus{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SteamActivationStatus", steamActivationStatus));
    this->SetSteamActivationStatus(std::move(steamActivationStatus));

    String steamCountry{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SteamCountry", steamCountry));
    this->SetSteamCountry(std::move(steamCountry));

    std::optional<PFCurrency> steamCurrency{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SteamCurrency", steamCurrency));
    this->SetSteamCurrency(std::move(steamCurrency));

    String steamId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SteamId", steamId));
    this->SetSteamId(std::move(steamId));

    String steamName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SteamName", steamName));
    this->SetSteamName(std::move(steamName));

    return S_OK;
}

size_t UserSteamInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserSteamInfo const*> UserSteamInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserSteamInfo>(&this->Model());
}

size_t UserSteamInfo::RequiredBufferSize(const PFUserSteamInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.steamActivationStatus)
    {
        requiredSize += (alignof(PFTitleActivationStatus) + sizeof(PFTitleActivationStatus));
    }
    if (model.steamCountry)
    {
        requiredSize += (std::strlen(model.steamCountry) + 1);
    }
    if (model.steamCurrency)
    {
        requiredSize += (alignof(PFCurrency) + sizeof(PFCurrency));
    }
    if (model.steamId)
    {
        requiredSize += (std::strlen(model.steamId) + 1);
    }
    if (model.steamName)
    {
        requiredSize += (std::strlen(model.steamName) + 1);
    }
    return requiredSize;
}

HRESULT UserSteamInfo::Copy(const PFUserSteamInfo& input, PFUserSteamInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.steamActivationStatus); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.steamActivationStatus = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.steamCountry); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.steamCountry = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.steamCurrency); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.steamCurrency = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.steamId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.steamId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.steamName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.steamName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue EntityKey::ToJson() const
{
    return EntityKey::ToJson(this->Model());
}

JsonValue EntityKey::ToJson(const PFEntityKey& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    return output;
}

HRESULT EntityKey::FromJson(const JsonValue& input)
{
    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    String type{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", type));
    this->SetType(std::move(type));

    return S_OK;
}

size_t EntityKey::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEntityKey const*> EntityKey::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityKey>(&this->Model());
}

size_t EntityKey::RequiredBufferSize(const PFEntityKey& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.type)
    {
        requiredSize += (std::strlen(model.type) + 1);
    }
    return requiredSize;
}

HRESULT EntityKey::Copy(const PFEntityKey& input, PFEntityKey& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.id); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.type); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.type = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserTitleInfo::FromJson(const JsonValue& input)
{
    String avatarUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AvatarUrl", avatarUrl));
    this->SetAvatarUrl(std::move(avatarUrl));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", this->m_model.created));

    String displayName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisplayName", displayName));
    this->SetDisplayName(std::move(displayName));

    std::optional<time_t> firstLogin{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "FirstLogin", firstLogin));
    this->SetFirstLogin(std::move(firstLogin));

    std::optional<bool> isBanned{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "isBanned", isBanned));
    this->SetisBanned(std::move(isBanned));

    std::optional<time_t> lastLogin{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastLogin", lastLogin));
    this->SetLastLogin(std::move(lastLogin));

    std::optional<PFUserOrigination> origination{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Origination", origination));
    this->SetOrigination(std::move(origination));

    std::optional<EntityKey> titlePlayerAccount{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitlePlayerAccount", titlePlayerAccount));
    if (titlePlayerAccount)
    {
        this->SetTitlePlayerAccount(std::move(*titlePlayerAccount));
    }

    return S_OK;
}

size_t UserTitleInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserTitleInfo const*> UserTitleInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserTitleInfo>(&this->Model());
}

size_t UserTitleInfo::RequiredBufferSize(const PFUserTitleInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.avatarUrl)
    {
        requiredSize += (std::strlen(model.avatarUrl) + 1);
    }
    if (model.displayName)
    {
        requiredSize += (std::strlen(model.displayName) + 1);
    }
    if (model.firstLogin)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.isBanned)
    {
        requiredSize += (alignof(bool) + sizeof(bool));
    }
    if (model.lastLogin)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.origination)
    {
        requiredSize += (alignof(PFUserOrigination) + sizeof(PFUserOrigination));
    }
    if (model.titlePlayerAccount)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.titlePlayerAccount);
    }
    return requiredSize;
}

HRESULT UserTitleInfo::Copy(const PFUserTitleInfo& input, PFUserTitleInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.avatarUrl); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.avatarUrl = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.displayName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.displayName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.firstLogin); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.firstLogin = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.isBanned); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.isBanned = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.lastLogin); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lastLogin = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.origination); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.origination = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.titlePlayerAccount); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titlePlayerAccount = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserTwitchInfo::FromJson(const JsonValue& input)
{
    String twitchId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TwitchId", twitchId));
    this->SetTwitchId(std::move(twitchId));

    String twitchUserName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TwitchUserName", twitchUserName));
    this->SetTwitchUserName(std::move(twitchUserName));

    return S_OK;
}

size_t UserTwitchInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserTwitchInfo const*> UserTwitchInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserTwitchInfo>(&this->Model());
}

size_t UserTwitchInfo::RequiredBufferSize(const PFUserTwitchInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.twitchId)
    {
        requiredSize += (std::strlen(model.twitchId) + 1);
    }
    if (model.twitchUserName)
    {
        requiredSize += (std::strlen(model.twitchUserName) + 1);
    }
    return requiredSize;
}

HRESULT UserTwitchInfo::Copy(const PFUserTwitchInfo& input, PFUserTwitchInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.twitchId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.twitchId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.twitchUserName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.twitchUserName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserXboxInfo::FromJson(const JsonValue& input)
{
    String xboxUserId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "XboxUserId", xboxUserId));
    this->SetXboxUserId(std::move(xboxUserId));

    String xboxUserSandbox{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "XboxUserSandbox", xboxUserSandbox));
    this->SetXboxUserSandbox(std::move(xboxUserSandbox));

    return S_OK;
}

size_t UserXboxInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserXboxInfo const*> UserXboxInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserXboxInfo>(&this->Model());
}

size_t UserXboxInfo::RequiredBufferSize(const PFUserXboxInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.xboxUserId)
    {
        requiredSize += (std::strlen(model.xboxUserId) + 1);
    }
    if (model.xboxUserSandbox)
    {
        requiredSize += (std::strlen(model.xboxUserSandbox) + 1);
    }
    return requiredSize;
}

HRESULT UserXboxInfo::Copy(const PFUserXboxInfo& input, PFUserXboxInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.xboxUserId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.xboxUserId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.xboxUserSandbox); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.xboxUserSandbox = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserAccountInfo::FromJson(const JsonValue& input)
{
    std::optional<UserAndroidDeviceInfo> androidDeviceInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AndroidDeviceInfo", androidDeviceInfo));
    if (androidDeviceInfo)
    {
        this->SetAndroidDeviceInfo(std::move(*androidDeviceInfo));
    }

    std::optional<UserAppleIdInfo> appleAccountInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AppleAccountInfo", appleAccountInfo));
    if (appleAccountInfo)
    {
        this->SetAppleAccountInfo(std::move(*appleAccountInfo));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", this->m_model.created));

    std::optional<UserCustomIdInfo> customIdInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CustomIdInfo", customIdInfo));
    if (customIdInfo)
    {
        this->SetCustomIdInfo(std::move(*customIdInfo));
    }

    std::optional<UserFacebookInfo> facebookInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FacebookInfo", facebookInfo));
    if (facebookInfo)
    {
        this->SetFacebookInfo(std::move(*facebookInfo));
    }

    std::optional<UserFacebookInstantGamesIdInfo> facebookInstantGamesIdInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FacebookInstantGamesIdInfo", facebookInstantGamesIdInfo));
    if (facebookInstantGamesIdInfo)
    {
        this->SetFacebookInstantGamesIdInfo(std::move(*facebookInstantGamesIdInfo));
    }

    std::optional<UserGameCenterInfo> gameCenterInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GameCenterInfo", gameCenterInfo));
    if (gameCenterInfo)
    {
        this->SetGameCenterInfo(std::move(*gameCenterInfo));
    }

    std::optional<UserGoogleInfo> googleInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GoogleInfo", googleInfo));
    if (googleInfo)
    {
        this->SetGoogleInfo(std::move(*googleInfo));
    }

    std::optional<UserGooglePlayGamesInfo> googlePlayGamesInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GooglePlayGamesInfo", googlePlayGamesInfo));
    if (googlePlayGamesInfo)
    {
        this->SetGooglePlayGamesInfo(std::move(*googlePlayGamesInfo));
    }

    std::optional<UserIosDeviceInfo> iosDeviceInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IosDeviceInfo", iosDeviceInfo));
    if (iosDeviceInfo)
    {
        this->SetIosDeviceInfo(std::move(*iosDeviceInfo));
    }

    std::optional<UserKongregateInfo> kongregateInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "KongregateInfo", kongregateInfo));
    if (kongregateInfo)
    {
        this->SetKongregateInfo(std::move(*kongregateInfo));
    }

    std::optional<UserNintendoSwitchAccountIdInfo> nintendoSwitchAccountInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NintendoSwitchAccountInfo", nintendoSwitchAccountInfo));
    if (nintendoSwitchAccountInfo)
    {
        this->SetNintendoSwitchAccountInfo(std::move(*nintendoSwitchAccountInfo));
    }

    std::optional<UserNintendoSwitchDeviceIdInfo> nintendoSwitchDeviceIdInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NintendoSwitchDeviceIdInfo", nintendoSwitchDeviceIdInfo));
    if (nintendoSwitchDeviceIdInfo)
    {
        this->SetNintendoSwitchDeviceIdInfo(std::move(*nintendoSwitchDeviceIdInfo));
    }

    ModelVector<UserOpenIdInfo> openIdInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<UserOpenIdInfo>(input, "OpenIdInfo", openIdInfo));
    this->SetOpenIdInfo(std::move(openIdInfo));

    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    std::optional<UserPrivateAccountInfo> privateInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PrivateInfo", privateInfo));
    if (privateInfo)
    {
        this->SetPrivateInfo(std::move(*privateInfo));
    }

    std::optional<UserPsnInfo> psnInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PsnInfo", psnInfo));
    if (psnInfo)
    {
        this->SetPsnInfo(std::move(*psnInfo));
    }

    std::optional<UserSteamInfo> steamInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SteamInfo", steamInfo));
    if (steamInfo)
    {
        this->SetSteamInfo(std::move(*steamInfo));
    }

    std::optional<UserTitleInfo> titleInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitleInfo", titleInfo));
    if (titleInfo)
    {
        this->SetTitleInfo(std::move(*titleInfo));
    }

    std::optional<UserTwitchInfo> twitchInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TwitchInfo", twitchInfo));
    if (twitchInfo)
    {
        this->SetTwitchInfo(std::move(*twitchInfo));
    }

    String username{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Username", username));
    this->SetUsername(std::move(username));

    std::optional<UserXboxInfo> xboxInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "XboxInfo", xboxInfo));
    if (xboxInfo)
    {
        this->SetXboxInfo(std::move(*xboxInfo));
    }

    return S_OK;
}

size_t UserAccountInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserAccountInfo const*> UserAccountInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserAccountInfo>(&this->Model());
}

size_t UserAccountInfo::RequiredBufferSize(const PFUserAccountInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.androidDeviceInfo)
    {
        requiredSize += UserAndroidDeviceInfo::RequiredBufferSize(*model.androidDeviceInfo);
    }
    if (model.appleAccountInfo)
    {
        requiredSize += UserAppleIdInfo::RequiredBufferSize(*model.appleAccountInfo);
    }
    if (model.customIdInfo)
    {
        requiredSize += UserCustomIdInfo::RequiredBufferSize(*model.customIdInfo);
    }
    if (model.facebookInfo)
    {
        requiredSize += UserFacebookInfo::RequiredBufferSize(*model.facebookInfo);
    }
    if (model.facebookInstantGamesIdInfo)
    {
        requiredSize += UserFacebookInstantGamesIdInfo::RequiredBufferSize(*model.facebookInstantGamesIdInfo);
    }
    if (model.gameCenterInfo)
    {
        requiredSize += UserGameCenterInfo::RequiredBufferSize(*model.gameCenterInfo);
    }
    if (model.googleInfo)
    {
        requiredSize += UserGoogleInfo::RequiredBufferSize(*model.googleInfo);
    }
    if (model.googlePlayGamesInfo)
    {
        requiredSize += UserGooglePlayGamesInfo::RequiredBufferSize(*model.googlePlayGamesInfo);
    }
    if (model.iosDeviceInfo)
    {
        requiredSize += UserIosDeviceInfo::RequiredBufferSize(*model.iosDeviceInfo);
    }
    if (model.kongregateInfo)
    {
        requiredSize += UserKongregateInfo::RequiredBufferSize(*model.kongregateInfo);
    }
    if (model.nintendoSwitchAccountInfo)
    {
        requiredSize += UserNintendoSwitchAccountIdInfo::RequiredBufferSize(*model.nintendoSwitchAccountInfo);
    }
    if (model.nintendoSwitchDeviceIdInfo)
    {
        requiredSize += UserNintendoSwitchDeviceIdInfo::RequiredBufferSize(*model.nintendoSwitchDeviceIdInfo);
    }
    requiredSize += (alignof(PFUserOpenIdInfo*) + sizeof(PFUserOpenIdInfo*) * model.openIdInfoCount);
    for (size_t i = 0; i < model.openIdInfoCount; ++i)
    {
        requiredSize += UserOpenIdInfo::RequiredBufferSize(*model.openIdInfo[i]);
    }
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    if (model.privateInfo)
    {
        requiredSize += UserPrivateAccountInfo::RequiredBufferSize(*model.privateInfo);
    }
    if (model.psnInfo)
    {
        requiredSize += UserPsnInfo::RequiredBufferSize(*model.psnInfo);
    }
    if (model.steamInfo)
    {
        requiredSize += UserSteamInfo::RequiredBufferSize(*model.steamInfo);
    }
    if (model.titleInfo)
    {
        requiredSize += UserTitleInfo::RequiredBufferSize(*model.titleInfo);
    }
    if (model.twitchInfo)
    {
        requiredSize += UserTwitchInfo::RequiredBufferSize(*model.twitchInfo);
    }
    if (model.username)
    {
        requiredSize += (std::strlen(model.username) + 1);
    }
    if (model.xboxInfo)
    {
        requiredSize += UserXboxInfo::RequiredBufferSize(*model.xboxInfo);
    }
    return requiredSize;
}

HRESULT UserAccountInfo::Copy(const PFUserAccountInfo& input, PFUserAccountInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<UserAndroidDeviceInfo>(input.androidDeviceInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.androidDeviceInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserAppleIdInfo>(input.appleAccountInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.appleAccountInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserCustomIdInfo>(input.customIdInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.customIdInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserFacebookInfo>(input.facebookInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.facebookInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserFacebookInstantGamesIdInfo>(input.facebookInstantGamesIdInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.facebookInstantGamesIdInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserGameCenterInfo>(input.gameCenterInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.gameCenterInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserGoogleInfo>(input.googleInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googleInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserGooglePlayGamesInfo>(input.googlePlayGamesInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.googlePlayGamesInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserIosDeviceInfo>(input.iosDeviceInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.iosDeviceInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserKongregateInfo>(input.kongregateInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.kongregateInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserNintendoSwitchAccountIdInfo>(input.nintendoSwitchAccountInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.nintendoSwitchAccountInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserNintendoSwitchDeviceIdInfo>(input.nintendoSwitchDeviceIdInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.nintendoSwitchDeviceIdInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<UserOpenIdInfo>(input.openIdInfo, input.openIdInfoCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.openIdInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserPrivateAccountInfo>(input.privateInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.privateInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserPsnInfo>(input.psnInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.psnInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserSteamInfo>(input.steamInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.steamInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserTitleInfo>(input.titleInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titleInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserTwitchInfo>(input.twitchInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.twitchInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.username); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.username = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserXboxInfo>(input.xboxInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.xboxInfo = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ItemInstance::FromJson(const JsonValue& input)
{
    String annotation{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Annotation", annotation));
    this->SetAnnotation(std::move(annotation));

    CStringVector bundleContents{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "BundleContents", bundleContents));
    this->SetBundleContents(std::move(bundleContents));

    String bundleParent{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "BundleParent", bundleParent));
    this->SetBundleParent(std::move(bundleParent));

    String catalogVersion{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CatalogVersion", catalogVersion));
    this->SetCatalogVersion(std::move(catalogVersion));

    StringDictionaryEntryVector customData{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CustomData", customData));
    this->SetCustomData(std::move(customData));

    String displayName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisplayName", displayName));
    this->SetDisplayName(std::move(displayName));

    std::optional<time_t> expiration{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Expiration", expiration));
    this->SetExpiration(std::move(expiration));

    String itemClass{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemClass", itemClass));
    this->SetItemClass(std::move(itemClass));

    String itemId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemId", itemId));
    this->SetItemId(std::move(itemId));

    String itemInstanceId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemInstanceId", itemInstanceId));
    this->SetItemInstanceId(std::move(itemInstanceId));

    std::optional<time_t> purchaseDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "PurchaseDate", purchaseDate));
    this->SetPurchaseDate(std::move(purchaseDate));

    std::optional<int32_t> remainingUses{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RemainingUses", remainingUses));
    this->SetRemainingUses(std::move(remainingUses));

    String unitCurrency{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UnitCurrency", unitCurrency));
    this->SetUnitCurrency(std::move(unitCurrency));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UnitPrice", this->m_model.unitPrice));

    std::optional<int32_t> usesIncrementedBy{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UsesIncrementedBy", usesIncrementedBy));
    this->SetUsesIncrementedBy(std::move(usesIncrementedBy));

    return S_OK;
}

size_t ItemInstance::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFItemInstance const*> ItemInstance::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ItemInstance>(&this->Model());
}

size_t ItemInstance::RequiredBufferSize(const PFItemInstance& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.annotation)
    {
        requiredSize += (std::strlen(model.annotation) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.bundleContentsCount);
    for (size_t i = 0; i < model.bundleContentsCount; ++i)
    {
        requiredSize += (std::strlen(model.bundleContents[i]) + 1);
    }
    if (model.bundleParent)
    {
        requiredSize += (std::strlen(model.bundleParent) + 1);
    }
    if (model.catalogVersion)
    {
        requiredSize += (std::strlen(model.catalogVersion) + 1);
    }
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.customDataCount);
    for (size_t i = 0; i < model.customDataCount; ++i)
    {
        requiredSize += (std::strlen(model.customData[i].key) + 1);
        requiredSize += (std::strlen(model.customData[i].value) + 1);
    }
    if (model.displayName)
    {
        requiredSize += (std::strlen(model.displayName) + 1);
    }
    if (model.expiration)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.itemClass)
    {
        requiredSize += (std::strlen(model.itemClass) + 1);
    }
    if (model.itemId)
    {
        requiredSize += (std::strlen(model.itemId) + 1);
    }
    if (model.itemInstanceId)
    {
        requiredSize += (std::strlen(model.itemInstanceId) + 1);
    }
    if (model.purchaseDate)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.remainingUses)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.unitCurrency)
    {
        requiredSize += (std::strlen(model.unitCurrency) + 1);
    }
    if (model.usesIncrementedBy)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    return requiredSize;
}

HRESULT ItemInstance::Copy(const PFItemInstance& input, PFItemInstance& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.annotation); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.annotation = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.bundleContents, input.bundleContentsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.bundleContents = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.bundleParent); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.bundleParent = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.catalogVersion); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.catalogVersion = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.customData, input.customDataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.customData = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.displayName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.displayName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.expiration); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.expiration = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.itemClass); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemClass = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.itemId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.itemInstanceId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemInstanceId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.purchaseDate); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.purchaseDate = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.remainingUses); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.remainingUses = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.unitCurrency); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.unitCurrency = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.usesIncrementedBy); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.usesIncrementedBy = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT CharacterInventory::FromJson(const JsonValue& input)
{
    String characterId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterId", characterId));
    this->SetCharacterId(std::move(characterId));

    ModelVector<ItemInstance> inventory{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<ItemInstance>(input, "Inventory", inventory));
    this->SetInventory(std::move(inventory));

    return S_OK;
}

size_t CharacterInventory::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCharacterInventory const*> CharacterInventory::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CharacterInventory>(&this->Model());
}

size_t CharacterInventory::RequiredBufferSize(const PFCharacterInventory& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.characterId)
    {
        requiredSize += (std::strlen(model.characterId) + 1);
    }
    requiredSize += (alignof(PFItemInstance*) + sizeof(PFItemInstance*) * model.inventoryCount);
    for (size_t i = 0; i < model.inventoryCount; ++i)
    {
        requiredSize += ItemInstance::RequiredBufferSize(*model.inventory[i]);
    }
    return requiredSize;
}

HRESULT CharacterInventory::Copy(const PFCharacterInventory& input, PFCharacterInventory& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.characterId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<ItemInstance>(input.inventory, input.inventoryCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.inventory = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT CharacterResult::FromJson(const JsonValue& input)
{
    String characterId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterId", characterId));
    this->SetCharacterId(std::move(characterId));

    String characterName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterName", characterName));
    this->SetCharacterName(std::move(characterName));

    String characterType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterType", characterType));
    this->SetCharacterType(std::move(characterType));

    return S_OK;
}

size_t CharacterResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCharacterResult const*> CharacterResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CharacterResult>(&this->Model());
}

size_t CharacterResult::RequiredBufferSize(const PFCharacterResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.characterId)
    {
        requiredSize += (std::strlen(model.characterId) + 1);
    }
    if (model.characterName)
    {
        requiredSize += (std::strlen(model.characterName) + 1);
    }
    if (model.characterType)
    {
        requiredSize += (std::strlen(model.characterType) + 1);
    }
    return requiredSize;
}

HRESULT CharacterResult::Copy(const PFCharacterResult& input, PFCharacterResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.characterId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.characterName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.characterType); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterType = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue AdCampaignAttributionModel::ToJson() const
{
    return AdCampaignAttributionModel::ToJson(this->Model());
}

JsonValue AdCampaignAttributionModel::ToJson(const PFAdCampaignAttributionModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberTime(output, "AttributedAt", input.attributedAt);
    JsonUtils::ObjectAddMember(output, "CampaignId", input.campaignId);
    JsonUtils::ObjectAddMember(output, "Platform", input.platform);
    return output;
}

HRESULT AdCampaignAttributionModel::FromJson(const JsonValue& input)
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

size_t AdCampaignAttributionModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFAdCampaignAttributionModel const*> AdCampaignAttributionModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<AdCampaignAttributionModel>(&this->Model());
}

size_t AdCampaignAttributionModel::RequiredBufferSize(const PFAdCampaignAttributionModel& model)
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

HRESULT AdCampaignAttributionModel::Copy(const PFAdCampaignAttributionModel& input, PFAdCampaignAttributionModel& output, ModelBuffer& buffer)
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

JsonValue ContactEmailInfoModel::ToJson() const
{
    return ContactEmailInfoModel::ToJson(this->Model());
}

JsonValue ContactEmailInfoModel::ToJson(const PFContactEmailInfoModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "EmailAddress", input.emailAddress);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "VerificationStatus", input.verificationStatus);
    return output;
}

HRESULT ContactEmailInfoModel::FromJson(const JsonValue& input)
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

size_t ContactEmailInfoModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFContactEmailInfoModel const*> ContactEmailInfoModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ContactEmailInfoModel>(&this->Model());
}

size_t ContactEmailInfoModel::RequiredBufferSize(const PFContactEmailInfoModel& model)
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

HRESULT ContactEmailInfoModel::Copy(const PFContactEmailInfoModel& input, PFContactEmailInfoModel& output, ModelBuffer& buffer)
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

JsonValue LinkedPlatformAccountModel::ToJson() const
{
    return LinkedPlatformAccountModel::ToJson(this->Model());
}

JsonValue LinkedPlatformAccountModel::ToJson(const PFLinkedPlatformAccountModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Email", input.email);
    JsonUtils::ObjectAddMember(output, "Platform", input.platform);
    JsonUtils::ObjectAddMember(output, "PlatformUserId", input.platformUserId);
    JsonUtils::ObjectAddMember(output, "Username", input.username);
    return output;
}

HRESULT LinkedPlatformAccountModel::FromJson(const JsonValue& input)
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

size_t LinkedPlatformAccountModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLinkedPlatformAccountModel const*> LinkedPlatformAccountModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<LinkedPlatformAccountModel>(&this->Model());
}

size_t LinkedPlatformAccountModel::RequiredBufferSize(const PFLinkedPlatformAccountModel& model)
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

HRESULT LinkedPlatformAccountModel::Copy(const PFLinkedPlatformAccountModel& input, PFLinkedPlatformAccountModel& output, ModelBuffer& buffer)
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

JsonValue LocationModel::ToJson() const
{
    return LocationModel::ToJson(this->Model());
}

JsonValue LocationModel::ToJson(const PFLocationModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "City", input.city);
    JsonUtils::ObjectAddMember(output, "ContinentCode", input.continentCode);
    JsonUtils::ObjectAddMember(output, "CountryCode", input.countryCode);
    JsonUtils::ObjectAddMember(output, "Latitude", input.latitude);
    JsonUtils::ObjectAddMember(output, "Longitude", input.longitude);
    return output;
}

HRESULT LocationModel::FromJson(const JsonValue& input)
{
    String city{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "City", city));
    this->SetCity(std::move(city));

    std::optional<PFContinentCode> continentCode{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinentCode", continentCode));
    this->SetContinentCode(std::move(continentCode));

    std::optional<PFCountryCode> countryCode{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CountryCode", countryCode));
    this->SetCountryCode(std::move(countryCode));

    std::optional<double> latitude{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Latitude", latitude));
    this->SetLatitude(std::move(latitude));

    std::optional<double> longitude{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Longitude", longitude));
    this->SetLongitude(std::move(longitude));

    return S_OK;
}

size_t LocationModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLocationModel const*> LocationModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<LocationModel>(&this->Model());
}

size_t LocationModel::RequiredBufferSize(const PFLocationModel& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.city)
    {
        requiredSize += (std::strlen(model.city) + 1);
    }
    if (model.continentCode)
    {
        requiredSize += (alignof(PFContinentCode) + sizeof(PFContinentCode));
    }
    if (model.countryCode)
    {
        requiredSize += (alignof(PFCountryCode) + sizeof(PFCountryCode));
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

HRESULT LocationModel::Copy(const PFLocationModel& input, PFLocationModel& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.city); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.city = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.continentCode); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continentCode = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.countryCode); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.countryCode = propCopyResult.ExtractPayload();
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

JsonValue SubscriptionModel::ToJson() const
{
    return SubscriptionModel::ToJson(this->Model());
}

JsonValue SubscriptionModel::ToJson(const PFSubscriptionModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberTime(output, "Expiration", input.expiration);
    JsonUtils::ObjectAddMemberTime(output, "InitialSubscriptionTime", input.initialSubscriptionTime);
    JsonUtils::ObjectAddMember(output, "IsActive", input.isActive);
    JsonUtils::ObjectAddMember(output, "Status", input.status);
    JsonUtils::ObjectAddMember(output, "SubscriptionId", input.subscriptionId);
    JsonUtils::ObjectAddMember(output, "SubscriptionItemId", input.subscriptionItemId);
    JsonUtils::ObjectAddMember(output, "SubscriptionProvider", input.subscriptionProvider);
    return output;
}

HRESULT SubscriptionModel::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Expiration", this->m_model.expiration));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "InitialSubscriptionTime", this->m_model.initialSubscriptionTime));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsActive", this->m_model.isActive));

    std::optional<PFSubscriptionProviderStatus> status{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Status", status));
    this->SetStatus(std::move(status));

    String subscriptionId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SubscriptionId", subscriptionId));
    this->SetSubscriptionId(std::move(subscriptionId));

    String subscriptionItemId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SubscriptionItemId", subscriptionItemId));
    this->SetSubscriptionItemId(std::move(subscriptionItemId));

    String subscriptionProvider{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SubscriptionProvider", subscriptionProvider));
    this->SetSubscriptionProvider(std::move(subscriptionProvider));

    return S_OK;
}

size_t SubscriptionModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSubscriptionModel const*> SubscriptionModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SubscriptionModel>(&this->Model());
}

size_t SubscriptionModel::RequiredBufferSize(const PFSubscriptionModel& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.status)
    {
        requiredSize += (alignof(PFSubscriptionProviderStatus) + sizeof(PFSubscriptionProviderStatus));
    }
    if (model.subscriptionId)
    {
        requiredSize += (std::strlen(model.subscriptionId) + 1);
    }
    if (model.subscriptionItemId)
    {
        requiredSize += (std::strlen(model.subscriptionItemId) + 1);
    }
    if (model.subscriptionProvider)
    {
        requiredSize += (std::strlen(model.subscriptionProvider) + 1);
    }
    return requiredSize;
}

HRESULT SubscriptionModel::Copy(const PFSubscriptionModel& input, PFSubscriptionModel& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.status); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.status = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.subscriptionId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.subscriptionId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.subscriptionItemId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.subscriptionItemId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.subscriptionProvider); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.subscriptionProvider = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue MembershipModel::ToJson() const
{
    return MembershipModel::ToJson(this->Model());
}

JsonValue MembershipModel::ToJson(const PFMembershipModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "IsActive", input.isActive);
    JsonUtils::ObjectAddMemberTime(output, "MembershipExpiration", input.membershipExpiration);
    JsonUtils::ObjectAddMember(output, "MembershipId", input.membershipId);
    JsonUtils::ObjectAddMemberTime(output, "OverrideExpiration", input.overrideExpiration);
    JsonUtils::ObjectAddMemberArray<SubscriptionModel>(output, "Subscriptions", input.subscriptions, input.subscriptionsCount);
    return output;
}

HRESULT MembershipModel::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsActive", this->m_model.isActive));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "MembershipExpiration", this->m_model.membershipExpiration));

    String membershipId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MembershipId", membershipId));
    this->SetMembershipId(std::move(membershipId));

    std::optional<time_t> overrideExpiration{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "OverrideExpiration", overrideExpiration));
    this->SetOverrideExpiration(std::move(overrideExpiration));

    ModelVector<SubscriptionModel> subscriptions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<SubscriptionModel>(input, "Subscriptions", subscriptions));
    this->SetSubscriptions(std::move(subscriptions));

    return S_OK;
}

size_t MembershipModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMembershipModel const*> MembershipModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<MembershipModel>(&this->Model());
}

size_t MembershipModel::RequiredBufferSize(const PFMembershipModel& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.membershipId)
    {
        requiredSize += (std::strlen(model.membershipId) + 1);
    }
    if (model.overrideExpiration)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    requiredSize += (alignof(PFSubscriptionModel*) + sizeof(PFSubscriptionModel*) * model.subscriptionsCount);
    for (size_t i = 0; i < model.subscriptionsCount; ++i)
    {
        requiredSize += SubscriptionModel::RequiredBufferSize(*model.subscriptions[i]);
    }
    return requiredSize;
}

HRESULT MembershipModel::Copy(const PFMembershipModel& input, PFMembershipModel& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.membershipId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.membershipId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.overrideExpiration); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.overrideExpiration = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<SubscriptionModel>(input.subscriptions, input.subscriptionsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.subscriptions = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue PushNotificationRegistrationModel::ToJson() const
{
    return PushNotificationRegistrationModel::ToJson(this->Model());
}

JsonValue PushNotificationRegistrationModel::ToJson(const PFPushNotificationRegistrationModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "NotificationEndpointARN", input.notificationEndpointARN);
    JsonUtils::ObjectAddMember(output, "Platform", input.platform);
    return output;
}

HRESULT PushNotificationRegistrationModel::FromJson(const JsonValue& input)
{
    String notificationEndpointARN{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NotificationEndpointARN", notificationEndpointARN));
    this->SetNotificationEndpointARN(std::move(notificationEndpointARN));

    std::optional<PFPushNotificationPlatform> platform{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Platform", platform));
    this->SetPlatform(std::move(platform));

    return S_OK;
}

size_t PushNotificationRegistrationModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPushNotificationRegistrationModel const*> PushNotificationRegistrationModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<PushNotificationRegistrationModel>(&this->Model());
}

size_t PushNotificationRegistrationModel::RequiredBufferSize(const PFPushNotificationRegistrationModel& model)
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

HRESULT PushNotificationRegistrationModel::Copy(const PFPushNotificationRegistrationModel& input, PFPushNotificationRegistrationModel& output, ModelBuffer& buffer)
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

JsonValue StatisticModel::ToJson() const
{
    return StatisticModel::ToJson(this->Model());
}

JsonValue StatisticModel::ToJson(const PFStatisticModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "Value", input.value);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

HRESULT StatisticModel::FromJson(const JsonValue& input)
{
    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Value", this->m_model.value));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    return S_OK;
}

size_t StatisticModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticModel const*> StatisticModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<StatisticModel>(&this->Model());
}

size_t StatisticModel::RequiredBufferSize(const PFStatisticModel& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT StatisticModel::Copy(const PFStatisticModel& input, PFStatisticModel& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue TagModel::ToJson() const
{
    return TagModel::ToJson(this->Model());
}

JsonValue TagModel::ToJson(const PFTagModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "TagValue", input.tagValue);
    return output;
}

HRESULT TagModel::FromJson(const JsonValue& input)
{
    String tagValue{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TagValue", tagValue));
    this->SetTagValue(std::move(tagValue));

    return S_OK;
}

size_t TagModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFTagModel const*> TagModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TagModel>(&this->Model());
}

size_t TagModel::RequiredBufferSize(const PFTagModel& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.tagValue)
    {
        requiredSize += (std::strlen(model.tagValue) + 1);
    }
    return requiredSize;
}

HRESULT TagModel::Copy(const PFTagModel& input, PFTagModel& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.tagValue); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tagValue = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ValueToDateModel::ToJson() const
{
    return ValueToDateModel::ToJson(this->Model());
}

JsonValue ValueToDateModel::ToJson(const PFValueToDateModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Currency", input.currency);
    JsonUtils::ObjectAddMember(output, "TotalValue", input.totalValue);
    JsonUtils::ObjectAddMember(output, "TotalValueAsDecimal", input.totalValueAsDecimal);
    return output;
}

HRESULT ValueToDateModel::FromJson(const JsonValue& input)
{
    String currency{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Currency", currency));
    this->SetCurrency(std::move(currency));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TotalValue", this->m_model.totalValue));

    String totalValueAsDecimal{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TotalValueAsDecimal", totalValueAsDecimal));
    this->SetTotalValueAsDecimal(std::move(totalValueAsDecimal));

    return S_OK;
}

size_t ValueToDateModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFValueToDateModel const*> ValueToDateModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ValueToDateModel>(&this->Model());
}

size_t ValueToDateModel::RequiredBufferSize(const PFValueToDateModel& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.currency)
    {
        requiredSize += (std::strlen(model.currency) + 1);
    }
    if (model.totalValueAsDecimal)
    {
        requiredSize += (std::strlen(model.totalValueAsDecimal) + 1);
    }
    return requiredSize;
}

HRESULT ValueToDateModel::Copy(const PFValueToDateModel& input, PFValueToDateModel& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.currency); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.currency = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.totalValueAsDecimal); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.totalValueAsDecimal = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue PlayerProfileModel::ToJson() const
{
    return PlayerProfileModel::ToJson(this->Model());
}

JsonValue PlayerProfileModel::ToJson(const PFPlayerProfileModel& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberArray<AdCampaignAttributionModel>(output, "AdCampaignAttributions", input.adCampaignAttributions, input.adCampaignAttributionsCount);
    JsonUtils::ObjectAddMember(output, "AvatarUrl", input.avatarUrl);
    JsonUtils::ObjectAddMemberTime(output, "BannedUntil", input.bannedUntil);
    JsonUtils::ObjectAddMemberArray<ContactEmailInfoModel>(output, "ContactEmailAddresses", input.contactEmailAddresses, input.contactEmailAddressesCount);
    JsonUtils::ObjectAddMemberTime(output, "Created", input.created);
    JsonUtils::ObjectAddMember(output, "DisplayName", input.displayName);
    JsonUtils::ObjectAddMemberArray(output, "ExperimentVariants", input.experimentVariants, input.experimentVariantsCount);
    JsonUtils::ObjectAddMemberTime(output, "LastLogin", input.lastLogin);
    JsonUtils::ObjectAddMemberArray<LinkedPlatformAccountModel>(output, "LinkedAccounts", input.linkedAccounts, input.linkedAccountsCount);
    JsonUtils::ObjectAddMemberArray<LocationModel>(output, "Locations", input.locations, input.locationsCount);
    JsonUtils::ObjectAddMemberArray<MembershipModel>(output, "Memberships", input.memberships, input.membershipsCount);
    JsonUtils::ObjectAddMember(output, "Origination", input.origination);
    JsonUtils::ObjectAddMember(output, "PlayerId", input.playerId);
    JsonUtils::ObjectAddMember(output, "PublisherId", input.publisherId);
    JsonUtils::ObjectAddMemberArray<PushNotificationRegistrationModel>(output, "PushNotificationRegistrations", input.pushNotificationRegistrations, input.pushNotificationRegistrationsCount);
    JsonUtils::ObjectAddMemberArray<StatisticModel>(output, "Statistics", input.statistics, input.statisticsCount);
    JsonUtils::ObjectAddMemberArray<TagModel>(output, "Tags", input.tags, input.tagsCount);
    JsonUtils::ObjectAddMember(output, "TitleId", input.titleId);
    JsonUtils::ObjectAddMember(output, "TotalValueToDateInUSD", input.totalValueToDateInUSD);
    JsonUtils::ObjectAddMemberArray<ValueToDateModel>(output, "ValuesToDate", input.valuesToDate, input.valuesToDateCount);
    return output;
}

HRESULT PlayerProfileModel::FromJson(const JsonValue& input)
{
    ModelVector<AdCampaignAttributionModel> adCampaignAttributions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<AdCampaignAttributionModel>(input, "AdCampaignAttributions", adCampaignAttributions));
    this->SetAdCampaignAttributions(std::move(adCampaignAttributions));

    String avatarUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AvatarUrl", avatarUrl));
    this->SetAvatarUrl(std::move(avatarUrl));

    std::optional<time_t> bannedUntil{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "BannedUntil", bannedUntil));
    this->SetBannedUntil(std::move(bannedUntil));

    ModelVector<ContactEmailInfoModel> contactEmailAddresses{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<ContactEmailInfoModel>(input, "ContactEmailAddresses", contactEmailAddresses));
    this->SetContactEmailAddresses(std::move(contactEmailAddresses));

    std::optional<time_t> created{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", created));
    this->SetCreated(std::move(created));

    String displayName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisplayName", displayName));
    this->SetDisplayName(std::move(displayName));

    CStringVector experimentVariants{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ExperimentVariants", experimentVariants));
    this->SetExperimentVariants(std::move(experimentVariants));

    std::optional<time_t> lastLogin{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastLogin", lastLogin));
    this->SetLastLogin(std::move(lastLogin));

    ModelVector<LinkedPlatformAccountModel> linkedAccounts{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<LinkedPlatformAccountModel>(input, "LinkedAccounts", linkedAccounts));
    this->SetLinkedAccounts(std::move(linkedAccounts));

    ModelVector<LocationModel> locations{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<LocationModel>(input, "Locations", locations));
    this->SetLocations(std::move(locations));

    ModelVector<MembershipModel> memberships{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<MembershipModel>(input, "Memberships", memberships));
    this->SetMemberships(std::move(memberships));

    std::optional<PFLoginIdentityProvider> origination{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Origination", origination));
    this->SetOrigination(std::move(origination));

    String playerId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayerId", playerId));
    this->SetPlayerId(std::move(playerId));

    String publisherId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PublisherId", publisherId));
    this->SetPublisherId(std::move(publisherId));

    ModelVector<PushNotificationRegistrationModel> pushNotificationRegistrations{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<PushNotificationRegistrationModel>(input, "PushNotificationRegistrations", pushNotificationRegistrations));
    this->SetPushNotificationRegistrations(std::move(pushNotificationRegistrations));

    ModelVector<StatisticModel> statistics{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<StatisticModel>(input, "Statistics", statistics));
    this->SetStatistics(std::move(statistics));

    ModelVector<TagModel> tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<TagModel>(input, "Tags", tags));
    this->SetTags(std::move(tags));

    String titleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitleId", titleId));
    this->SetTitleId(std::move(titleId));

    std::optional<uint32_t> totalValueToDateInUSD{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TotalValueToDateInUSD", totalValueToDateInUSD));
    this->SetTotalValueToDateInUSD(std::move(totalValueToDateInUSD));

    ModelVector<ValueToDateModel> valuesToDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<ValueToDateModel>(input, "ValuesToDate", valuesToDate));
    this->SetValuesToDate(std::move(valuesToDate));

    return S_OK;
}

size_t PlayerProfileModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerProfileModel const*> PlayerProfileModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<PlayerProfileModel>(&this->Model());
}

size_t PlayerProfileModel::RequiredBufferSize(const PFPlayerProfileModel& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFAdCampaignAttributionModel*) + sizeof(PFAdCampaignAttributionModel*) * model.adCampaignAttributionsCount);
    for (size_t i = 0; i < model.adCampaignAttributionsCount; ++i)
    {
        requiredSize += AdCampaignAttributionModel::RequiredBufferSize(*model.adCampaignAttributions[i]);
    }
    if (model.avatarUrl)
    {
        requiredSize += (std::strlen(model.avatarUrl) + 1);
    }
    if (model.bannedUntil)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    requiredSize += (alignof(PFContactEmailInfoModel*) + sizeof(PFContactEmailInfoModel*) * model.contactEmailAddressesCount);
    for (size_t i = 0; i < model.contactEmailAddressesCount; ++i)
    {
        requiredSize += ContactEmailInfoModel::RequiredBufferSize(*model.contactEmailAddresses[i]);
    }
    if (model.created)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.displayName)
    {
        requiredSize += (std::strlen(model.displayName) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.experimentVariantsCount);
    for (size_t i = 0; i < model.experimentVariantsCount; ++i)
    {
        requiredSize += (std::strlen(model.experimentVariants[i]) + 1);
    }
    if (model.lastLogin)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    requiredSize += (alignof(PFLinkedPlatformAccountModel*) + sizeof(PFLinkedPlatformAccountModel*) * model.linkedAccountsCount);
    for (size_t i = 0; i < model.linkedAccountsCount; ++i)
    {
        requiredSize += LinkedPlatformAccountModel::RequiredBufferSize(*model.linkedAccounts[i]);
    }
    requiredSize += (alignof(PFLocationModel*) + sizeof(PFLocationModel*) * model.locationsCount);
    for (size_t i = 0; i < model.locationsCount; ++i)
    {
        requiredSize += LocationModel::RequiredBufferSize(*model.locations[i]);
    }
    requiredSize += (alignof(PFMembershipModel*) + sizeof(PFMembershipModel*) * model.membershipsCount);
    for (size_t i = 0; i < model.membershipsCount; ++i)
    {
        requiredSize += MembershipModel::RequiredBufferSize(*model.memberships[i]);
    }
    if (model.origination)
    {
        requiredSize += (alignof(PFLoginIdentityProvider) + sizeof(PFLoginIdentityProvider));
    }
    if (model.playerId)
    {
        requiredSize += (std::strlen(model.playerId) + 1);
    }
    if (model.publisherId)
    {
        requiredSize += (std::strlen(model.publisherId) + 1);
    }
    requiredSize += (alignof(PFPushNotificationRegistrationModel*) + sizeof(PFPushNotificationRegistrationModel*) * model.pushNotificationRegistrationsCount);
    for (size_t i = 0; i < model.pushNotificationRegistrationsCount; ++i)
    {
        requiredSize += PushNotificationRegistrationModel::RequiredBufferSize(*model.pushNotificationRegistrations[i]);
    }
    requiredSize += (alignof(PFStatisticModel*) + sizeof(PFStatisticModel*) * model.statisticsCount);
    for (size_t i = 0; i < model.statisticsCount; ++i)
    {
        requiredSize += StatisticModel::RequiredBufferSize(*model.statistics[i]);
    }
    requiredSize += (alignof(PFTagModel*) + sizeof(PFTagModel*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += TagModel::RequiredBufferSize(*model.tags[i]);
    }
    if (model.titleId)
    {
        requiredSize += (std::strlen(model.titleId) + 1);
    }
    if (model.totalValueToDateInUSD)
    {
        requiredSize += (alignof(uint32_t) + sizeof(uint32_t));
    }
    requiredSize += (alignof(PFValueToDateModel*) + sizeof(PFValueToDateModel*) * model.valuesToDateCount);
    for (size_t i = 0; i < model.valuesToDateCount; ++i)
    {
        requiredSize += ValueToDateModel::RequiredBufferSize(*model.valuesToDate[i]);
    }
    return requiredSize;
}

HRESULT PlayerProfileModel::Copy(const PFPlayerProfileModel& input, PFPlayerProfileModel& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<AdCampaignAttributionModel>(input.adCampaignAttributions, input.adCampaignAttributionsCount);
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
        auto propCopyResult = buffer.CopyToArray<ContactEmailInfoModel>(input.contactEmailAddresses, input.contactEmailAddressesCount);
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
        auto propCopyResult = buffer.CopyToArray(input.experimentVariants, input.experimentVariantsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.experimentVariants = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.lastLogin); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lastLogin = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<LinkedPlatformAccountModel>(input.linkedAccounts, input.linkedAccountsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.linkedAccounts = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<LocationModel>(input.locations, input.locationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.locations = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<MembershipModel>(input.memberships, input.membershipsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.memberships = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.origination); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.origination = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.playerId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playerId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.publisherId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.publisherId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<PushNotificationRegistrationModel>(input.pushNotificationRegistrations, input.pushNotificationRegistrationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.pushNotificationRegistrations = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<StatisticModel>(input.statistics, input.statisticsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.statistics = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<TagModel>(input.tags, input.tagsCount);
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
        auto propCopyResult = buffer.CopyToArray<ValueToDateModel>(input.valuesToDate, input.valuesToDateCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.valuesToDate = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT StatisticValue::FromJson(const JsonValue& input)
{
    String statisticName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StatisticName", statisticName));
    this->SetStatisticName(std::move(statisticName));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Value", this->m_model.value));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    return S_OK;
}

size_t StatisticValue::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticValue const*> StatisticValue::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<StatisticValue>(&this->Model());
}

size_t StatisticValue::RequiredBufferSize(const PFStatisticValue& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.statisticName)
    {
        requiredSize += (std::strlen(model.statisticName) + 1);
    }
    return requiredSize;
}

HRESULT StatisticValue::Copy(const PFStatisticValue& input, PFStatisticValue& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.statisticName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.statisticName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT UserDataRecord::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastUpdated", this->m_model.lastUpdated));

    std::optional<PFUserDataPermission> permission{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Permission", permission));
    this->SetPermission(std::move(permission));

    String value{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Value", value));
    this->SetValue(std::move(value));

    return S_OK;
}

size_t UserDataRecord::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFUserDataRecord const*> UserDataRecord::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserDataRecord>(&this->Model());
}

size_t UserDataRecord::RequiredBufferSize(const PFUserDataRecord& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.permission)
    {
        requiredSize += (alignof(PFUserDataPermission) + sizeof(PFUserDataPermission));
    }
    if (model.value)
    {
        requiredSize += (std::strlen(model.value) + 1);
    }
    return requiredSize;
}

HRESULT UserDataRecord::Copy(const PFUserDataRecord& input, PFUserDataRecord& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.permission); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.permission = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.value); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.value = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT VirtualCurrencyRechargeTime::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RechargeMax", this->m_model.rechargeMax));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "RechargeTime", this->m_model.rechargeTime));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SecondsToRecharge", this->m_model.secondsToRecharge));

    return S_OK;
}

size_t VirtualCurrencyRechargeTime::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFVirtualCurrencyRechargeTime const*> VirtualCurrencyRechargeTime::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<VirtualCurrencyRechargeTime>(&this->Model());
}

size_t VirtualCurrencyRechargeTime::RequiredBufferSize(const PFVirtualCurrencyRechargeTime& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT VirtualCurrencyRechargeTime::Copy(const PFVirtualCurrencyRechargeTime& input, PFVirtualCurrencyRechargeTime& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

HRESULT GetPlayerCombinedInfoResultPayload::FromJson(const JsonValue& input)
{
    std::optional<UserAccountInfo> accountInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AccountInfo", accountInfo));
    if (accountInfo)
    {
        this->SetAccountInfo(std::move(*accountInfo));
    }

    ModelVector<CharacterInventory> characterInventories{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CharacterInventory>(input, "CharacterInventories", characterInventories));
    this->SetCharacterInventories(std::move(characterInventories));

    ModelVector<CharacterResult> characterList{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CharacterResult>(input, "CharacterList", characterList));
    this->SetCharacterList(std::move(characterList));

    std::optional<PlayerProfileModel> playerProfile{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayerProfile", playerProfile));
    if (playerProfile)
    {
        this->SetPlayerProfile(std::move(*playerProfile));
    }

    ModelVector<StatisticValue> playerStatistics{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<StatisticValue>(input, "PlayerStatistics", playerStatistics));
    this->SetPlayerStatistics(std::move(playerStatistics));

    StringDictionaryEntryVector titleData{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitleData", titleData));
    this->SetTitleData(std::move(titleData));

    ModelDictionaryEntryVector<UserDataRecord> userData{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<UserDataRecord>(input, "UserData", userData));
    this->SetUserData(std::move(userData));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UserDataVersion", this->m_model.userDataVersion));

    ModelVector<ItemInstance> userInventory{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<ItemInstance>(input, "UserInventory", userInventory));
    this->SetUserInventory(std::move(userInventory));

    ModelDictionaryEntryVector<UserDataRecord> userReadOnlyData{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<UserDataRecord>(input, "UserReadOnlyData", userReadOnlyData));
    this->SetUserReadOnlyData(std::move(userReadOnlyData));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UserReadOnlyDataVersion", this->m_model.userReadOnlyDataVersion));

    DictionaryEntryVector<PFInt32DictionaryEntry> userVirtualCurrency{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UserVirtualCurrency", userVirtualCurrency));
    this->SetUserVirtualCurrency(std::move(userVirtualCurrency));

    ModelDictionaryEntryVector<VirtualCurrencyRechargeTime> userVirtualCurrencyRechargeTimes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<VirtualCurrencyRechargeTime>(input, "UserVirtualCurrencyRechargeTimes", userVirtualCurrencyRechargeTimes));
    this->SetUserVirtualCurrencyRechargeTimes(std::move(userVirtualCurrencyRechargeTimes));

    return S_OK;
}

size_t GetPlayerCombinedInfoResultPayload::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGetPlayerCombinedInfoResultPayload const*> GetPlayerCombinedInfoResultPayload::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetPlayerCombinedInfoResultPayload>(&this->Model());
}

size_t GetPlayerCombinedInfoResultPayload::RequiredBufferSize(const PFGetPlayerCombinedInfoResultPayload& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.accountInfo)
    {
        requiredSize += UserAccountInfo::RequiredBufferSize(*model.accountInfo);
    }
    requiredSize += (alignof(PFCharacterInventory*) + sizeof(PFCharacterInventory*) * model.characterInventoriesCount);
    for (size_t i = 0; i < model.characterInventoriesCount; ++i)
    {
        requiredSize += CharacterInventory::RequiredBufferSize(*model.characterInventories[i]);
    }
    requiredSize += (alignof(PFCharacterResult*) + sizeof(PFCharacterResult*) * model.characterListCount);
    for (size_t i = 0; i < model.characterListCount; ++i)
    {
        requiredSize += CharacterResult::RequiredBufferSize(*model.characterList[i]);
    }
    if (model.playerProfile)
    {
        requiredSize += PlayerProfileModel::RequiredBufferSize(*model.playerProfile);
    }
    requiredSize += (alignof(PFStatisticValue*) + sizeof(PFStatisticValue*) * model.playerStatisticsCount);
    for (size_t i = 0; i < model.playerStatisticsCount; ++i)
    {
        requiredSize += StatisticValue::RequiredBufferSize(*model.playerStatistics[i]);
    }
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.titleDataCount);
    for (size_t i = 0; i < model.titleDataCount; ++i)
    {
        requiredSize += (std::strlen(model.titleData[i].key) + 1);
        requiredSize += (std::strlen(model.titleData[i].value) + 1);
    }
    requiredSize += (alignof(PFUserDataRecordDictionaryEntry) + sizeof(PFUserDataRecordDictionaryEntry) * model.userDataCount);
    for (size_t i = 0; i < model.userDataCount; ++i)
    {
        requiredSize += (std::strlen(model.userData[i].key) + 1);
        requiredSize += UserDataRecord::RequiredBufferSize(*model.userData[i].value);
    }
    requiredSize += (alignof(PFItemInstance*) + sizeof(PFItemInstance*) * model.userInventoryCount);
    for (size_t i = 0; i < model.userInventoryCount; ++i)
    {
        requiredSize += ItemInstance::RequiredBufferSize(*model.userInventory[i]);
    }
    requiredSize += (alignof(PFUserDataRecordDictionaryEntry) + sizeof(PFUserDataRecordDictionaryEntry) * model.userReadOnlyDataCount);
    for (size_t i = 0; i < model.userReadOnlyDataCount; ++i)
    {
        requiredSize += (std::strlen(model.userReadOnlyData[i].key) + 1);
        requiredSize += UserDataRecord::RequiredBufferSize(*model.userReadOnlyData[i].value);
    }
    requiredSize += (alignof(PFInt32DictionaryEntry) + sizeof(PFInt32DictionaryEntry) * model.userVirtualCurrencyCount);
    for (size_t i = 0; i < model.userVirtualCurrencyCount; ++i)
    {
        requiredSize += (std::strlen(model.userVirtualCurrency[i].key) + 1);
    }
    requiredSize += (alignof(PFVirtualCurrencyRechargeTimeDictionaryEntry) + sizeof(PFVirtualCurrencyRechargeTimeDictionaryEntry) * model.userVirtualCurrencyRechargeTimesCount);
    for (size_t i = 0; i < model.userVirtualCurrencyRechargeTimesCount; ++i)
    {
        requiredSize += (std::strlen(model.userVirtualCurrencyRechargeTimes[i].key) + 1);
        requiredSize += VirtualCurrencyRechargeTime::RequiredBufferSize(*model.userVirtualCurrencyRechargeTimes[i].value);
    }
    return requiredSize;
}

HRESULT GetPlayerCombinedInfoResultPayload::Copy(const PFGetPlayerCombinedInfoResultPayload& input, PFGetPlayerCombinedInfoResultPayload& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<UserAccountInfo>(input.accountInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.accountInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<CharacterInventory>(input.characterInventories, input.characterInventoriesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterInventories = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<CharacterResult>(input.characterList, input.characterListCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterList = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<PlayerProfileModel>(input.playerProfile); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playerProfile = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<StatisticValue>(input.playerStatistics, input.playerStatisticsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playerStatistics = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.titleData, input.titleDataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titleData = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<UserDataRecord>(input.userData, input.userDataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.userData = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<ItemInstance>(input.userInventory, input.userInventoryCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.userInventory = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<UserDataRecord>(input.userReadOnlyData, input.userReadOnlyDataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.userReadOnlyData = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.userVirtualCurrency, input.userVirtualCurrencyCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.userVirtualCurrency = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<VirtualCurrencyRechargeTime>(input.userVirtualCurrencyRechargeTimes, input.userVirtualCurrencyRechargeTimesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.userVirtualCurrencyRechargeTimes = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue Variable::ToJson() const
{
    return Variable::ToJson(this->Model());
}

JsonValue Variable::ToJson(const PFVariable& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "Value", input.value);
    return output;
}

HRESULT Variable::FromJson(const JsonValue& input)
{
    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    String value{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Value", value));
    this->SetValue(std::move(value));

    return S_OK;
}

size_t Variable::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFVariable const*> Variable::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Variable>(&this->Model());
}

size_t Variable::RequiredBufferSize(const PFVariable& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    if (model.value)
    {
        requiredSize += (std::strlen(model.value) + 1);
    }
    return requiredSize;
}

HRESULT Variable::Copy(const PFVariable& input, PFVariable& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.value); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.value = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT TreatmentAssignment::FromJson(const JsonValue& input)
{
    ModelVector<Variable> variables{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<Variable>(input, "Variables", variables));
    this->SetVariables(std::move(variables));

    CStringVector variants{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Variants", variants));
    this->SetVariants(std::move(variants));

    return S_OK;
}

size_t TreatmentAssignment::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFTreatmentAssignment const*> TreatmentAssignment::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TreatmentAssignment>(&this->Model());
}

size_t TreatmentAssignment::RequiredBufferSize(const PFTreatmentAssignment& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFVariable*) + sizeof(PFVariable*) * model.variablesCount);
    for (size_t i = 0; i < model.variablesCount; ++i)
    {
        requiredSize += Variable::RequiredBufferSize(*model.variables[i]);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.variantsCount);
    for (size_t i = 0; i < model.variantsCount; ++i)
    {
        requiredSize += (std::strlen(model.variants[i]) + 1);
    }
    return requiredSize;
}

HRESULT TreatmentAssignment::Copy(const PFTreatmentAssignment& input, PFTreatmentAssignment& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<Variable>(input.variables, input.variablesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.variables = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.variants, input.variantsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.variants = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT EntityLineage::FromJson(const JsonValue& input)
{
    String characterId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterId", characterId));
    this->SetCharacterId(std::move(characterId));

    String groupId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GroupId", groupId));
    this->SetGroupId(std::move(groupId));

    String masterPlayerAccountId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MasterPlayerAccountId", masterPlayerAccountId));
    this->SetMasterPlayerAccountId(std::move(masterPlayerAccountId));

    String namespaceId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NamespaceId", namespaceId));
    this->SetNamespaceId(std::move(namespaceId));

    String titleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitleId", titleId));
    this->SetTitleId(std::move(titleId));

    String titlePlayerAccountId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitlePlayerAccountId", titlePlayerAccountId));
    this->SetTitlePlayerAccountId(std::move(titlePlayerAccountId));

    return S_OK;
}

size_t EntityLineage::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEntityLineage const*> EntityLineage::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityLineage>(&this->Model());
}

size_t EntityLineage::RequiredBufferSize(const PFEntityLineage& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.characterId)
    {
        requiredSize += (std::strlen(model.characterId) + 1);
    }
    if (model.groupId)
    {
        requiredSize += (std::strlen(model.groupId) + 1);
    }
    if (model.masterPlayerAccountId)
    {
        requiredSize += (std::strlen(model.masterPlayerAccountId) + 1);
    }
    if (model.namespaceId)
    {
        requiredSize += (std::strlen(model.namespaceId) + 1);
    }
    if (model.titleId)
    {
        requiredSize += (std::strlen(model.titleId) + 1);
    }
    if (model.titlePlayerAccountId)
    {
        requiredSize += (std::strlen(model.titlePlayerAccountId) + 1);
    }
    return requiredSize;
}

HRESULT EntityLineage::Copy(const PFEntityLineage& input, PFEntityLineage& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.characterId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.groupId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.groupId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.masterPlayerAccountId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.masterPlayerAccountId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.namespaceId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.namespaceId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.titleId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.titlePlayerAccountId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titlePlayerAccountId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace PlayFab

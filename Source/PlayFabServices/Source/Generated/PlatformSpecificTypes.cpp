#include "stdafx.h"
#include "PlatformSpecificTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace PlatformSpecific
{

JsonValue AndroidDevicePushNotificationRegistrationRequest::ToJson() const
{
    return AndroidDevicePushNotificationRegistrationRequest::ToJson(this->Model());
}

JsonValue AndroidDevicePushNotificationRegistrationRequest::ToJson(const PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "ConfirmationMessage", input.confirmationMessage);
    JsonUtils::ObjectAddMember(output, "DeviceToken", input.deviceToken);
    JsonUtils::ObjectAddMember(output, "SendPushNotificationConfirmation", input.sendPushNotificationConfirmation);
    return output;
}

JsonValue RefreshPSNAuthTokenRequest::ToJson() const
{
    return RefreshPSNAuthTokenRequest::ToJson(this->Model());
}

JsonValue RefreshPSNAuthTokenRequest::ToJson(const PFPlatformSpecificRefreshPSNAuthTokenRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "AuthCode", input.authCode);
    JsonUtils::ObjectAddMember(output, "IssuerId", input.issuerId);
    JsonUtils::ObjectAddMember(output, "RedirectUri", input.redirectUri);
    return output;
}

JsonValue RegisterForIOSPushNotificationRequest::ToJson() const
{
    return RegisterForIOSPushNotificationRequest::ToJson(this->Model());
}

JsonValue RegisterForIOSPushNotificationRequest::ToJson(const PFPlatformSpecificRegisterForIOSPushNotificationRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "ConfirmationMessage", input.confirmationMessage);
    JsonUtils::ObjectAddMember(output, "DeviceToken", input.deviceToken);
    JsonUtils::ObjectAddMember(output, "SendPushNotificationConfirmation", input.sendPushNotificationConfirmation);
    return output;
}

JsonValue AwardSteamAchievementItem::ToJson() const
{
    return AwardSteamAchievementItem::ToJson(this->Model());
}

JsonValue AwardSteamAchievementItem::ToJson(const PFPlatformSpecificAwardSteamAchievementItem& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "AchievementName", input.achievementName);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMember(output, "Result", input.result);
    return output;
}

HRESULT AwardSteamAchievementItem::FromJson(const JsonValue& input)
{
    String achievementName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AchievementName", achievementName));
    this->SetAchievementName(std::move(achievementName));

    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Result", this->m_model.result));

    return S_OK;
}

size_t AwardSteamAchievementItem::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlatformSpecificAwardSteamAchievementItem const*> AwardSteamAchievementItem::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<AwardSteamAchievementItem>(&this->Model());
}

size_t AwardSteamAchievementItem::RequiredBufferSize(const PFPlatformSpecificAwardSteamAchievementItem& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.achievementName)
    {
        requiredSize += (std::strlen(model.achievementName) + 1);
    }
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    return requiredSize;
}

HRESULT AwardSteamAchievementItem::Copy(const PFPlatformSpecificAwardSteamAchievementItem& input, PFPlatformSpecificAwardSteamAchievementItem& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.achievementName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.achievementName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue AwardSteamAchievementRequest::ToJson() const
{
    return AwardSteamAchievementRequest::ToJson(this->Model());
}

JsonValue AwardSteamAchievementRequest::ToJson(const PFPlatformSpecificAwardSteamAchievementRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<AwardSteamAchievementItem>(output, "Achievements", input.achievements, input.achievementsCount);
    return output;
}

HRESULT AwardSteamAchievementResult::FromJson(const JsonValue& input)
{
    ModelVector<AwardSteamAchievementItem> achievementResults{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<AwardSteamAchievementItem>(input, "AchievementResults", achievementResults));
    this->SetAchievementResults(std::move(achievementResults));

    return S_OK;
}

size_t AwardSteamAchievementResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlatformSpecificAwardSteamAchievementResult const*> AwardSteamAchievementResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<AwardSteamAchievementResult>(&this->Model());
}

size_t AwardSteamAchievementResult::RequiredBufferSize(const PFPlatformSpecificAwardSteamAchievementResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFPlatformSpecificAwardSteamAchievementItem*) + sizeof(PFPlatformSpecificAwardSteamAchievementItem*) * model.achievementResultsCount);
    for (size_t i = 0; i < model.achievementResultsCount; ++i)
    {
        requiredSize += AwardSteamAchievementItem::RequiredBufferSize(*model.achievementResults[i]);
    }
    return requiredSize;
}

HRESULT AwardSteamAchievementResult::Copy(const PFPlatformSpecificAwardSteamAchievementResult& input, PFPlatformSpecificAwardSteamAchievementResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<AwardSteamAchievementItem>(input.achievementResults, input.achievementResultsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.achievementResults = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace PlatformSpecific

// Json serialization helpers

} // namespace PlayFab

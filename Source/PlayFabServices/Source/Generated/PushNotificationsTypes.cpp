#include "stdafx.h"
#include "PushNotificationsTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace PushNotifications
{

JsonValue AdvancedPushPlatformMsg::ToJson() const
{
    return AdvancedPushPlatformMsg::ToJson(this->Model());
}

JsonValue AdvancedPushPlatformMsg::ToJson(const PFPushNotificationsAdvancedPushPlatformMsg& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "GCMDataOnly", input.gCMDataOnly);
    JsonUtils::ObjectAddMember(output, "Json", input.json);
    JsonUtils::ObjectAddMember(output, "Platform", input.platform);
    return output;
}

JsonValue PushNotificationPackage::ToJson() const
{
    return PushNotificationPackage::ToJson(this->Model());
}

JsonValue PushNotificationPackage::ToJson(const PFPushNotificationsPushNotificationPackage& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Badge", input.badge);
    JsonUtils::ObjectAddMember(output, "CustomData", input.customData);
    JsonUtils::ObjectAddMember(output, "Icon", input.icon);
    JsonUtils::ObjectAddMember(output, "Message", input.message);
    JsonUtils::ObjectAddMember(output, "Sound", input.sound);
    JsonUtils::ObjectAddMember(output, "Title", input.title);
    return output;
}

JsonValue SendPushNotificationRequest::ToJson() const
{
    return SendPushNotificationRequest::ToJson(this->Model());
}

JsonValue SendPushNotificationRequest::ToJson(const PFPushNotificationsSendPushNotificationRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberArray<AdvancedPushPlatformMsg>(output, "AdvancedPlatformDelivery", input.advancedPlatformDelivery, input.advancedPlatformDeliveryCount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Message", input.message);
    JsonUtils::ObjectAddMember<PushNotificationPackage>(output, "Package", input.package);
    JsonUtils::ObjectAddMember(output, "Recipient", input.recipient);
    JsonUtils::ObjectAddMember(output, "Subject", input.subject);
    JsonUtils::ObjectAddMemberArray(output, "TargetPlatforms", input.targetPlatforms, input.targetPlatformsCount);
    return output;
}

JsonValue SendPushNotificationFromTemplateRequest::ToJson() const
{
    return SendPushNotificationFromTemplateRequest::ToJson(this->Model());
}

JsonValue SendPushNotificationFromTemplateRequest::ToJson(const PFPushNotificationsSendPushNotificationFromTemplateRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PushNotificationTemplateId", input.pushNotificationTemplateId);
    JsonUtils::ObjectAddMember(output, "Recipient", input.recipient);
    return output;
}

} // namespace PushNotifications

// Json serialization helpers

} // namespace PlayFab

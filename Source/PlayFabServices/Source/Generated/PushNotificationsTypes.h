#pragma once

#include <playfab/services/cpp/PushNotificationsTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace PushNotifications
{

// PushNotifications Classes
class AdvancedPushPlatformMsg : public Wrappers::PFPushNotificationsAdvancedPushPlatformMsgWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPushNotificationsAdvancedPushPlatformMsgWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPushNotificationsAdvancedPushPlatformMsg& input);
};

class PushNotificationPackage : public Wrappers::PFPushNotificationsPushNotificationPackageWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPushNotificationsPushNotificationPackageWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPushNotificationsPushNotificationPackage& input);
};

class SendPushNotificationRequest : public Wrappers::PFPushNotificationsSendPushNotificationRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPushNotificationsSendPushNotificationRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPushNotificationsSendPushNotificationRequest& input);
};

class SendPushNotificationFromTemplateRequest : public Wrappers::PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPushNotificationsSendPushNotificationFromTemplateRequest& input);
};

} // namespace PushNotifications
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

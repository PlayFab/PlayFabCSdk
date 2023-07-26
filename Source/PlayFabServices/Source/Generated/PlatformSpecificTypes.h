#pragma once

#include <playfab/services/cpp/PlatformSpecificTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace PlatformSpecific
{

// PlatformSpecific Classes
class AndroidDevicePushNotificationRegistrationRequest : public Wrappers::PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest& input);
};

class RefreshPSNAuthTokenRequest : public Wrappers::PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlatformSpecificRefreshPSNAuthTokenRequest& input);
};

class RegisterForIOSPushNotificationRequest : public Wrappers::PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlatformSpecificRegisterForIOSPushNotificationRequest& input);
};

class AwardSteamAchievementItem : public Wrappers::PFPlatformSpecificAwardSteamAchievementItemWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFPlatformSpecificAwardSteamAchievementItem>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlatformSpecificAwardSteamAchievementItemWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlatformSpecificAwardSteamAchievementItem& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlatformSpecificAwardSteamAchievementItem const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlatformSpecificAwardSteamAchievementItem& model);
    static HRESULT Copy(const PFPlatformSpecificAwardSteamAchievementItem& input, PFPlatformSpecificAwardSteamAchievementItem& output, ModelBuffer& buffer);
};

class AwardSteamAchievementRequest : public Wrappers::PFPlatformSpecificAwardSteamAchievementRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlatformSpecificAwardSteamAchievementRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlatformSpecificAwardSteamAchievementRequest& input);
};

class AwardSteamAchievementResult : public Wrappers::PFPlatformSpecificAwardSteamAchievementResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlatformSpecificAwardSteamAchievementResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlatformSpecificAwardSteamAchievementResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlatformSpecificAwardSteamAchievementResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlatformSpecificAwardSteamAchievementResult& model);
    static HRESULT Copy(const PFPlatformSpecificAwardSteamAchievementResult& input, PFPlatformSpecificAwardSteamAchievementResult& output, ModelBuffer& buffer);
};

} // namespace PlatformSpecific
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

#pragma once

#include <playfab/core/cpp/TypeWrappers.h>
#include "BaseModel.h"

namespace PlayFab
{

// Shared Classes
class ItemInstance : public Wrappers::PFItemInstanceWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFItemInstance>
{
public:
    using ModelWrapperType = typename Wrappers::PFItemInstanceWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFItemInstance const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFItemInstance& model);
    static HRESULT Copy(const PFItemInstance& input, PFItemInstance& output, ModelBuffer& buffer);
};

class UserAndroidDeviceInfo : public Wrappers::PFUserAndroidDeviceInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserAndroidDeviceInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserAndroidDeviceInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserAndroidDeviceInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserAndroidDeviceInfo& model);
    static HRESULT Copy(const PFUserAndroidDeviceInfo& input, PFUserAndroidDeviceInfo& output, ModelBuffer& buffer);
};

class UserAppleIdInfo : public Wrappers::PFUserAppleIdInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserAppleIdInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserAppleIdInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserAppleIdInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserAppleIdInfo& model);
    static HRESULT Copy(const PFUserAppleIdInfo& input, PFUserAppleIdInfo& output, ModelBuffer& buffer);
};

class UserCustomIdInfo : public Wrappers::PFUserCustomIdInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserCustomIdInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserCustomIdInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserCustomIdInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserCustomIdInfo& model);
    static HRESULT Copy(const PFUserCustomIdInfo& input, PFUserCustomIdInfo& output, ModelBuffer& buffer);
};

class UserFacebookInfo : public Wrappers::PFUserFacebookInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserFacebookInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserFacebookInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserFacebookInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserFacebookInfo& model);
    static HRESULT Copy(const PFUserFacebookInfo& input, PFUserFacebookInfo& output, ModelBuffer& buffer);
};

class UserFacebookInstantGamesIdInfo : public Wrappers::PFUserFacebookInstantGamesIdInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserFacebookInstantGamesIdInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserFacebookInstantGamesIdInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserFacebookInstantGamesIdInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserFacebookInstantGamesIdInfo& model);
    static HRESULT Copy(const PFUserFacebookInstantGamesIdInfo& input, PFUserFacebookInstantGamesIdInfo& output, ModelBuffer& buffer);
};

class UserGameCenterInfo : public Wrappers::PFUserGameCenterInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserGameCenterInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserGameCenterInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserGameCenterInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserGameCenterInfo& model);
    static HRESULT Copy(const PFUserGameCenterInfo& input, PFUserGameCenterInfo& output, ModelBuffer& buffer);
};

class UserGoogleInfo : public Wrappers::PFUserGoogleInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserGoogleInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserGoogleInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserGoogleInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserGoogleInfo& model);
    static HRESULT Copy(const PFUserGoogleInfo& input, PFUserGoogleInfo& output, ModelBuffer& buffer);
};

class UserGooglePlayGamesInfo : public Wrappers::PFUserGooglePlayGamesInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserGooglePlayGamesInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserGooglePlayGamesInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserGooglePlayGamesInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserGooglePlayGamesInfo& model);
    static HRESULT Copy(const PFUserGooglePlayGamesInfo& input, PFUserGooglePlayGamesInfo& output, ModelBuffer& buffer);
};

class UserIosDeviceInfo : public Wrappers::PFUserIosDeviceInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserIosDeviceInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserIosDeviceInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserIosDeviceInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserIosDeviceInfo& model);
    static HRESULT Copy(const PFUserIosDeviceInfo& input, PFUserIosDeviceInfo& output, ModelBuffer& buffer);
};

class UserKongregateInfo : public Wrappers::PFUserKongregateInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserKongregateInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserKongregateInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserKongregateInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserKongregateInfo& model);
    static HRESULT Copy(const PFUserKongregateInfo& input, PFUserKongregateInfo& output, ModelBuffer& buffer);
};

class UserNintendoSwitchAccountIdInfo : public Wrappers::PFUserNintendoSwitchAccountIdInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserNintendoSwitchAccountIdInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserNintendoSwitchAccountIdInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserNintendoSwitchAccountIdInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserNintendoSwitchAccountIdInfo& model);
    static HRESULT Copy(const PFUserNintendoSwitchAccountIdInfo& input, PFUserNintendoSwitchAccountIdInfo& output, ModelBuffer& buffer);
};

class UserNintendoSwitchDeviceIdInfo : public Wrappers::PFUserNintendoSwitchDeviceIdInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserNintendoSwitchDeviceIdInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserNintendoSwitchDeviceIdInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserNintendoSwitchDeviceIdInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserNintendoSwitchDeviceIdInfo& model);
    static HRESULT Copy(const PFUserNintendoSwitchDeviceIdInfo& input, PFUserNintendoSwitchDeviceIdInfo& output, ModelBuffer& buffer);
};

class UserOpenIdInfo : public Wrappers::PFUserOpenIdInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserOpenIdInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserOpenIdInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserOpenIdInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserOpenIdInfo& model);
    static HRESULT Copy(const PFUserOpenIdInfo& input, PFUserOpenIdInfo& output, ModelBuffer& buffer);
};

class UserPrivateAccountInfo : public Wrappers::PFUserPrivateAccountInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserPrivateAccountInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserPrivateAccountInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserPrivateAccountInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserPrivateAccountInfo& model);
    static HRESULT Copy(const PFUserPrivateAccountInfo& input, PFUserPrivateAccountInfo& output, ModelBuffer& buffer);
};

class UserPsnInfo : public Wrappers::PFUserPsnInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserPsnInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserPsnInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserPsnInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserPsnInfo& model);
    static HRESULT Copy(const PFUserPsnInfo& input, PFUserPsnInfo& output, ModelBuffer& buffer);
};

class UserServerCustomIdInfo : public Wrappers::PFUserServerCustomIdInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserServerCustomIdInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserServerCustomIdInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserServerCustomIdInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserServerCustomIdInfo& model);
    static HRESULT Copy(const PFUserServerCustomIdInfo& input, PFUserServerCustomIdInfo& output, ModelBuffer& buffer);
};

class UserSteamInfo : public Wrappers::PFUserSteamInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserSteamInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserSteamInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserSteamInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserSteamInfo& model);
    static HRESULT Copy(const PFUserSteamInfo& input, PFUserSteamInfo& output, ModelBuffer& buffer);
};

class EntityKey : public Wrappers::PFEntityKeyWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFEntityKey>
{
public:
    using ModelWrapperType = typename Wrappers::PFEntityKeyWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEntityKey& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEntityKey const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEntityKey& model);
    static HRESULT Copy(const PFEntityKey& input, PFEntityKey& output, ModelBuffer& buffer);
};

class UserTitleInfo : public Wrappers::PFUserTitleInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserTitleInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserTitleInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserTitleInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserTitleInfo& model);
    static HRESULT Copy(const PFUserTitleInfo& input, PFUserTitleInfo& output, ModelBuffer& buffer);
};

class UserTwitchInfo : public Wrappers::PFUserTwitchInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserTwitchInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserTwitchInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserTwitchInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserTwitchInfo& model);
    static HRESULT Copy(const PFUserTwitchInfo& input, PFUserTwitchInfo& output, ModelBuffer& buffer);
};

class UserXboxInfo : public Wrappers::PFUserXboxInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserXboxInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserXboxInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserXboxInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserXboxInfo& model);
    static HRESULT Copy(const PFUserXboxInfo& input, PFUserXboxInfo& output, ModelBuffer& buffer);
};

class UserAccountInfo : public Wrappers::PFUserAccountInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserAccountInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserAccountInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserAccountInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserAccountInfo& model);
    static HRESULT Copy(const PFUserAccountInfo& input, PFUserAccountInfo& output, ModelBuffer& buffer);
};

class CharacterResult : public Wrappers::PFCharacterResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCharacterResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCharacterResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCharacterResult& model);
    static HRESULT Copy(const PFCharacterResult& input, PFCharacterResult& output, ModelBuffer& buffer);
};

class UserDataRecord : public Wrappers::PFUserDataRecordWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFUserDataRecord>
{
public:
    using ModelWrapperType = typename Wrappers::PFUserDataRecordWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFUserDataRecord const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFUserDataRecord& model);
    static HRESULT Copy(const PFUserDataRecord& input, PFUserDataRecord& output, ModelBuffer& buffer);
};

class VirtualCurrencyRechargeTime : public Wrappers::PFVirtualCurrencyRechargeTimeWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFVirtualCurrencyRechargeTime>
{
public:
    using ModelWrapperType = typename Wrappers::PFVirtualCurrencyRechargeTimeWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFVirtualCurrencyRechargeTime const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFVirtualCurrencyRechargeTime& model);
    static HRESULT Copy(const PFVirtualCurrencyRechargeTime& input, PFVirtualCurrencyRechargeTime& output, ModelBuffer& buffer);
};

class PlayerProfileViewConstraints : public Wrappers::PFPlayerProfileViewConstraintsWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerProfileViewConstraintsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerProfileViewConstraints& input);
};

class AdCampaignAttributionModel : public Wrappers::PFAdCampaignAttributionModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFAdCampaignAttributionModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFAdCampaignAttributionModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAdCampaignAttributionModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAdCampaignAttributionModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAdCampaignAttributionModel& model);
    static HRESULT Copy(const PFAdCampaignAttributionModel& input, PFAdCampaignAttributionModel& output, ModelBuffer& buffer);
};

class ContactEmailInfoModel : public Wrappers::PFContactEmailInfoModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFContactEmailInfoModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFContactEmailInfoModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFContactEmailInfoModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFContactEmailInfoModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFContactEmailInfoModel& model);
    static HRESULT Copy(const PFContactEmailInfoModel& input, PFContactEmailInfoModel& output, ModelBuffer& buffer);
};

class LinkedPlatformAccountModel : public Wrappers::PFLinkedPlatformAccountModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFLinkedPlatformAccountModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFLinkedPlatformAccountModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLinkedPlatformAccountModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLinkedPlatformAccountModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLinkedPlatformAccountModel& model);
    static HRESULT Copy(const PFLinkedPlatformAccountModel& input, PFLinkedPlatformAccountModel& output, ModelBuffer& buffer);
};

class LocationModel : public Wrappers::PFLocationModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFLocationModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFLocationModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLocationModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLocationModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLocationModel& model);
    static HRESULT Copy(const PFLocationModel& input, PFLocationModel& output, ModelBuffer& buffer);
};

class SubscriptionModel : public Wrappers::PFSubscriptionModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFSubscriptionModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFSubscriptionModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSubscriptionModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSubscriptionModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSubscriptionModel& model);
    static HRESULT Copy(const PFSubscriptionModel& input, PFSubscriptionModel& output, ModelBuffer& buffer);
};

class MembershipModel : public Wrappers::PFMembershipModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFMembershipModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFMembershipModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFMembershipModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMembershipModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMembershipModel& model);
    static HRESULT Copy(const PFMembershipModel& input, PFMembershipModel& output, ModelBuffer& buffer);
};

class PushNotificationRegistrationModel : public Wrappers::PFPushNotificationRegistrationModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFPushNotificationRegistrationModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFPushNotificationRegistrationModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPushNotificationRegistrationModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPushNotificationRegistrationModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPushNotificationRegistrationModel& model);
    static HRESULT Copy(const PFPushNotificationRegistrationModel& input, PFPushNotificationRegistrationModel& output, ModelBuffer& buffer);
};

class StatisticModel : public Wrappers::PFStatisticModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFStatisticModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticModel& model);
    static HRESULT Copy(const PFStatisticModel& input, PFStatisticModel& output, ModelBuffer& buffer);
};

class TagModel : public Wrappers::PFTagModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFTagModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFTagModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFTagModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFTagModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFTagModel& model);
    static HRESULT Copy(const PFTagModel& input, PFTagModel& output, ModelBuffer& buffer);
};

class ValueToDateModel : public Wrappers::PFValueToDateModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFValueToDateModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFValueToDateModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFValueToDateModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFValueToDateModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFValueToDateModel& model);
    static HRESULT Copy(const PFValueToDateModel& input, PFValueToDateModel& output, ModelBuffer& buffer);
};

class PlayerProfileModel : public Wrappers::PFPlayerProfileModelWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFPlayerProfileModel>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerProfileModelWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerProfileModel& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerProfileModel const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerProfileModel& model);
    static HRESULT Copy(const PFPlayerProfileModel& input, PFPlayerProfileModel& output, ModelBuffer& buffer);
};

class GetPlayerCombinedInfoRequestParams : public Wrappers::PFGetPlayerCombinedInfoRequestParamsWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGetPlayerCombinedInfoRequestParamsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGetPlayerCombinedInfoRequestParams& input);
};

class CharacterInventory : public Wrappers::PFCharacterInventoryWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCharacterInventory>
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterInventoryWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCharacterInventory const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCharacterInventory& model);
    static HRESULT Copy(const PFCharacterInventory& input, PFCharacterInventory& output, ModelBuffer& buffer);
};

class StatisticValue : public Wrappers::PFStatisticValueWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticValue>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticValueWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticValue const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticValue& model);
    static HRESULT Copy(const PFStatisticValue& input, PFStatisticValue& output, ModelBuffer& buffer);
};

class GetPlayerCombinedInfoResultPayload : public Wrappers::PFGetPlayerCombinedInfoResultPayloadWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGetPlayerCombinedInfoResultPayload>
{
public:
    using ModelWrapperType = typename Wrappers::PFGetPlayerCombinedInfoResultPayloadWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGetPlayerCombinedInfoResultPayload const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGetPlayerCombinedInfoResultPayload& model);
    static HRESULT Copy(const PFGetPlayerCombinedInfoResultPayload& input, PFGetPlayerCombinedInfoResultPayload& output, ModelBuffer& buffer);
};

class Variable : public Wrappers::PFVariableWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFVariable>
{
public:
    using ModelWrapperType = typename Wrappers::PFVariableWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFVariable& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFVariable const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFVariable& model);
    static HRESULT Copy(const PFVariable& input, PFVariable& output, ModelBuffer& buffer);
};

class TreatmentAssignment : public Wrappers::PFTreatmentAssignmentWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFTreatmentAssignment>
{
public:
    using ModelWrapperType = typename Wrappers::PFTreatmentAssignmentWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFTreatmentAssignment const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFTreatmentAssignment& model);
    static HRESULT Copy(const PFTreatmentAssignment& input, PFTreatmentAssignment& output, ModelBuffer& buffer);
};

class EntityLineage : public Wrappers::PFEntityLineageWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEntityLineage>
{
public:
    using ModelWrapperType = typename Wrappers::PFEntityLineageWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEntityLineage const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEntityLineage& model);
    static HRESULT Copy(const PFEntityLineage& input, PFEntityLineage& output, ModelBuffer& buffer);
};


// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFTitleActivationStatus>
{
    static constexpr PFTitleActivationStatus maxValue = PFTitleActivationStatus::RevokedSteam;
};

template<> struct EnumRange<PFCurrency>
{
    static constexpr PFCurrency maxValue = PFCurrency::ZWD;
};

template<> struct EnumRange<PFUserOrigination>
{
    static constexpr PFUserOrigination maxValue = PFUserOrigination::King;
};

template<> struct EnumRange<PFUserDataPermission>
{
    static constexpr PFUserDataPermission maxValue = PFUserDataPermission::Public;
};

template<> struct EnumRange<PFEmailVerificationStatus>
{
    static constexpr PFEmailVerificationStatus maxValue = PFEmailVerificationStatus::Confirmed;
};

template<> struct EnumRange<PFLoginIdentityProvider>
{
    static constexpr PFLoginIdentityProvider maxValue = PFLoginIdentityProvider::King;
};

template<> struct EnumRange<PFContinentCode>
{
    static constexpr PFContinentCode maxValue = PFContinentCode::Unknown;
};

template<> struct EnumRange<PFCountryCode>
{
    static constexpr PFCountryCode maxValue = PFCountryCode::Unknown;
};

template<> struct EnumRange<PFSubscriptionProviderStatus>
{
    static constexpr PFSubscriptionProviderStatus maxValue = PFSubscriptionProviderStatus::PaymentPending;
};

template<> struct EnumRange<PFPushNotificationPlatform>
{
    static constexpr PFPushNotificationPlatform maxValue = PFPushNotificationPlatform::GoogleCloudMessaging;
};

} // namespace PlayFab

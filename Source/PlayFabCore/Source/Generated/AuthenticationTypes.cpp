#include "stdafx.h"
#include "AuthenticationTypes.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Authentication
{

JsonValue LoginWithAndroidDeviceIDRequest::ToJson() const
{
    return LoginWithAndroidDeviceIDRequest::ToJson(this->Model());
}

JsonValue LoginWithAndroidDeviceIDRequest::ToJson(const PFAuthenticationLoginWithAndroidDeviceIDRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "AndroidDevice", input.androidDevice);
    JsonUtils::ObjectAddMember(output, "AndroidDeviceId", input.androidDeviceId);
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "OS", input.OS);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

HRESULT UserSettings::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GatherDeviceInfo", this->m_model.gatherDeviceInfo));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GatherFocusInfo", this->m_model.gatherFocusInfo));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NeedsAttribution", this->m_model.needsAttribution));

    return S_OK;
}

size_t UserSettings::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFAuthenticationUserSettings const*> UserSettings::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserSettings>(&this->Model());
}

size_t UserSettings::RequiredBufferSize(const PFAuthenticationUserSettings& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT UserSettings::Copy(const PFAuthenticationUserSettings& input, PFAuthenticationUserSettings& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

HRESULT LoginResult::FromJson(const JsonValue& input)
{
    std::optional<GetPlayerCombinedInfoResultPayload> infoResultPayload{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "InfoResultPayload", infoResultPayload));
    if (infoResultPayload)
    {
        this->SetInfoResultPayload(std::move(*infoResultPayload));
    }

    std::optional<time_t> lastLoginTime{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastLoginTime", lastLoginTime));
    this->SetLastLoginTime(std::move(lastLoginTime));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NewlyCreated", this->m_model.newlyCreated));

    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    std::optional<UserSettings> settingsForUser{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SettingsForUser", settingsForUser));
    if (settingsForUser)
    {
        this->SetSettingsForUser(std::move(*settingsForUser));
    }

    std::optional<TreatmentAssignment> treatmentAssignment{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TreatmentAssignment", treatmentAssignment));
    if (treatmentAssignment)
    {
        this->SetTreatmentAssignment(std::move(*treatmentAssignment));
    }

    return S_OK;
}

size_t LoginResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFAuthenticationLoginResult const*> LoginResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<LoginResult>(&this->Model());
}

size_t LoginResult::RequiredBufferSize(const PFAuthenticationLoginResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.infoResultPayload)
    {
        requiredSize += GetPlayerCombinedInfoResultPayload::RequiredBufferSize(*model.infoResultPayload);
    }
    if (model.lastLoginTime)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    if (model.settingsForUser)
    {
        requiredSize += UserSettings::RequiredBufferSize(*model.settingsForUser);
    }
    if (model.treatmentAssignment)
    {
        requiredSize += TreatmentAssignment::RequiredBufferSize(*model.treatmentAssignment);
    }
    return requiredSize;
}

HRESULT LoginResult::Copy(const PFAuthenticationLoginResult& input, PFAuthenticationLoginResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<GetPlayerCombinedInfoResultPayload>(input.infoResultPayload); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.infoResultPayload = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.lastLoginTime); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lastLoginTime = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserSettings>(input.settingsForUser); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.settingsForUser = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<TreatmentAssignment>(input.treatmentAssignment); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.treatmentAssignment = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue LoginWithAppleRequest::ToJson() const
{
    return LoginWithAppleRequest::ToJson(this->Model());
}

JsonValue LoginWithAppleRequest::ToJson(const PFAuthenticationLoginWithAppleRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "IdentityToken", input.identityToken);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithCustomIDRequest::ToJson() const
{
    return LoginWithCustomIDRequest::ToJson(this->Model());
}

JsonValue LoginWithCustomIDRequest::ToJson(const PFAuthenticationLoginWithCustomIDRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMember(output, "CustomId", input.customId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithEmailAddressRequest::ToJson() const
{
    return LoginWithEmailAddressRequest::ToJson(this->Model());
}

JsonValue LoginWithEmailAddressRequest::ToJson(const PFAuthenticationLoginWithEmailAddressRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Email", input.email);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "Password", input.password);
    return output;
}

JsonValue LoginWithFacebookRequest::ToJson() const
{
    return LoginWithFacebookRequest::ToJson(this->Model());
}

JsonValue LoginWithFacebookRequest::ToJson(const PFAuthenticationLoginWithFacebookRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "AccessToken", input.accessToken);
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithFacebookInstantGamesIdRequest::ToJson() const
{
    return LoginWithFacebookInstantGamesIdRequest::ToJson(this->Model());
}

JsonValue LoginWithFacebookInstantGamesIdRequest::ToJson(const PFAuthenticationLoginWithFacebookInstantGamesIdRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "FacebookInstantGamesSignature", input.facebookInstantGamesSignature);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithGameCenterRequest::ToJson() const
{
    return LoginWithGameCenterRequest::ToJson(this->Model());
}

JsonValue LoginWithGameCenterRequest::ToJson(const PFAuthenticationLoginWithGameCenterRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerId", input.playerId);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    JsonUtils::ObjectAddMember(output, "PublicKeyUrl", input.publicKeyUrl);
    JsonUtils::ObjectAddMember(output, "Salt", input.salt);
    JsonUtils::ObjectAddMember(output, "Signature", input.signature);
    JsonUtils::ObjectAddMember(output, "Timestamp", input.timestamp);
    return output;
}

JsonValue LoginWithGoogleAccountRequest::ToJson() const
{
    return LoginWithGoogleAccountRequest::ToJson(this->Model());
}

JsonValue LoginWithGoogleAccountRequest::ToJson(const PFAuthenticationLoginWithGoogleAccountRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    JsonUtils::ObjectAddMember(output, "ServerAuthCode", input.serverAuthCode);
    JsonUtils::ObjectAddMember(output, "SetEmail", input.setEmail);
    return output;
}

JsonValue LoginWithGooglePlayGamesServicesRequest::ToJson() const
{
    return LoginWithGooglePlayGamesServicesRequest::ToJson(this->Model());
}

JsonValue LoginWithGooglePlayGamesServicesRequest::ToJson(const PFAuthenticationLoginWithGooglePlayGamesServicesRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    JsonUtils::ObjectAddMember(output, "ServerAuthCode", input.serverAuthCode);
    return output;
}

JsonValue LoginWithIOSDeviceIDRequest::ToJson() const
{
    return LoginWithIOSDeviceIDRequest::ToJson(this->Model());
}

JsonValue LoginWithIOSDeviceIDRequest::ToJson(const PFAuthenticationLoginWithIOSDeviceIDRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DeviceId", input.deviceId);
    JsonUtils::ObjectAddMember(output, "DeviceModel", input.deviceModel);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "OS", input.OS);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithKongregateRequest::ToJson() const
{
    return LoginWithKongregateRequest::ToJson(this->Model());
}

JsonValue LoginWithKongregateRequest::ToJson(const PFAuthenticationLoginWithKongregateRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "AuthTicket", input.authTicket);
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "KongregateId", input.kongregateId);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithNintendoServiceAccountRequest::ToJson() const
{
    return LoginWithNintendoServiceAccountRequest::ToJson(this->Model());
}

JsonValue LoginWithNintendoServiceAccountRequest::ToJson(const PFAuthenticationLoginWithNintendoServiceAccountRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "IdentityToken", input.identityToken);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithNintendoSwitchDeviceIdRequest::ToJson() const
{
    return LoginWithNintendoSwitchDeviceIdRequest::ToJson(this->Model());
}

JsonValue LoginWithNintendoSwitchDeviceIdRequest::ToJson(const PFAuthenticationLoginWithNintendoSwitchDeviceIdRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "NintendoSwitchDeviceId", input.nintendoSwitchDeviceId);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithOpenIdConnectRequest::ToJson() const
{
    return LoginWithOpenIdConnectRequest::ToJson(this->Model());
}

JsonValue LoginWithOpenIdConnectRequest::ToJson(const PFAuthenticationLoginWithOpenIdConnectRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "ConnectionId", input.connectionId);
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "IdToken", input.idToken);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithPlayFabRequest::ToJson() const
{
    return LoginWithPlayFabRequest::ToJson(this->Model());
}

JsonValue LoginWithPlayFabRequest::ToJson(const PFAuthenticationLoginWithPlayFabRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "Password", input.password);
    JsonUtils::ObjectAddMember(output, "Username", input.username);
    return output;
}

JsonValue LoginWithPSNRequest::ToJson() const
{
    return LoginWithPSNRequest::ToJson(this->Model());
}

JsonValue LoginWithPSNRequest::ToJson(const PFAuthenticationLoginWithPSNRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "AuthCode", input.authCode);
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "IssuerId", input.issuerId);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    JsonUtils::ObjectAddMember(output, "RedirectUri", input.redirectUri);
    return output;
}

JsonValue LoginWithSteamRequest::ToJson() const
{
    return LoginWithSteamRequest::ToJson(this->Model());
}

JsonValue LoginWithSteamRequest::ToJson(const PFAuthenticationLoginWithSteamRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    JsonUtils::ObjectAddMember(output, "SteamTicket", input.steamTicket);
    JsonUtils::ObjectAddMember(output, "TicketIsServiceSpecific", input.ticketIsServiceSpecific);
    return output;
}

JsonValue LoginWithTwitchRequest::ToJson() const
{
    return LoginWithTwitchRequest::ToJson(this->Model());
}

JsonValue LoginWithTwitchRequest::ToJson(const PFAuthenticationLoginWithTwitchRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "AccessToken", input.accessToken);
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}

JsonValue LoginWithXboxRequest::ToJson() const
{
    return LoginWithXboxRequest::ToJson(this->Model());
}

JsonValue LoginWithXboxRequest::ToJson(const PFAuthenticationLoginWithXboxRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    JsonUtils::ObjectAddMember(output, "XboxToken", input.xboxToken);
    return output;
}

#if HC_PLATFORM == HC_PLATFORM_GDK
JsonValue LoginWithXUserRequest::ToJson() const
{
    return LoginWithXUserRequest::ToJson(this->Model());
}

JsonValue LoginWithXUserRequest::ToJson(const PFAuthenticationLoginWithXUserRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    return output;
}
#endif

JsonValue RegisterPlayFabUserRequest::ToJson() const
{
    return RegisterPlayFabUserRequest::ToJson(this->Model());
}

JsonValue RegisterPlayFabUserRequest::ToJson(const PFAuthenticationRegisterPlayFabUserRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DisplayName", input.displayName);
    JsonUtils::ObjectAddMember(output, "Email", input.email);
    JsonUtils::ObjectAddMember(output, "EncryptedRequest", input.encryptedRequest);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "Password", input.password);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    JsonUtils::ObjectAddMember(output, "RequireBothUsernameAndEmail", input.requireBothUsernameAndEmail);
    JsonUtils::ObjectAddMember(output, "TitleId", input.titleId);
    JsonUtils::ObjectAddMember(output, "Username", input.username);
    return output;
}

HRESULT RegisterPlayFabUserResult::FromJson(const JsonValue& input)
{
    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    std::optional<UserSettings> settingsForUser{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SettingsForUser", settingsForUser));
    if (settingsForUser)
    {
        this->SetSettingsForUser(std::move(*settingsForUser));
    }

    String username{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Username", username));
    this->SetUsername(std::move(username));

    return S_OK;
}

size_t RegisterPlayFabUserResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFAuthenticationRegisterPlayFabUserResult const*> RegisterPlayFabUserResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RegisterPlayFabUserResult>(&this->Model());
}

size_t RegisterPlayFabUserResult::RequiredBufferSize(const PFAuthenticationRegisterPlayFabUserResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    if (model.settingsForUser)
    {
        requiredSize += UserSettings::RequiredBufferSize(*model.settingsForUser);
    }
    if (model.username)
    {
        requiredSize += (std::strlen(model.username) + 1);
    }
    return requiredSize;
}

HRESULT RegisterPlayFabUserResult::Copy(const PFAuthenticationRegisterPlayFabUserResult& input, PFAuthenticationRegisterPlayFabUserResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserSettings>(input.settingsForUser); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.settingsForUser = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.username); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.username = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ServerLoginWithPSNRequest::ToJson() const
{
    return ServerLoginWithPSNRequest::ToJson(this->Model());
}

JsonValue ServerLoginWithPSNRequest::ToJson(const PFAuthenticationServerLoginWithPSNRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "AuthCode", input.authCode);
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "IssuerId", input.issuerId);
    JsonUtils::ObjectAddMember(output, "RedirectUri", input.redirectUri);
    return output;
}

JsonValue LoginWithServerCustomIdRequest::ToJson() const
{
    return LoginWithServerCustomIdRequest::ToJson(this->Model());
}

JsonValue LoginWithServerCustomIdRequest::ToJson(const PFAuthenticationLoginWithServerCustomIdRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "PlayerSecret", input.playerSecret);
    JsonUtils::ObjectAddMember(output, "ServerCustomId", input.serverCustomId);
    return output;
}

JsonValue LoginWithSteamIdRequest::ToJson() const
{
    return LoginWithSteamIdRequest::ToJson(this->Model());
}

JsonValue LoginWithSteamIdRequest::ToJson(const PFAuthenticationLoginWithSteamIdRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "SteamId", input.steamId);
    return output;
}

JsonValue ServerLoginWithXboxRequest::ToJson() const
{
    return ServerLoginWithXboxRequest::ToJson(this->Model());
}

JsonValue ServerLoginWithXboxRequest::ToJson(const PFAuthenticationServerLoginWithXboxRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "XboxToken", input.xboxToken);
    return output;
}

JsonValue LoginWithXboxIdRequest::ToJson() const
{
    return LoginWithXboxIdRequest::ToJson(this->Model());
}

JsonValue LoginWithXboxIdRequest::ToJson(const PFAuthenticationLoginWithXboxIdRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CreateAccount", input.createAccount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<GetPlayerCombinedInfoRequestParams>(output, "InfoRequestParameters", input.infoRequestParameters);
    JsonUtils::ObjectAddMember(output, "Sandbox", input.sandbox);
    JsonUtils::ObjectAddMember(output, "XboxId", input.xboxId);
    return output;
}

#if HC_PLATFORM != HC_PLATFORM_GDK
JsonValue AuthenticateCustomIdRequest::ToJson() const
{
    return AuthenticateCustomIdRequest::ToJson(this->Model());
}

JsonValue AuthenticateCustomIdRequest::ToJson(const PFAuthenticationAuthenticateCustomIdRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CustomId", input.customId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    return output;
}

#endif
HRESULT EntityTokenResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    String entityToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EntityToken", entityToken));
    this->SetEntityToken(std::move(entityToken));

    std::optional<time_t> tokenExpiration{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "TokenExpiration", tokenExpiration));
    this->SetTokenExpiration(std::move(tokenExpiration));

    return S_OK;
}

size_t EntityTokenResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFAuthenticationEntityTokenResponse const*> EntityTokenResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityTokenResponse>(&this->Model());
}

size_t EntityTokenResponse::RequiredBufferSize(const PFAuthenticationEntityTokenResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    if (model.entityToken)
    {
        requiredSize += (std::strlen(model.entityToken) + 1);
    }
    if (model.tokenExpiration)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    return requiredSize;
}

HRESULT EntityTokenResponse::Copy(const PFAuthenticationEntityTokenResponse& input, PFAuthenticationEntityTokenResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.entityToken); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entityToken = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.tokenExpiration); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tokenExpiration = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

#if HC_PLATFORM != HC_PLATFORM_GDK
HRESULT AuthenticateCustomIdResult::FromJson(const JsonValue& input)
{
    std::optional<EntityTokenResponse> entityToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EntityToken", entityToken));
    if (entityToken)
    {
        this->SetEntityToken(std::move(*entityToken));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NewlyCreated", this->m_model.newlyCreated));

    return S_OK;
}

size_t AuthenticateCustomIdResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFAuthenticationAuthenticateCustomIdResult const*> AuthenticateCustomIdResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<AuthenticateCustomIdResult>(&this->Model());
}

size_t AuthenticateCustomIdResult::RequiredBufferSize(const PFAuthenticationAuthenticateCustomIdResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entityToken)
    {
        requiredSize += EntityTokenResponse::RequiredBufferSize(*model.entityToken);
    }
    return requiredSize;
}

HRESULT AuthenticateCustomIdResult::Copy(const PFAuthenticationAuthenticateCustomIdResult& input, PFAuthenticationAuthenticateCustomIdResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityTokenResponse>(input.entityToken); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entityToken = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

#endif
JsonValue DeleteRequest::ToJson() const
{
    return DeleteRequest::ToJson(this->Model());
}

JsonValue DeleteRequest::ToJson(const PFAuthenticationDeleteRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

JsonValue GetEntityRequest::ToJson() const
{
    return GetEntityRequest::ToJson(this->Model());
}

JsonValue GetEntityRequest::ToJson(const PFAuthenticationGetEntityRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT GetEntityTokenResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EntityToken", entityToken));
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "TokenExpiration", tokenExpiration));
    return S_OK;
}

JsonValue ValidateEntityTokenRequest::ToJson() const
{
    return ValidateEntityTokenRequest::ToJson(this->Model());
}

JsonValue ValidateEntityTokenRequest::ToJson(const PFAuthenticationValidateEntityTokenRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "EntityToken", input.entityToken);
    return output;
}

HRESULT ValidateEntityTokenResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    std::optional<PFAuthenticationIdentifiedDeviceType> identifiedDeviceType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdentifiedDeviceType", identifiedDeviceType));
    this->SetIdentifiedDeviceType(std::move(identifiedDeviceType));

    std::optional<PFLoginIdentityProvider> identityProvider{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdentityProvider", identityProvider));
    this->SetIdentityProvider(std::move(identityProvider));

    String identityProviderIssuedId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdentityProviderIssuedId", identityProviderIssuedId));
    this->SetIdentityProviderIssuedId(std::move(identityProviderIssuedId));

    std::optional<EntityLineage> lineage{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Lineage", lineage));
    if (lineage)
    {
        this->SetLineage(std::move(*lineage));
    }

    return S_OK;
}

size_t ValidateEntityTokenResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFAuthenticationValidateEntityTokenResponse const*> ValidateEntityTokenResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ValidateEntityTokenResponse>(&this->Model());
}

size_t ValidateEntityTokenResponse::RequiredBufferSize(const PFAuthenticationValidateEntityTokenResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    if (model.identifiedDeviceType)
    {
        requiredSize += (alignof(PFAuthenticationIdentifiedDeviceType) + sizeof(PFAuthenticationIdentifiedDeviceType));
    }
    if (model.identityProvider)
    {
        requiredSize += (alignof(PFLoginIdentityProvider) + sizeof(PFLoginIdentityProvider));
    }
    if (model.identityProviderIssuedId)
    {
        requiredSize += (std::strlen(model.identityProviderIssuedId) + 1);
    }
    if (model.lineage)
    {
        requiredSize += EntityLineage::RequiredBufferSize(*model.lineage);
    }
    return requiredSize;
}

HRESULT ValidateEntityTokenResponse::Copy(const PFAuthenticationValidateEntityTokenResponse& input, PFAuthenticationValidateEntityTokenResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.identifiedDeviceType); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.identifiedDeviceType = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.identityProvider); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.identityProvider = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.identityProviderIssuedId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.identityProviderIssuedId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityLineage>(input.lineage); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lineage = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Authentication
} // namespace PlayFab

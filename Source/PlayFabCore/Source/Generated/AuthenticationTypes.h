#pragma once

#include <playfab/core/cpp/AuthenticationTypeWrappers.h>
#include "BaseModel.h"

namespace PlayFab
{
namespace Authentication
{

// Authentication Classes
class LoginWithAndroidDeviceIDRequest : public Wrappers::PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithAndroidDeviceIDRequest& input);
};

class UserSettings : public Wrappers::PFAuthenticationUserSettingsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAuthenticationUserSettings>
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationUserSettingsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAuthenticationUserSettings const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAuthenticationUserSettings& model);
    static HRESULT Copy(const PFAuthenticationUserSettings& input, PFAuthenticationUserSettings& output, ModelBuffer& buffer);
};

class LoginResult : public Wrappers::PFAuthenticationLoginResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAuthenticationLoginResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAuthenticationLoginResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAuthenticationLoginResult& model);
    static HRESULT Copy(const PFAuthenticationLoginResult& input, PFAuthenticationLoginResult& output, ModelBuffer& buffer);
};

class LoginWithAppleRequest : public Wrappers::PFAuthenticationLoginWithAppleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithAppleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithAppleRequest& input);
};

class LoginWithCustomIDRequest : public Wrappers::PFAuthenticationLoginWithCustomIDRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithCustomIDRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithCustomIDRequest& input);
};

class LoginWithEmailAddressRequest : public Wrappers::PFAuthenticationLoginWithEmailAddressRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithEmailAddressRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithEmailAddressRequest& input);
};

class LoginWithFacebookRequest : public Wrappers::PFAuthenticationLoginWithFacebookRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithFacebookRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithFacebookRequest& input);
};

class LoginWithFacebookInstantGamesIdRequest : public Wrappers::PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithFacebookInstantGamesIdRequest& input);
};

class LoginWithGameCenterRequest : public Wrappers::PFAuthenticationLoginWithGameCenterRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithGameCenterRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithGameCenterRequest& input);
};

class LoginWithGoogleAccountRequest : public Wrappers::PFAuthenticationLoginWithGoogleAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithGoogleAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithGoogleAccountRequest& input);
};

class LoginWithGooglePlayGamesServicesRequest : public Wrappers::PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithGooglePlayGamesServicesRequest& input);
};

class LoginWithIOSDeviceIDRequest : public Wrappers::PFAuthenticationLoginWithIOSDeviceIDRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithIOSDeviceIDRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithIOSDeviceIDRequest& input);
};

class LoginWithKongregateRequest : public Wrappers::PFAuthenticationLoginWithKongregateRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithKongregateRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithKongregateRequest& input);
};

class LoginWithNintendoServiceAccountRequest : public Wrappers::PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithNintendoServiceAccountRequest& input);
};

class LoginWithNintendoSwitchDeviceIdRequest : public Wrappers::PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithNintendoSwitchDeviceIdRequest& input);
};

class LoginWithOpenIdConnectRequest : public Wrappers::PFAuthenticationLoginWithOpenIdConnectRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithOpenIdConnectRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithOpenIdConnectRequest& input);
};

class LoginWithPlayFabRequest : public Wrappers::PFAuthenticationLoginWithPlayFabRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithPlayFabRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithPlayFabRequest& input);
};

class LoginWithPSNRequest : public Wrappers::PFAuthenticationLoginWithPSNRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithPSNRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithPSNRequest& input);
};

class LoginWithSteamRequest : public Wrappers::PFAuthenticationLoginWithSteamRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithSteamRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithSteamRequest& input);
};

class LoginWithTwitchRequest : public Wrappers::PFAuthenticationLoginWithTwitchRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithTwitchRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithTwitchRequest& input);
};

class LoginWithXboxRequest : public Wrappers::PFAuthenticationLoginWithXboxRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithXboxRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithXboxRequest& input);
};

#if HC_PLATFORM == HC_PLATFORM_GDK
class LoginWithXUserRequest : public Wrappers::PFAuthenticationLoginWithXUserRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithXUserRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithXUserRequest& input);
};
#endif

class RegisterPlayFabUserRequest : public Wrappers::PFAuthenticationRegisterPlayFabUserRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationRegisterPlayFabUserRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationRegisterPlayFabUserRequest& input);
};

class RegisterPlayFabUserResult : public Wrappers::PFAuthenticationRegisterPlayFabUserResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAuthenticationRegisterPlayFabUserResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationRegisterPlayFabUserResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAuthenticationRegisterPlayFabUserResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAuthenticationRegisterPlayFabUserResult& model);
    static HRESULT Copy(const PFAuthenticationRegisterPlayFabUserResult& input, PFAuthenticationRegisterPlayFabUserResult& output, ModelBuffer& buffer);
};

class LoginWithServerCustomIdRequest : public Wrappers::PFAuthenticationLoginWithServerCustomIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithServerCustomIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithServerCustomIdRequest& input);
};

class LoginWithSteamIdRequest : public Wrappers::PFAuthenticationLoginWithSteamIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithSteamIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithSteamIdRequest& input);
};

class ServerLoginWithXboxRequest : public Wrappers::PFAuthenticationServerLoginWithXboxRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationServerLoginWithXboxRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationServerLoginWithXboxRequest& input);
};

class LoginWithXboxIdRequest : public Wrappers::PFAuthenticationLoginWithXboxIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationLoginWithXboxIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationLoginWithXboxIdRequest& input);
};

#if HC_PLATFORM != HC_PLATFORM_GDK
class AuthenticateCustomIdRequest : public Wrappers::PFAuthenticationAuthenticateCustomIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationAuthenticateCustomIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationAuthenticateCustomIdRequest& input);
};
#endif

class EntityTokenResponse : public Wrappers::PFAuthenticationEntityTokenResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAuthenticationEntityTokenResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationEntityTokenResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAuthenticationEntityTokenResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAuthenticationEntityTokenResponse& model);
    static HRESULT Copy(const PFAuthenticationEntityTokenResponse& input, PFAuthenticationEntityTokenResponse& output, ModelBuffer& buffer);
};

#if HC_PLATFORM != HC_PLATFORM_GDK
class AuthenticateCustomIdResult : public Wrappers::PFAuthenticationAuthenticateCustomIdResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAuthenticationAuthenticateCustomIdResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationAuthenticateCustomIdResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAuthenticationAuthenticateCustomIdResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAuthenticationAuthenticateCustomIdResult& model);
    static HRESULT Copy(const PFAuthenticationAuthenticateCustomIdResult& input, PFAuthenticationAuthenticateCustomIdResult& output, ModelBuffer& buffer);
};
#endif

class DeleteRequest : public Wrappers::PFAuthenticationDeleteRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationDeleteRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationDeleteRequest& input);
};

class GetEntityRequest : public Wrappers::PFAuthenticationGetEntityRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationGetEntityRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationGetEntityRequest& input);
};

class GetEntityTokenResponse : public ServiceOutputModel
{
public:
    HRESULT FromJson(const JsonValue& input);

    std::optional<EntityKey> entity;
    String entityToken;
    std::optional<time_t> tokenExpiration;
};

class ValidateEntityTokenRequest : public Wrappers::PFAuthenticationValidateEntityTokenRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationValidateEntityTokenRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAuthenticationValidateEntityTokenRequest& input);
};

class ValidateEntityTokenResponse : public Wrappers::PFAuthenticationValidateEntityTokenResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAuthenticationValidateEntityTokenResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFAuthenticationValidateEntityTokenResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAuthenticationValidateEntityTokenResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAuthenticationValidateEntityTokenResponse& model);
    static HRESULT Copy(const PFAuthenticationValidateEntityTokenResponse& input, PFAuthenticationValidateEntityTokenResponse& output, ModelBuffer& buffer);
};

} // namespace Authentication

// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFAuthenticationIdentifiedDeviceType>
{
    static constexpr PFAuthenticationIdentifiedDeviceType maxValue = PFAuthenticationIdentifiedDeviceType::Scarlett;
};

} // namespace PlayFab

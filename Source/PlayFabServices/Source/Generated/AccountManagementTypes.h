#pragma once

#include <playfab/services/cpp/AccountManagementTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace AccountManagement
{

// AccountManagement Classes
class AddOrUpdateContactEmailRequest : public Wrappers::PFAccountManagementAddOrUpdateContactEmailRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementAddOrUpdateContactEmailRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementAddOrUpdateContactEmailRequest& input);
};

class AddUsernamePasswordRequest : public Wrappers::PFAccountManagementAddUsernamePasswordRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementAddUsernamePasswordRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementAddUsernamePasswordRequest& input);
};

class AddUsernamePasswordResult : public Wrappers::PFAccountManagementAddUsernamePasswordResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementAddUsernamePasswordResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementAddUsernamePasswordResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementAddUsernamePasswordResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementAddUsernamePasswordResult& model);
    static HRESULT Copy(const PFAccountManagementAddUsernamePasswordResult& input, PFAccountManagementAddUsernamePasswordResult& output, ModelBuffer& buffer);
};

class GetAccountInfoRequest : public Wrappers::PFAccountManagementGetAccountInfoRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetAccountInfoRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetAccountInfoRequest& input);
};

class GetAccountInfoResult : public Wrappers::PFAccountManagementGetAccountInfoResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetAccountInfoResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetAccountInfoResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetAccountInfoResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetAccountInfoResult& model);
    static HRESULT Copy(const PFAccountManagementGetAccountInfoResult& input, PFAccountManagementGetAccountInfoResult& output, ModelBuffer& buffer);
};

class GetPlayerCombinedInfoRequest : public Wrappers::PFAccountManagementGetPlayerCombinedInfoRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayerCombinedInfoRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayerCombinedInfoRequest& input);
};

class GetPlayerCombinedInfoResult : public Wrappers::PFAccountManagementGetPlayerCombinedInfoResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayerCombinedInfoResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayerCombinedInfoResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayerCombinedInfoResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayerCombinedInfoResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayerCombinedInfoResult& input, PFAccountManagementGetPlayerCombinedInfoResult& output, ModelBuffer& buffer);
};

class GetPlayerProfileRequest : public Wrappers::PFAccountManagementGetPlayerProfileRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayerProfileRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayerProfileRequest& input);
};

class GetPlayerProfileResult : public Wrappers::PFAccountManagementGetPlayerProfileResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayerProfileResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayerProfileResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayerProfileResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayerProfileResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayerProfileResult& input, PFAccountManagementGetPlayerProfileResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromFacebookIDsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest& input);
};

class FacebookPlayFabIdPair : public Wrappers::PFAccountManagementFacebookPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementFacebookPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementFacebookPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementFacebookPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementFacebookPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementFacebookPlayFabIdPair& input, PFAccountManagementFacebookPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromFacebookIDsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromFacebookIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromFacebookIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromFacebookIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromFacebookIDsResult& input, PFAccountManagementGetPlayFabIDsFromFacebookIDsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromFacebookInstantGamesIdsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest& input);
};

class FacebookInstantGamesPlayFabIdPair : public Wrappers::PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementFacebookInstantGamesPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementFacebookInstantGamesPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementFacebookInstantGamesPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementFacebookInstantGamesPlayFabIdPair& input, PFAccountManagementFacebookInstantGamesPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromFacebookInstantGamesIdsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult& input, PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromGameCenterIDsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequest& input);
};

class GameCenterPlayFabIdPair : public Wrappers::PFAccountManagementGameCenterPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGameCenterPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGameCenterPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGameCenterPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGameCenterPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementGameCenterPlayFabIdPair& input, PFAccountManagementGameCenterPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromGameCenterIDsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult& input, PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromGoogleIDsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromGoogleIDsRequest& input);
};

class GooglePlayFabIdPair : public Wrappers::PFAccountManagementGooglePlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGooglePlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGooglePlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGooglePlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGooglePlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementGooglePlayFabIdPair& input, PFAccountManagementGooglePlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromGoogleIDsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromGoogleIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromGoogleIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromGoogleIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromGoogleIDsResult& input, PFAccountManagementGetPlayFabIDsFromGoogleIDsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest& input);
};

class GooglePlayGamesPlayFabIdPair : public Wrappers::PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGooglePlayGamesPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGooglePlayGamesPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGooglePlayGamesPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementGooglePlayGamesPlayFabIdPair& input, PFAccountManagementGooglePlayGamesPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromGooglePlayGamesPlayerIDsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult& input, PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromKongregateIDsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromKongregateIDsRequest& input);
};

class KongregatePlayFabIdPair : public Wrappers::PFAccountManagementKongregatePlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementKongregatePlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementKongregatePlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementKongregatePlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementKongregatePlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementKongregatePlayFabIdPair& input, PFAccountManagementKongregatePlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromKongregateIDsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromKongregateIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromKongregateIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromKongregateIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromKongregateIDsResult& input, PFAccountManagementGetPlayFabIDsFromKongregateIDsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromNintendoServiceAccountIdsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest& input);
};

class NintendoServiceAccountPlayFabIdPair : public Wrappers::PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementNintendoServiceAccountPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementNintendoServiceAccountPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementNintendoServiceAccountPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementNintendoServiceAccountPlayFabIdPair& input, PFAccountManagementNintendoServiceAccountPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromNintendoServiceAccountIdsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult& input, PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromNintendoSwitchDeviceIdsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest& input);
};

class NintendoSwitchPlayFabIdPair : public Wrappers::PFAccountManagementNintendoSwitchPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementNintendoSwitchPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementNintendoSwitchPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementNintendoSwitchPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementNintendoSwitchPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementNintendoSwitchPlayFabIdPair& input, PFAccountManagementNintendoSwitchPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromNintendoSwitchDeviceIdsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult& input, PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromPSNAccountIDsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest& input);
};

class PSNAccountPlayFabIdPair : public Wrappers::PFAccountManagementPSNAccountPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementPSNAccountPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementPSNAccountPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementPSNAccountPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementPSNAccountPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementPSNAccountPlayFabIdPair& input, PFAccountManagementPSNAccountPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromPSNAccountIDsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult& input, PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromSteamIDsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromSteamIDsRequest& input);
};

class SteamPlayFabIdPair : public Wrappers::PFAccountManagementSteamPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementSteamPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementSteamPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementSteamPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementSteamPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementSteamPlayFabIdPair& input, PFAccountManagementSteamPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromSteamIDsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromSteamIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromSteamIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromSteamIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromSteamIDsResult& input, PFAccountManagementGetPlayFabIDsFromSteamIDsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromTwitchIDsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest& input);
};

class TwitchPlayFabIdPair : public Wrappers::PFAccountManagementTwitchPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementTwitchPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementTwitchPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementTwitchPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementTwitchPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementTwitchPlayFabIdPair& input, PFAccountManagementTwitchPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromTwitchIDsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromTwitchIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromTwitchIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromTwitchIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromTwitchIDsResult& input, PFAccountManagementGetPlayFabIDsFromTwitchIDsResult& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromXboxLiveIDsRequest : public Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest& input);
};

class XboxLiveAccountPlayFabIdPair : public Wrappers::PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementXboxLiveAccountPlayFabIdPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementXboxLiveAccountPlayFabIdPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementXboxLiveAccountPlayFabIdPair& model);
    static HRESULT Copy(const PFAccountManagementXboxLiveAccountPlayFabIdPair& input, PFAccountManagementXboxLiveAccountPlayFabIdPair& output, ModelBuffer& buffer);
};

class GetPlayFabIDsFromXboxLiveIDsResult : public Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult& input, PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult& output, ModelBuffer& buffer);
};

class LinkAndroidDeviceIDRequest : public Wrappers::PFAccountManagementLinkAndroidDeviceIDRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkAndroidDeviceIDRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkAndroidDeviceIDRequest& input);
};

class LinkAppleRequest : public Wrappers::PFAccountManagementLinkAppleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkAppleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkAppleRequest& input);
};

class LinkCustomIDRequest : public Wrappers::PFAccountManagementLinkCustomIDRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkCustomIDRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkCustomIDRequest& input);
};

class LinkFacebookAccountRequest : public Wrappers::PFAccountManagementLinkFacebookAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkFacebookAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkFacebookAccountRequest& input);
};

class LinkFacebookInstantGamesIdRequest : public Wrappers::PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkFacebookInstantGamesIdRequest& input);
};

class LinkGameCenterAccountRequest : public Wrappers::PFAccountManagementLinkGameCenterAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkGameCenterAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkGameCenterAccountRequest& input);
};

class LinkGoogleAccountRequest : public Wrappers::PFAccountManagementLinkGoogleAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkGoogleAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkGoogleAccountRequest& input);
};

class LinkGooglePlayGamesServicesAccountRequest : public Wrappers::PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkGooglePlayGamesServicesAccountRequest& input);
};

class LinkIOSDeviceIDRequest : public Wrappers::PFAccountManagementLinkIOSDeviceIDRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkIOSDeviceIDRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkIOSDeviceIDRequest& input);
};

class LinkKongregateAccountRequest : public Wrappers::PFAccountManagementLinkKongregateAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkKongregateAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkKongregateAccountRequest& input);
};

class ClientLinkNintendoServiceAccountRequest : public Wrappers::PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementClientLinkNintendoServiceAccountRequest& input);
};

class ClientLinkNintendoSwitchDeviceIdRequest : public Wrappers::PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementClientLinkNintendoSwitchDeviceIdRequest& input);
};

class LinkOpenIdConnectRequest : public Wrappers::PFAccountManagementLinkOpenIdConnectRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkOpenIdConnectRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkOpenIdConnectRequest& input);
};

class ClientLinkPSNAccountRequest : public Wrappers::PFAccountManagementClientLinkPSNAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementClientLinkPSNAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementClientLinkPSNAccountRequest& input);
};

class LinkSteamAccountRequest : public Wrappers::PFAccountManagementLinkSteamAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkSteamAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkSteamAccountRequest& input);
};

class LinkTwitchAccountRequest : public Wrappers::PFAccountManagementLinkTwitchAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkTwitchAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkTwitchAccountRequest& input);
};

class ClientLinkXboxAccountRequest : public Wrappers::PFAccountManagementClientLinkXboxAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementClientLinkXboxAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementClientLinkXboxAccountRequest& input);
};

class RemoveContactEmailRequest : public Wrappers::PFAccountManagementRemoveContactEmailRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementRemoveContactEmailRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementRemoveContactEmailRequest& input);
};

class ReportPlayerClientRequest : public Wrappers::PFAccountManagementReportPlayerClientRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementReportPlayerClientRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementReportPlayerClientRequest& input);
};

class ReportPlayerClientResult : public Wrappers::PFAccountManagementReportPlayerClientResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementReportPlayerClientResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementReportPlayerClientResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementReportPlayerClientResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementReportPlayerClientResult& model);
    static HRESULT Copy(const PFAccountManagementReportPlayerClientResult& input, PFAccountManagementReportPlayerClientResult& output, ModelBuffer& buffer);
};

class SendAccountRecoveryEmailRequest : public Wrappers::PFAccountManagementSendAccountRecoveryEmailRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementSendAccountRecoveryEmailRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementSendAccountRecoveryEmailRequest& input);
};

class UnlinkAndroidDeviceIDRequest : public Wrappers::PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkAndroidDeviceIDRequest& input);
};

class UnlinkAppleRequest : public Wrappers::PFAccountManagementUnlinkAppleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkAppleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkAppleRequest& input);
};

class UnlinkCustomIDRequest : public Wrappers::PFAccountManagementUnlinkCustomIDRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkCustomIDRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkCustomIDRequest& input);
};

class UnlinkFacebookAccountRequest : public Wrappers::PFAccountManagementUnlinkFacebookAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkFacebookAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkFacebookAccountRequest& input);
};

class UnlinkFacebookInstantGamesIdRequest : public Wrappers::PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkFacebookInstantGamesIdRequest& input);
};

class UnlinkGameCenterAccountRequest : public Wrappers::PFAccountManagementUnlinkGameCenterAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkGameCenterAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkGameCenterAccountRequest& input);
};

class UnlinkGoogleAccountRequest : public Wrappers::PFAccountManagementUnlinkGoogleAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkGoogleAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkGoogleAccountRequest& input);
};

class UnlinkGooglePlayGamesServicesAccountRequest : public Wrappers::PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequest& input);
};

class UnlinkIOSDeviceIDRequest : public Wrappers::PFAccountManagementUnlinkIOSDeviceIDRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkIOSDeviceIDRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkIOSDeviceIDRequest& input);
};

class UnlinkKongregateAccountRequest : public Wrappers::PFAccountManagementUnlinkKongregateAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkKongregateAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkKongregateAccountRequest& input);
};

class ClientUnlinkNintendoServiceAccountRequest : public Wrappers::PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementClientUnlinkNintendoServiceAccountRequest& input);
};

class ClientUnlinkNintendoSwitchDeviceIdRequest : public Wrappers::PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequest& input);
};

class UnlinkOpenIdConnectRequest : public Wrappers::PFAccountManagementUnlinkOpenIdConnectRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkOpenIdConnectRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkOpenIdConnectRequest& input);
};

class ClientUnlinkPSNAccountRequest : public Wrappers::PFAccountManagementClientUnlinkPSNAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementClientUnlinkPSNAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementClientUnlinkPSNAccountRequest& input);
};

class UnlinkSteamAccountRequest : public Wrappers::PFAccountManagementUnlinkSteamAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkSteamAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkSteamAccountRequest& input);
};

class UnlinkTwitchAccountRequest : public Wrappers::PFAccountManagementUnlinkTwitchAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkTwitchAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkTwitchAccountRequest& input);
};

class ClientUnlinkXboxAccountRequest : public Wrappers::PFAccountManagementClientUnlinkXboxAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementClientUnlinkXboxAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementClientUnlinkXboxAccountRequest& input);
};

class ClientUpdateAvatarUrlRequest : public Wrappers::PFAccountManagementClientUpdateAvatarUrlRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementClientUpdateAvatarUrlRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementClientUpdateAvatarUrlRequest& input);
};

class UpdateUserTitleDisplayNameRequest : public Wrappers::PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUpdateUserTitleDisplayNameRequest& input);
};

class UpdateUserTitleDisplayNameResult : public Wrappers::PFAccountManagementUpdateUserTitleDisplayNameResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementUpdateUserTitleDisplayNameResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUpdateUserTitleDisplayNameResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementUpdateUserTitleDisplayNameResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementUpdateUserTitleDisplayNameResult& model);
    static HRESULT Copy(const PFAccountManagementUpdateUserTitleDisplayNameResult& input, PFAccountManagementUpdateUserTitleDisplayNameResult& output, ModelBuffer& buffer);
};

class BanRequest : public Wrappers::PFAccountManagementBanRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementBanRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementBanRequest& input);
};

class BanUsersRequest : public Wrappers::PFAccountManagementBanUsersRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementBanUsersRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementBanUsersRequest& input);
};

class BanInfo : public Wrappers::PFAccountManagementBanInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementBanInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementBanInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementBanInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementBanInfo& model);
    static HRESULT Copy(const PFAccountManagementBanInfo& input, PFAccountManagementBanInfo& output, ModelBuffer& buffer);
};

class BanUsersResult : public Wrappers::PFAccountManagementBanUsersResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementBanUsersResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementBanUsersResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementBanUsersResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementBanUsersResult& model);
    static HRESULT Copy(const PFAccountManagementBanUsersResult& input, PFAccountManagementBanUsersResult& output, ModelBuffer& buffer);
};

class DeletePlayerRequest : public Wrappers::PFAccountManagementDeletePlayerRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementDeletePlayerRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementDeletePlayerRequest& input);
};

class GetServerCustomIDsFromPlayFabIDsRequest : public Wrappers::PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequest& input);
};

class ServerCustomIDPlayFabIDPair : public Wrappers::PFAccountManagementServerCustomIDPlayFabIDPairWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementServerCustomIDPlayFabIDPair>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerCustomIDPlayFabIDPairWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementServerCustomIDPlayFabIDPair const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementServerCustomIDPlayFabIDPair& model);
    static HRESULT Copy(const PFAccountManagementServerCustomIDPlayFabIDPair& input, PFAccountManagementServerCustomIDPlayFabIDPair& output, ModelBuffer& buffer);
};

class GetServerCustomIDsFromPlayFabIDsResult : public Wrappers::PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult& model);
    static HRESULT Copy(const PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult& input, PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult& output, ModelBuffer& buffer);
};

class GetUserAccountInfoRequest : public Wrappers::PFAccountManagementGetUserAccountInfoRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetUserAccountInfoRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetUserAccountInfoRequest& input);
};

class GetUserAccountInfoResult : public Wrappers::PFAccountManagementGetUserAccountInfoResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetUserAccountInfoResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetUserAccountInfoResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetUserAccountInfoResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetUserAccountInfoResult& model);
    static HRESULT Copy(const PFAccountManagementGetUserAccountInfoResult& input, PFAccountManagementGetUserAccountInfoResult& output, ModelBuffer& buffer);
};

class GetUserBansRequest : public Wrappers::PFAccountManagementGetUserBansRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetUserBansRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetUserBansRequest& input);
};

class GetUserBansResult : public Wrappers::PFAccountManagementGetUserBansResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetUserBansResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetUserBansResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetUserBansResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetUserBansResult& model);
    static HRESULT Copy(const PFAccountManagementGetUserBansResult& input, PFAccountManagementGetUserBansResult& output, ModelBuffer& buffer);
};

class ServerLinkNintendoServiceAccountRequest : public Wrappers::PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementServerLinkNintendoServiceAccountRequest& input);
};

class ServerLinkNintendoSwitchDeviceIdRequest : public Wrappers::PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementServerLinkNintendoSwitchDeviceIdRequest& input);
};

class ServerLinkPSNAccountRequest : public Wrappers::PFAccountManagementServerLinkPSNAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerLinkPSNAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementServerLinkPSNAccountRequest& input);
};

class LinkPSNIdRequest : public Wrappers::PFAccountManagementLinkPSNIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkPSNIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkPSNIdRequest& input);
};

class LinkServerCustomIdRequest : public Wrappers::PFAccountManagementLinkServerCustomIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkServerCustomIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkServerCustomIdRequest& input);
};

class LinkSteamIdRequest : public Wrappers::PFAccountManagementLinkSteamIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementLinkSteamIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementLinkSteamIdRequest& input);
};

class ServerLinkXboxAccountRequest : public Wrappers::PFAccountManagementServerLinkXboxAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerLinkXboxAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementServerLinkXboxAccountRequest& input);
};

class RevokeAllBansForUserRequest : public Wrappers::PFAccountManagementRevokeAllBansForUserRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementRevokeAllBansForUserRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementRevokeAllBansForUserRequest& input);
};

class RevokeAllBansForUserResult : public Wrappers::PFAccountManagementRevokeAllBansForUserResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementRevokeAllBansForUserResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementRevokeAllBansForUserResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementRevokeAllBansForUserResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementRevokeAllBansForUserResult& model);
    static HRESULT Copy(const PFAccountManagementRevokeAllBansForUserResult& input, PFAccountManagementRevokeAllBansForUserResult& output, ModelBuffer& buffer);
};

class RevokeBansRequest : public Wrappers::PFAccountManagementRevokeBansRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementRevokeBansRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementRevokeBansRequest& input);
};

class RevokeBansResult : public Wrappers::PFAccountManagementRevokeBansResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementRevokeBansResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementRevokeBansResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementRevokeBansResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementRevokeBansResult& model);
    static HRESULT Copy(const PFAccountManagementRevokeBansResult& input, PFAccountManagementRevokeBansResult& output, ModelBuffer& buffer);
};

class SendCustomAccountRecoveryEmailRequest : public Wrappers::PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementSendCustomAccountRecoveryEmailRequest& input);
};

class SendEmailFromTemplateRequest : public Wrappers::PFAccountManagementSendEmailFromTemplateRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementSendEmailFromTemplateRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementSendEmailFromTemplateRequest& input);
};

class ServerUnlinkNintendoServiceAccountRequest : public Wrappers::PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementServerUnlinkNintendoServiceAccountRequest& input);
};

class ServerUnlinkNintendoSwitchDeviceIdRequest : public Wrappers::PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequest& input);
};

class ServerUnlinkPSNAccountRequest : public Wrappers::PFAccountManagementServerUnlinkPSNAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerUnlinkPSNAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementServerUnlinkPSNAccountRequest& input);
};

class UnlinkServerCustomIdRequest : public Wrappers::PFAccountManagementUnlinkServerCustomIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkServerCustomIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkServerCustomIdRequest& input);
};

class UnlinkSteamIdRequest : public Wrappers::PFAccountManagementUnlinkSteamIdRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUnlinkSteamIdRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUnlinkSteamIdRequest& input);
};

class ServerUnlinkXboxAccountRequest : public Wrappers::PFAccountManagementServerUnlinkXboxAccountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerUnlinkXboxAccountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementServerUnlinkXboxAccountRequest& input);
};

class ServerUpdateAvatarUrlRequest : public Wrappers::PFAccountManagementServerUpdateAvatarUrlRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementServerUpdateAvatarUrlRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementServerUpdateAvatarUrlRequest& input);
};

class UpdateBanRequest : public Wrappers::PFAccountManagementUpdateBanRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUpdateBanRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUpdateBanRequest& input);
};

class UpdateBansRequest : public Wrappers::PFAccountManagementUpdateBansRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUpdateBansRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementUpdateBansRequest& input);
};

class UpdateBansResult : public Wrappers::PFAccountManagementUpdateBansResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementUpdateBansResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementUpdateBansResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementUpdateBansResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementUpdateBansResult& model);
    static HRESULT Copy(const PFAccountManagementUpdateBansResult& input, PFAccountManagementUpdateBansResult& output, ModelBuffer& buffer);
};

class GetTitlePlayersFromXboxLiveIDsRequest : public Wrappers::PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequest& input);
};

class GetTitlePlayersFromProviderIDsResponse : public Wrappers::PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFAccountManagementGetTitlePlayersFromProviderIDsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFAccountManagementGetTitlePlayersFromProviderIDsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFAccountManagementGetTitlePlayersFromProviderIDsResponse& model);
    static HRESULT Copy(const PFAccountManagementGetTitlePlayersFromProviderIDsResponse& input, PFAccountManagementGetTitlePlayersFromProviderIDsResponse& output, ModelBuffer& buffer);
};

} // namespace AccountManagement

// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

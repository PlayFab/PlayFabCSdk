#pragma once

#include <playfab/services/cpp/MultiplayerServerTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace MultiplayerServer
{

// MultiplayerServer Classes
class ListBuildAliasesRequest : public Wrappers::PFMultiplayerServerListBuildAliasesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerListBuildAliasesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFMultiplayerServerListBuildAliasesRequest& input);
};

class BuildSelectionCriterion : public Wrappers::PFMultiplayerServerBuildSelectionCriterionWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerBuildSelectionCriterion>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerBuildSelectionCriterionWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerBuildSelectionCriterion const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerBuildSelectionCriterion& model);
    static HRESULT Copy(const PFMultiplayerServerBuildSelectionCriterion& input, PFMultiplayerServerBuildSelectionCriterion& output, ModelBuffer& buffer);
};

class BuildAliasDetailsResponse : public Wrappers::PFMultiplayerServerBuildAliasDetailsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerBuildAliasDetailsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerBuildAliasDetailsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerBuildAliasDetailsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerBuildAliasDetailsResponse& model);
    static HRESULT Copy(const PFMultiplayerServerBuildAliasDetailsResponse& input, PFMultiplayerServerBuildAliasDetailsResponse& output, ModelBuffer& buffer);
};

class ListBuildAliasesResponse : public Wrappers::PFMultiplayerServerListBuildAliasesResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerListBuildAliasesResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerListBuildAliasesResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerListBuildAliasesResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerListBuildAliasesResponse& model);
    static HRESULT Copy(const PFMultiplayerServerListBuildAliasesResponse& input, PFMultiplayerServerListBuildAliasesResponse& output, ModelBuffer& buffer);
};

class ListBuildSummariesRequest : public Wrappers::PFMultiplayerServerListBuildSummariesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerListBuildSummariesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFMultiplayerServerListBuildSummariesRequest& input);
};

class CurrentServerStats : public Wrappers::PFMultiplayerServerCurrentServerStatsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerCurrentServerStats>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerCurrentServerStatsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerCurrentServerStats const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerCurrentServerStats& model);
    static HRESULT Copy(const PFMultiplayerServerCurrentServerStats& input, PFMultiplayerServerCurrentServerStats& output, ModelBuffer& buffer);
};

class DynamicStandbyThreshold : public Wrappers::PFMultiplayerServerDynamicStandbyThresholdWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerDynamicStandbyThreshold>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerDynamicStandbyThresholdWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerDynamicStandbyThreshold const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerDynamicStandbyThreshold& model);
    static HRESULT Copy(const PFMultiplayerServerDynamicStandbyThreshold& input, PFMultiplayerServerDynamicStandbyThreshold& output, ModelBuffer& buffer);
};

class DynamicStandbySettings : public Wrappers::PFMultiplayerServerDynamicStandbySettingsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerDynamicStandbySettings>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerDynamicStandbySettingsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerDynamicStandbySettings const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerDynamicStandbySettings& model);
    static HRESULT Copy(const PFMultiplayerServerDynamicStandbySettings& input, PFMultiplayerServerDynamicStandbySettings& output, ModelBuffer& buffer);
};

class Schedule : public Wrappers::PFMultiplayerServerScheduleWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerSchedule>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerScheduleWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerSchedule const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerSchedule& model);
    static HRESULT Copy(const PFMultiplayerServerSchedule& input, PFMultiplayerServerSchedule& output, ModelBuffer& buffer);
};

class ScheduledStandbySettings : public Wrappers::PFMultiplayerServerScheduledStandbySettingsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerScheduledStandbySettings>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerScheduledStandbySettingsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerScheduledStandbySettings const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerScheduledStandbySettings& model);
    static HRESULT Copy(const PFMultiplayerServerScheduledStandbySettings& input, PFMultiplayerServerScheduledStandbySettings& output, ModelBuffer& buffer);
};

class BuildRegion : public Wrappers::PFMultiplayerServerBuildRegionWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerBuildRegion>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerBuildRegionWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerBuildRegion const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerBuildRegion& model);
    static HRESULT Copy(const PFMultiplayerServerBuildRegion& input, PFMultiplayerServerBuildRegion& output, ModelBuffer& buffer);
};

class BuildSummary : public Wrappers::PFMultiplayerServerBuildSummaryWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerBuildSummary>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerBuildSummaryWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerBuildSummary const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerBuildSummary& model);
    static HRESULT Copy(const PFMultiplayerServerBuildSummary& input, PFMultiplayerServerBuildSummary& output, ModelBuffer& buffer);
};

class ListBuildSummariesResponse : public Wrappers::PFMultiplayerServerListBuildSummariesResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerListBuildSummariesResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerListBuildSummariesResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerListBuildSummariesResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerListBuildSummariesResponse& model);
    static HRESULT Copy(const PFMultiplayerServerListBuildSummariesResponse& input, PFMultiplayerServerListBuildSummariesResponse& output, ModelBuffer& buffer);
};

class ListQosServersForTitleRequest : public Wrappers::PFMultiplayerServerListQosServersForTitleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerListQosServersForTitleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFMultiplayerServerListQosServersForTitleRequest& input);
};

class QosServer : public Wrappers::PFMultiplayerServerQosServerWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerQosServer>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerQosServerWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerQosServer const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerQosServer& model);
    static HRESULT Copy(const PFMultiplayerServerQosServer& input, PFMultiplayerServerQosServer& output, ModelBuffer& buffer);
};

class ListQosServersForTitleResponse : public Wrappers::PFMultiplayerServerListQosServersForTitleResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerListQosServersForTitleResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerListQosServersForTitleResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerListQosServersForTitleResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerListQosServersForTitleResponse& model);
    static HRESULT Copy(const PFMultiplayerServerListQosServersForTitleResponse& input, PFMultiplayerServerListQosServersForTitleResponse& output, ModelBuffer& buffer);
};

class BuildAliasParams : public Wrappers::PFMultiplayerServerBuildAliasParamsWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerBuildAliasParamsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFMultiplayerServerBuildAliasParams& input);
};

class RequestMultiplayerServerRequest : public Wrappers::PFMultiplayerServerRequestMultiplayerServerRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerRequestMultiplayerServerRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFMultiplayerServerRequestMultiplayerServerRequest& input);
};

class ConnectedPlayer : public Wrappers::PFMultiplayerServerConnectedPlayerWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerConnectedPlayer>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerConnectedPlayerWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerConnectedPlayer const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerConnectedPlayer& model);
    static HRESULT Copy(const PFMultiplayerServerConnectedPlayer& input, PFMultiplayerServerConnectedPlayer& output, ModelBuffer& buffer);
};

class Port : public Wrappers::PFMultiplayerServerPortWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerPort>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerPortWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerPort const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerPort& model);
    static HRESULT Copy(const PFMultiplayerServerPort& input, PFMultiplayerServerPort& output, ModelBuffer& buffer);
};

class PublicIpAddress : public Wrappers::PFMultiplayerServerPublicIpAddressWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerPublicIpAddress>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerPublicIpAddressWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerPublicIpAddress const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerPublicIpAddress& model);
    static HRESULT Copy(const PFMultiplayerServerPublicIpAddress& input, PFMultiplayerServerPublicIpAddress& output, ModelBuffer& buffer);
};

class RequestMultiplayerServerResponse : public Wrappers::PFMultiplayerServerRequestMultiplayerServerResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFMultiplayerServerRequestMultiplayerServerResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFMultiplayerServerRequestMultiplayerServerResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerRequestMultiplayerServerResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFMultiplayerServerRequestMultiplayerServerResponse& model);
    static HRESULT Copy(const PFMultiplayerServerRequestMultiplayerServerResponse& input, PFMultiplayerServerRequestMultiplayerServerResponse& output, ModelBuffer& buffer);
};

} // namespace MultiplayerServer

// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFMultiplayerServerAzureVmSize>
{
    static constexpr PFMultiplayerServerAzureVmSize maxValue = PFMultiplayerServerAzureVmSize::Standard_HB120rs_v3;
};

template<> struct EnumRange<PFMultiplayerServerProtocolType>
{
    static constexpr PFMultiplayerServerProtocolType maxValue = PFMultiplayerServerProtocolType::UDP;
};

} // namespace PlayFab

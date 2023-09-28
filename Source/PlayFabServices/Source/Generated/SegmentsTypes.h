#pragma once

#include <playfab/services/cpp/SegmentsTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Segments
{

// Segments Classes
class GetSegmentResult : public Wrappers::PFSegmentsGetSegmentResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsGetSegmentResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetSegmentResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsGetSegmentResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsGetSegmentResult& model);
    static HRESULT Copy(const PFSegmentsGetSegmentResult& input, PFSegmentsGetSegmentResult& output, ModelBuffer& buffer);
};

class GetPlayerSegmentsResult : public Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsGetPlayerSegmentsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsGetPlayerSegmentsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsGetPlayerSegmentsResult& model);
    static HRESULT Copy(const PFSegmentsGetPlayerSegmentsResult& input, PFSegmentsGetPlayerSegmentsResult& output, ModelBuffer& buffer);
};

class GetPlayerTagsRequest : public Wrappers::PFSegmentsGetPlayerTagsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayerTagsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSegmentsGetPlayerTagsRequest& input);
};

class GetPlayerTagsResult : public Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsGetPlayerTagsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsGetPlayerTagsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsGetPlayerTagsResult& model);
    static HRESULT Copy(const PFSegmentsGetPlayerTagsResult& input, PFSegmentsGetPlayerTagsResult& output, ModelBuffer& buffer);
};

class AddPlayerTagRequest : public Wrappers::PFSegmentsAddPlayerTagRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsAddPlayerTagRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSegmentsAddPlayerTagRequest& input);
};

class GetAllSegmentsResult : public Wrappers::PFSegmentsGetAllSegmentsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsGetAllSegmentsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetAllSegmentsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsGetAllSegmentsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsGetAllSegmentsResult& model);
    static HRESULT Copy(const PFSegmentsGetAllSegmentsResult& input, PFSegmentsGetAllSegmentsResult& output, ModelBuffer& buffer);
};

class GetPlayersSegmentsRequest : public Wrappers::PFSegmentsGetPlayersSegmentsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayersSegmentsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSegmentsGetPlayersSegmentsRequest& input);
};

class GetPlayersInSegmentRequest : public Wrappers::PFSegmentsGetPlayersInSegmentRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayersInSegmentRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSegmentsGetPlayersInSegmentRequest& input);
};

class AdCampaignAttribution : public Wrappers::PFSegmentsAdCampaignAttributionWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsAdCampaignAttribution>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsAdCampaignAttributionWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsAdCampaignAttribution const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsAdCampaignAttribution& model);
    static HRESULT Copy(const PFSegmentsAdCampaignAttribution& input, PFSegmentsAdCampaignAttribution& output, ModelBuffer& buffer);
};

class ContactEmailInfo : public Wrappers::PFSegmentsContactEmailInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsContactEmailInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsContactEmailInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsContactEmailInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsContactEmailInfo& model);
    static HRESULT Copy(const PFSegmentsContactEmailInfo& input, PFSegmentsContactEmailInfo& output, ModelBuffer& buffer);
};

class PlayerLinkedAccount : public Wrappers::PFSegmentsPlayerLinkedAccountWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsPlayerLinkedAccount>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsPlayerLinkedAccountWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsPlayerLinkedAccount const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsPlayerLinkedAccount& model);
    static HRESULT Copy(const PFSegmentsPlayerLinkedAccount& input, PFSegmentsPlayerLinkedAccount& output, ModelBuffer& buffer);
};

class PlayerLocation : public Wrappers::PFSegmentsPlayerLocationWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsPlayerLocation>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsPlayerLocationWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsPlayerLocation const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsPlayerLocation& model);
    static HRESULT Copy(const PFSegmentsPlayerLocation& input, PFSegmentsPlayerLocation& output, ModelBuffer& buffer);
};

class PlayerStatistic : public Wrappers::PFSegmentsPlayerStatisticWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsPlayerStatistic>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsPlayerStatisticWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsPlayerStatistic const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsPlayerStatistic& model);
    static HRESULT Copy(const PFSegmentsPlayerStatistic& input, PFSegmentsPlayerStatistic& output, ModelBuffer& buffer);
};

class PushNotificationRegistration : public Wrappers::PFSegmentsPushNotificationRegistrationWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsPushNotificationRegistration>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsPushNotificationRegistrationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsPushNotificationRegistration const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsPushNotificationRegistration& model);
    static HRESULT Copy(const PFSegmentsPushNotificationRegistration& input, PFSegmentsPushNotificationRegistration& output, ModelBuffer& buffer);
};

class PlayerProfile : public Wrappers::PFSegmentsPlayerProfileWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsPlayerProfile>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsPlayerProfileWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsPlayerProfile const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsPlayerProfile& model);
    static HRESULT Copy(const PFSegmentsPlayerProfile& input, PFSegmentsPlayerProfile& output, ModelBuffer& buffer);
};

class GetPlayersInSegmentResult : public Wrappers::PFSegmentsGetPlayersInSegmentResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsGetPlayersInSegmentResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayersInSegmentResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsGetPlayersInSegmentResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsGetPlayersInSegmentResult& model);
    static HRESULT Copy(const PFSegmentsGetPlayersInSegmentResult& input, PFSegmentsGetPlayersInSegmentResult& output, ModelBuffer& buffer);
};

class RemovePlayerTagRequest : public Wrappers::PFSegmentsRemovePlayerTagRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsRemovePlayerTagRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSegmentsRemovePlayerTagRequest& input);
};

} // namespace Segments

// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFSegmentsChurnRiskLevel>
{
    static constexpr PFSegmentsChurnRiskLevel maxValue = PFSegmentsChurnRiskLevel::HighRisk;
};

} // namespace PlayFab

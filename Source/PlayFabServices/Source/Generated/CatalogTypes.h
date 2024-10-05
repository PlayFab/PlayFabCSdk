#pragma once

#include <playfab/services/cpp/CatalogTypeWrappers.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Catalog
{

// Catalog Classes
class CatalogAlternateId : public Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogAlternateId>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogAlternateId& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogAlternateId const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogAlternateId& model);
    static HRESULT Copy(const PFCatalogCatalogAlternateId& input, PFCatalogCatalogAlternateId& output, ModelBuffer& buffer);
};

class Content : public Wrappers::PFCatalogContentWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogContent>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogContentWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogContent& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogContent const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogContent& model);
    static HRESULT Copy(const PFCatalogContent& input, PFCatalogContent& output, ModelBuffer& buffer);
};

class DeepLink : public Wrappers::PFCatalogDeepLinkWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogDeepLink>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogDeepLinkWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogDeepLink& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogDeepLink const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogDeepLink& model);
    static HRESULT Copy(const PFCatalogDeepLink& input, PFCatalogDeepLink& output, ModelBuffer& buffer);
};

class Image : public Wrappers::PFCatalogImageWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogImage>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogImageWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogImage& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogImage const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogImage& model);
    static HRESULT Copy(const PFCatalogImage& input, PFCatalogImage& output, ModelBuffer& buffer);
};

class CatalogPriceAmount : public Wrappers::PFCatalogCatalogPriceAmountWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogPriceAmount>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogPriceAmountWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogPriceAmount& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogPriceAmount const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogPriceAmount& model);
    static HRESULT Copy(const PFCatalogCatalogPriceAmount& input, PFCatalogCatalogPriceAmount& output, ModelBuffer& buffer);
};

class CatalogPrice : public Wrappers::PFCatalogCatalogPriceWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogPrice>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogPriceWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogPrice& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogPrice const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogPrice& model);
    static HRESULT Copy(const PFCatalogCatalogPrice& input, PFCatalogCatalogPrice& output, ModelBuffer& buffer);
};

class CatalogPriceOptions : public Wrappers::PFCatalogCatalogPriceOptionsWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogPriceOptions>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogPriceOptionsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogPriceOptions& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogPriceOptions const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogPriceOptions& model);
    static HRESULT Copy(const PFCatalogCatalogPriceOptions& input, PFCatalogCatalogPriceOptions& output, ModelBuffer& buffer);
};

class CatalogItemReference : public Wrappers::PFCatalogCatalogItemReferenceWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogItemReference>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogItemReferenceWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogItemReference& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogItemReference const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogItemReference& model);
    static HRESULT Copy(const PFCatalogCatalogItemReference& input, PFCatalogCatalogItemReference& output, ModelBuffer& buffer);
};

class KeywordSet : public Wrappers::PFCatalogKeywordSetWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogKeywordSet>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogKeywordSetWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogKeywordSet& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogKeywordSet const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogKeywordSet& model);
    static HRESULT Copy(const PFCatalogKeywordSet& input, PFCatalogKeywordSet& output, ModelBuffer& buffer);
};

class ModerationState : public Wrappers::PFCatalogModerationStateWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogModerationState>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogModerationStateWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogModerationState& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogModerationState const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogModerationState& model);
    static HRESULT Copy(const PFCatalogModerationState& input, PFCatalogModerationState& output, ModelBuffer& buffer);
};

class Rating : public Wrappers::PFCatalogRatingWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogRating>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogRatingWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogRating& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogRating const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogRating& model);
    static HRESULT Copy(const PFCatalogRating& input, PFCatalogRating& output, ModelBuffer& buffer);
};

class FilterOptions : public Wrappers::PFCatalogFilterOptionsWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogFilterOptions>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogFilterOptionsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogFilterOptions& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogFilterOptions const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogFilterOptions& model);
    static HRESULT Copy(const PFCatalogFilterOptions& input, PFCatalogFilterOptions& output, ModelBuffer& buffer);
};

class Permissions : public Wrappers::PFCatalogPermissionsWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogPermissions>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogPermissionsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogPermissions& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogPermissions const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogPermissions& model);
    static HRESULT Copy(const PFCatalogPermissions& input, PFCatalogPermissions& output, ModelBuffer& buffer);
};

class CatalogPriceAmountOverride : public Wrappers::PFCatalogCatalogPriceAmountOverrideWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogPriceAmountOverride>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogPriceAmountOverrideWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogPriceAmountOverride& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogPriceAmountOverride const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogPriceAmountOverride& model);
    static HRESULT Copy(const PFCatalogCatalogPriceAmountOverride& input, PFCatalogCatalogPriceAmountOverride& output, ModelBuffer& buffer);
};

class CatalogPriceOverride : public Wrappers::PFCatalogCatalogPriceOverrideWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogPriceOverride>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogPriceOverrideWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogPriceOverride& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogPriceOverride const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogPriceOverride& model);
    static HRESULT Copy(const PFCatalogCatalogPriceOverride& input, PFCatalogCatalogPriceOverride& output, ModelBuffer& buffer);
};

class CatalogPriceOptionsOverride : public Wrappers::PFCatalogCatalogPriceOptionsOverrideWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogPriceOptionsOverride>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogPriceOptionsOverrideWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogPriceOptionsOverride& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogPriceOptionsOverride const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogPriceOptionsOverride& model);
    static HRESULT Copy(const PFCatalogCatalogPriceOptionsOverride& input, PFCatalogCatalogPriceOptionsOverride& output, ModelBuffer& buffer);
};

class StoreDetails : public Wrappers::PFCatalogStoreDetailsWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogStoreDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogStoreDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogStoreDetails& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogStoreDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogStoreDetails& model);
    static HRESULT Copy(const PFCatalogStoreDetails& input, PFCatalogStoreDetails& output, ModelBuffer& buffer);
};

class CatalogItem : public Wrappers::PFCatalogCatalogItemWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogItem>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogItemWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogItem& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogItem const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogItem& model);
    static HRESULT Copy(const PFCatalogCatalogItem& input, PFCatalogCatalogItem& output, ModelBuffer& buffer);
};

class CreateDraftItemRequest : public Wrappers::PFCatalogCreateDraftItemRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCreateDraftItemRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCreateDraftItemRequest& input);
};

class CreateDraftItemResponse : public Wrappers::PFCatalogCreateDraftItemResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogCreateDraftItemResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCreateDraftItemResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCreateDraftItemResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCreateDraftItemResponse& model);
    static HRESULT Copy(const PFCatalogCreateDraftItemResponse& input, PFCatalogCreateDraftItemResponse& output, ModelBuffer& buffer);
};

class UploadInfo : public Wrappers::PFCatalogUploadInfoWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogUploadInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogUploadInfo& input);
};

class CreateUploadUrlsRequest : public Wrappers::PFCatalogCreateUploadUrlsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCreateUploadUrlsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCreateUploadUrlsRequest& input);
};

class UploadUrlMetadata : public Wrappers::PFCatalogUploadUrlMetadataWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogUploadUrlMetadata>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogUploadUrlMetadataWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogUploadUrlMetadata const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogUploadUrlMetadata& model);
    static HRESULT Copy(const PFCatalogUploadUrlMetadata& input, PFCatalogUploadUrlMetadata& output, ModelBuffer& buffer);
};

class CreateUploadUrlsResponse : public Wrappers::PFCatalogCreateUploadUrlsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogCreateUploadUrlsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCreateUploadUrlsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCreateUploadUrlsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCreateUploadUrlsResponse& model);
    static HRESULT Copy(const PFCatalogCreateUploadUrlsResponse& input, PFCatalogCreateUploadUrlsResponse& output, ModelBuffer& buffer);
};

class DeleteEntityItemReviewsRequest : public Wrappers::PFCatalogDeleteEntityItemReviewsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogDeleteEntityItemReviewsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogDeleteEntityItemReviewsRequest& input);
};

class DeleteItemRequest : public Wrappers::PFCatalogDeleteItemRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogDeleteItemRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogDeleteItemRequest& input);
};

class GetCatalogConfigRequest : public Wrappers::PFCatalogGetCatalogConfigRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetCatalogConfigRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetCatalogConfigRequest& input);
};

class CatalogSpecificConfig : public Wrappers::PFCatalogCatalogSpecificConfigWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogSpecificConfig>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogSpecificConfigWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogSpecificConfig& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogSpecificConfig const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogSpecificConfig& model);
    static HRESULT Copy(const PFCatalogCatalogSpecificConfig& input, PFCatalogCatalogSpecificConfig& output, ModelBuffer& buffer);
};

class DeepLinkFormat : public Wrappers::PFCatalogDeepLinkFormatWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogDeepLinkFormat>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogDeepLinkFormatWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogDeepLinkFormat& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogDeepLinkFormat const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogDeepLinkFormat& model);
    static HRESULT Copy(const PFCatalogDeepLinkFormat& input, PFCatalogDeepLinkFormat& output, ModelBuffer& buffer);
};

class DisplayPropertyIndexInfo : public Wrappers::PFCatalogDisplayPropertyIndexInfoWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogDisplayPropertyIndexInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogDisplayPropertyIndexInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogDisplayPropertyIndexInfo& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogDisplayPropertyIndexInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogDisplayPropertyIndexInfo& model);
    static HRESULT Copy(const PFCatalogDisplayPropertyIndexInfo& input, PFCatalogDisplayPropertyIndexInfo& output, ModelBuffer& buffer);
};

class FileConfig : public Wrappers::PFCatalogFileConfigWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogFileConfig>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogFileConfigWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogFileConfig& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogFileConfig const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogFileConfig& model);
    static HRESULT Copy(const PFCatalogFileConfig& input, PFCatalogFileConfig& output, ModelBuffer& buffer);
};

class ImageConfig : public Wrappers::PFCatalogImageConfigWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogImageConfig>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogImageConfigWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogImageConfig& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogImageConfig const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogImageConfig& model);
    static HRESULT Copy(const PFCatalogImageConfig& input, PFCatalogImageConfig& output, ModelBuffer& buffer);
};

class CategoryRatingConfig : public Wrappers::PFCatalogCategoryRatingConfigWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCategoryRatingConfig>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCategoryRatingConfigWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCategoryRatingConfig& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCategoryRatingConfig const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCategoryRatingConfig& model);
    static HRESULT Copy(const PFCatalogCategoryRatingConfig& input, PFCatalogCategoryRatingConfig& output, ModelBuffer& buffer);
};

class ReviewConfig : public Wrappers::PFCatalogReviewConfigWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogReviewConfig>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogReviewConfigWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogReviewConfig& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogReviewConfig const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogReviewConfig& model);
    static HRESULT Copy(const PFCatalogReviewConfig& input, PFCatalogReviewConfig& output, ModelBuffer& buffer);
};

class UserGeneratedContentSpecificConfig : public Wrappers::PFCatalogUserGeneratedContentSpecificConfigWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogUserGeneratedContentSpecificConfig>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogUserGeneratedContentSpecificConfigWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogUserGeneratedContentSpecificConfig& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogUserGeneratedContentSpecificConfig const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogUserGeneratedContentSpecificConfig& model);
    static HRESULT Copy(const PFCatalogUserGeneratedContentSpecificConfig& input, PFCatalogUserGeneratedContentSpecificConfig& output, ModelBuffer& buffer);
};

class CatalogConfig : public Wrappers::PFCatalogCatalogConfigWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogCatalogConfig>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogCatalogConfigWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogCatalogConfig& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogCatalogConfig const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogCatalogConfig& model);
    static HRESULT Copy(const PFCatalogCatalogConfig& input, PFCatalogCatalogConfig& output, ModelBuffer& buffer);
};

class GetCatalogConfigResponse : public Wrappers::PFCatalogGetCatalogConfigResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetCatalogConfigResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetCatalogConfigResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetCatalogConfigResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetCatalogConfigResponse& model);
    static HRESULT Copy(const PFCatalogGetCatalogConfigResponse& input, PFCatalogGetCatalogConfigResponse& output, ModelBuffer& buffer);
};

class GetDraftItemRequest : public Wrappers::PFCatalogGetDraftItemRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetDraftItemRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetDraftItemRequest& input);
};

class GetDraftItemResponse : public Wrappers::PFCatalogGetDraftItemResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetDraftItemResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetDraftItemResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetDraftItemResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetDraftItemResponse& model);
    static HRESULT Copy(const PFCatalogGetDraftItemResponse& input, PFCatalogGetDraftItemResponse& output, ModelBuffer& buffer);
};

class GetDraftItemsRequest : public Wrappers::PFCatalogGetDraftItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetDraftItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetDraftItemsRequest& input);
};

class GetDraftItemsResponse : public Wrappers::PFCatalogGetDraftItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetDraftItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetDraftItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetDraftItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetDraftItemsResponse& model);
    static HRESULT Copy(const PFCatalogGetDraftItemsResponse& input, PFCatalogGetDraftItemsResponse& output, ModelBuffer& buffer);
};

class GetEntityDraftItemsRequest : public Wrappers::PFCatalogGetEntityDraftItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetEntityDraftItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetEntityDraftItemsRequest& input);
};

class GetEntityDraftItemsResponse : public Wrappers::PFCatalogGetEntityDraftItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetEntityDraftItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetEntityDraftItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetEntityDraftItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetEntityDraftItemsResponse& model);
    static HRESULT Copy(const PFCatalogGetEntityDraftItemsResponse& input, PFCatalogGetEntityDraftItemsResponse& output, ModelBuffer& buffer);
};

class GetEntityItemReviewRequest : public Wrappers::PFCatalogGetEntityItemReviewRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetEntityItemReviewRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetEntityItemReviewRequest& input);
};

class Review : public Wrappers::PFCatalogReviewWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFCatalogReview>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogReviewWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogReview& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogReview const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogReview& model);
    static HRESULT Copy(const PFCatalogReview& input, PFCatalogReview& output, ModelBuffer& buffer);
};

class GetEntityItemReviewResponse : public Wrappers::PFCatalogGetEntityItemReviewResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetEntityItemReviewResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetEntityItemReviewResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetEntityItemReviewResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetEntityItemReviewResponse& model);
    static HRESULT Copy(const PFCatalogGetEntityItemReviewResponse& input, PFCatalogGetEntityItemReviewResponse& output, ModelBuffer& buffer);
};

class GetItemRequest : public Wrappers::PFCatalogGetItemRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetItemRequest& input);
};

class GetItemResponse : public Wrappers::PFCatalogGetItemResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetItemResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetItemResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetItemResponse& model);
    static HRESULT Copy(const PFCatalogGetItemResponse& input, PFCatalogGetItemResponse& output, ModelBuffer& buffer);
};

class GetItemContainersRequest : public Wrappers::PFCatalogGetItemContainersRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemContainersRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetItemContainersRequest& input);
};

class GetItemContainersResponse : public Wrappers::PFCatalogGetItemContainersResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetItemContainersResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemContainersResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetItemContainersResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetItemContainersResponse& model);
    static HRESULT Copy(const PFCatalogGetItemContainersResponse& input, PFCatalogGetItemContainersResponse& output, ModelBuffer& buffer);
};

class GetItemModerationStateRequest : public Wrappers::PFCatalogGetItemModerationStateRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemModerationStateRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetItemModerationStateRequest& input);
};

class GetItemModerationStateResponse : public Wrappers::PFCatalogGetItemModerationStateResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetItemModerationStateResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemModerationStateResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetItemModerationStateResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetItemModerationStateResponse& model);
    static HRESULT Copy(const PFCatalogGetItemModerationStateResponse& input, PFCatalogGetItemModerationStateResponse& output, ModelBuffer& buffer);
};

class GetItemPublishStatusRequest : public Wrappers::PFCatalogGetItemPublishStatusRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemPublishStatusRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetItemPublishStatusRequest& input);
};

class GetItemPublishStatusResponse : public Wrappers::PFCatalogGetItemPublishStatusResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetItemPublishStatusResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemPublishStatusResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetItemPublishStatusResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetItemPublishStatusResponse& model);
    static HRESULT Copy(const PFCatalogGetItemPublishStatusResponse& input, PFCatalogGetItemPublishStatusResponse& output, ModelBuffer& buffer);
};

class GetItemReviewsRequest : public Wrappers::PFCatalogGetItemReviewsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemReviewsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetItemReviewsRequest& input);
};

class GetItemReviewsResponse : public Wrappers::PFCatalogGetItemReviewsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetItemReviewsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemReviewsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetItemReviewsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetItemReviewsResponse& model);
    static HRESULT Copy(const PFCatalogGetItemReviewsResponse& input, PFCatalogGetItemReviewsResponse& output, ModelBuffer& buffer);
};

class GetItemReviewSummaryRequest : public Wrappers::PFCatalogGetItemReviewSummaryRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemReviewSummaryRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetItemReviewSummaryRequest& input);
};

class GetItemReviewSummaryResponse : public Wrappers::PFCatalogGetItemReviewSummaryResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetItemReviewSummaryResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemReviewSummaryResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetItemReviewSummaryResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetItemReviewSummaryResponse& model);
    static HRESULT Copy(const PFCatalogGetItemReviewSummaryResponse& input, PFCatalogGetItemReviewSummaryResponse& output, ModelBuffer& buffer);
};

class GetItemsRequest : public Wrappers::PFCatalogGetItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogGetItemsRequest& input);
};

class GetItemsResponse : public Wrappers::PFCatalogGetItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogGetItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogGetItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogGetItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogGetItemsResponse& model);
    static HRESULT Copy(const PFCatalogGetItemsResponse& input, PFCatalogGetItemsResponse& output, ModelBuffer& buffer);
};

class PublishDraftItemRequest : public Wrappers::PFCatalogPublishDraftItemRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogPublishDraftItemRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogPublishDraftItemRequest& input);
};

class ReportItemRequest : public Wrappers::PFCatalogReportItemRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogReportItemRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogReportItemRequest& input);
};

class ReportItemReviewRequest : public Wrappers::PFCatalogReportItemReviewRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogReportItemReviewRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogReportItemReviewRequest& input);
};

class ReviewItemRequest : public Wrappers::PFCatalogReviewItemRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogReviewItemRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogReviewItemRequest& input);
};

class StoreReference : public Wrappers::PFCatalogStoreReferenceWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogStoreReferenceWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogStoreReference& input);
};

class SearchItemsRequest : public Wrappers::PFCatalogSearchItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogSearchItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogSearchItemsRequest& input);
};

class SearchItemsResponse : public Wrappers::PFCatalogSearchItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogSearchItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogSearchItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogSearchItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogSearchItemsResponse& model);
    static HRESULT Copy(const PFCatalogSearchItemsResponse& input, PFCatalogSearchItemsResponse& output, ModelBuffer& buffer);
};

class SetItemModerationStateRequest : public Wrappers::PFCatalogSetItemModerationStateRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogSetItemModerationStateRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogSetItemModerationStateRequest& input);
};

class SubmitItemReviewVoteRequest : public Wrappers::PFCatalogSubmitItemReviewVoteRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogSubmitItemReviewVoteRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogSubmitItemReviewVoteRequest& input);
};

class ReviewTakedown : public Wrappers::PFCatalogReviewTakedownWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogReviewTakedownWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogReviewTakedown& input);
};

class TakedownItemReviewsRequest : public Wrappers::PFCatalogTakedownItemReviewsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogTakedownItemReviewsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogTakedownItemReviewsRequest& input);
};

class UpdateCatalogConfigRequest : public Wrappers::PFCatalogUpdateCatalogConfigRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogUpdateCatalogConfigRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogUpdateCatalogConfigRequest& input);
};

class UpdateDraftItemRequest : public Wrappers::PFCatalogUpdateDraftItemRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogUpdateDraftItemRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCatalogUpdateDraftItemRequest& input);
};

class UpdateDraftItemResponse : public Wrappers::PFCatalogUpdateDraftItemResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCatalogUpdateDraftItemResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFCatalogUpdateDraftItemResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCatalogUpdateDraftItemResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCatalogUpdateDraftItemResponse& model);
    static HRESULT Copy(const PFCatalogUpdateDraftItemResponse& input, PFCatalogUpdateDraftItemResponse& output, ModelBuffer& buffer);
};

} // namespace Catalog

// Json serialization helpers

// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFCatalogModerationStatus>
{
    static constexpr PFCatalogModerationStatus maxValue = PFCatalogModerationStatus::Rejected;
};

template<> struct EnumRange<PFCatalogDisplayPropertyType>
{
    static constexpr PFCatalogDisplayPropertyType maxValue = PFCatalogDisplayPropertyType::SearchString;
};

template<> struct EnumRange<PFCatalogPublishResult>
{
    static constexpr PFCatalogPublishResult maxValue = PFCatalogPublishResult::Canceled;
};

template<> struct EnumRange<PFCatalogConcernCategory>
{
    static constexpr PFCatalogConcernCategory maxValue = PFCatalogConcernCategory::Profanity;
};

template<> struct EnumRange<PFCatalogHelpfulnessVote>
{
    static constexpr PFCatalogHelpfulnessVote maxValue = PFCatalogHelpfulnessVote::Helpful;
};


} // namespace PlayFab

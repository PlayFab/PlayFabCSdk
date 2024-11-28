#include "stdafx.h"
#include "CatalogTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Catalog
{

JsonValue CatalogAlternateId::ToJson() const
{
    return CatalogAlternateId::ToJson(this->Model());
}

JsonValue CatalogAlternateId::ToJson(const PFCatalogCatalogAlternateId& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    JsonUtils::ObjectAddMember(output, "Value", input.value);
    return output;
}

HRESULT CatalogAlternateId::FromJson(const JsonValue& input)
{
    String type{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", type));
    this->SetType(std::move(type));

    String value{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Value", value));
    this->SetValue(std::move(value));

    return S_OK;
}

size_t CatalogAlternateId::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogAlternateId const*> CatalogAlternateId::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogAlternateId>(&this->Model());
}

size_t CatalogAlternateId::RequiredBufferSize(const PFCatalogCatalogAlternateId& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.type)
    {
        requiredSize += (std::strlen(model.type) + 1);
    }
    if (model.value)
    {
        requiredSize += (std::strlen(model.value) + 1);
    }
    return requiredSize;
}

HRESULT CatalogAlternateId::Copy(const PFCatalogCatalogAlternateId& input, PFCatalogCatalogAlternateId& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.type);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.type = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.value);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.value = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue Content::ToJson() const
{
    return Content::ToJson(this->Model());
}

JsonValue Content::ToJson(const PFCatalogContent& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember(output, "MaxClientVersion", input.maxClientVersion);
    JsonUtils::ObjectAddMember(output, "MinClientVersion", input.minClientVersion);
    JsonUtils::ObjectAddMemberArray(output, "Tags", input.tags, input.tagsCount);
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    JsonUtils::ObjectAddMember(output, "Url", input.url);
    return output;
}

HRESULT Content::FromJson(const JsonValue& input)
{
    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    String maxClientVersion{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MaxClientVersion", maxClientVersion));
    this->SetMaxClientVersion(std::move(maxClientVersion));

    String minClientVersion{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MinClientVersion", minClientVersion));
    this->SetMinClientVersion(std::move(minClientVersion));

    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    String type{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", type));
    this->SetType(std::move(type));

    String url{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Url", url));
    this->SetUrl(std::move(url));

    return S_OK;
}

size_t Content::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogContent const*> Content::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Content>(&this->Model());
}

size_t Content::RequiredBufferSize(const PFCatalogContent& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.maxClientVersion)
    {
        requiredSize += (std::strlen(model.maxClientVersion) + 1);
    }
    if (model.minClientVersion)
    {
        requiredSize += (std::strlen(model.minClientVersion) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    if (model.type)
    {
        requiredSize += (std::strlen(model.type) + 1);
    }
    if (model.url)
    {
        requiredSize += (std::strlen(model.url) + 1);
    }
    return requiredSize;
}

HRESULT Content::Copy(const PFCatalogContent& input, PFCatalogContent& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.id);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.maxClientVersion);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.maxClientVersion = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.minClientVersion);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.minClientVersion = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.type);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.type = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.url);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.url = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DeepLink::ToJson() const
{
    return DeepLink::ToJson(this->Model());
}

JsonValue DeepLink::ToJson(const PFCatalogDeepLink& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Platform", input.platform);
    JsonUtils::ObjectAddMember(output, "Url", input.url);
    return output;
}

HRESULT DeepLink::FromJson(const JsonValue& input)
{
    String platform{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Platform", platform));
    this->SetPlatform(std::move(platform));

    String url{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Url", url));
    this->SetUrl(std::move(url));

    return S_OK;
}

size_t DeepLink::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogDeepLink const*> DeepLink::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DeepLink>(&this->Model());
}

size_t DeepLink::RequiredBufferSize(const PFCatalogDeepLink& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.platform)
    {
        requiredSize += (std::strlen(model.platform) + 1);
    }
    if (model.url)
    {
        requiredSize += (std::strlen(model.url) + 1);
    }
    return requiredSize;
}

HRESULT DeepLink::Copy(const PFCatalogDeepLink& input, PFCatalogDeepLink& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.platform);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.platform = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.url);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.url = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue Image::ToJson() const
{
    return Image::ToJson(this->Model());
}

JsonValue Image::ToJson(const PFCatalogImage& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember(output, "Tag", input.tag);
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    JsonUtils::ObjectAddMember(output, "Url", input.url);
    return output;
}

HRESULT Image::FromJson(const JsonValue& input)
{
    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    String tag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tag", tag));
    this->SetTag(std::move(tag));

    String type{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", type));
    this->SetType(std::move(type));

    String url{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Url", url));
    this->SetUrl(std::move(url));

    return S_OK;
}

size_t Image::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogImage const*> Image::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Image>(&this->Model());
}

size_t Image::RequiredBufferSize(const PFCatalogImage& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.tag)
    {
        requiredSize += (std::strlen(model.tag) + 1);
    }
    if (model.type)
    {
        requiredSize += (std::strlen(model.type) + 1);
    }
    if (model.url)
    {
        requiredSize += (std::strlen(model.url) + 1);
    }
    return requiredSize;
}

HRESULT Image::Copy(const PFCatalogImage& input, PFCatalogImage& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.id);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.tag);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.type);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.type = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.url);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.url = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CatalogPriceAmount::ToJson() const
{
    return CatalogPriceAmount::ToJson(this->Model());
}

JsonValue CatalogPriceAmount::ToJson(const PFCatalogCatalogPriceAmount& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "ItemId", input.itemId);
    return output;
}

HRESULT CatalogPriceAmount::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Amount", this->m_model.amount));

    String itemId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemId", itemId));
    this->SetItemId(std::move(itemId));

    return S_OK;
}

size_t CatalogPriceAmount::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogPriceAmount const*> CatalogPriceAmount::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogPriceAmount>(&this->Model());
}

size_t CatalogPriceAmount::RequiredBufferSize(const PFCatalogCatalogPriceAmount& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.itemId)
    {
        requiredSize += (std::strlen(model.itemId) + 1);
    }
    return requiredSize;
}

HRESULT CatalogPriceAmount::Copy(const PFCatalogCatalogPriceAmount& input, PFCatalogCatalogPriceAmount& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.itemId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CatalogPrice::ToJson() const
{
    return CatalogPrice::ToJson(this->Model());
}

JsonValue CatalogPrice::ToJson(const PFCatalogCatalogPrice& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<CatalogPriceAmount>(output, "Amounts", input.amounts, input.amountsCount);
    JsonUtils::ObjectAddMember(output, "UnitAmount", input.unitAmount);
    JsonUtils::ObjectAddMember(output, "UnitDurationInSeconds", input.unitDurationInSeconds);
    return output;
}

HRESULT CatalogPrice::FromJson(const JsonValue& input)
{
    ModelVector<CatalogPriceAmount> amounts{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogPriceAmount>(input, "Amounts", amounts));
    this->SetAmounts(std::move(amounts));

    std::optional<int32_t> unitAmount{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UnitAmount", unitAmount));
    this->SetUnitAmount(std::move(unitAmount));

    std::optional<double> unitDurationInSeconds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UnitDurationInSeconds", unitDurationInSeconds));
    this->SetUnitDurationInSeconds(std::move(unitDurationInSeconds));

    return S_OK;
}

size_t CatalogPrice::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogPrice const*> CatalogPrice::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogPrice>(&this->Model());
}

size_t CatalogPrice::RequiredBufferSize(const PFCatalogCatalogPrice& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCatalogCatalogPriceAmount*) + sizeof(PFCatalogCatalogPriceAmount*) * model.amountsCount);
    for (size_t i = 0; i < model.amountsCount; ++i)
    {
        requiredSize += CatalogPriceAmount::RequiredBufferSize(*model.amounts[i]);
    }
    if (model.unitAmount)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.unitDurationInSeconds)
    {
        requiredSize += (alignof(double) + sizeof(double));
    }
    return requiredSize;
}

HRESULT CatalogPrice::Copy(const PFCatalogCatalogPrice& input, PFCatalogCatalogPrice& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CatalogPriceAmount>(input.amounts, input.amountsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.amounts = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.unitAmount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.unitAmount = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.unitDurationInSeconds);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.unitDurationInSeconds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CatalogPriceOptions::ToJson() const
{
    return CatalogPriceOptions::ToJson(this->Model());
}

JsonValue CatalogPriceOptions::ToJson(const PFCatalogCatalogPriceOptions& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<CatalogPrice>(output, "Prices", input.prices, input.pricesCount);
    return output;
}

HRESULT CatalogPriceOptions::FromJson(const JsonValue& input)
{
    ModelVector<CatalogPrice> prices{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogPrice>(input, "Prices", prices));
    this->SetPrices(std::move(prices));

    return S_OK;
}

size_t CatalogPriceOptions::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogPriceOptions const*> CatalogPriceOptions::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogPriceOptions>(&this->Model());
}

size_t CatalogPriceOptions::RequiredBufferSize(const PFCatalogCatalogPriceOptions& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCatalogCatalogPrice*) + sizeof(PFCatalogCatalogPrice*) * model.pricesCount);
    for (size_t i = 0; i < model.pricesCount; ++i)
    {
        requiredSize += CatalogPrice::RequiredBufferSize(*model.prices[i]);
    }
    return requiredSize;
}

HRESULT CatalogPriceOptions::Copy(const PFCatalogCatalogPriceOptions& input, PFCatalogCatalogPriceOptions& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CatalogPrice>(input.prices, input.pricesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.prices = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CatalogItemReference::ToJson() const
{
    return CatalogItemReference::ToJson(this->Model());
}

JsonValue CatalogItemReference::ToJson(const PFCatalogCatalogItemReference& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember<CatalogPriceOptions>(output, "PriceOptions", input.priceOptions);
    return output;
}

HRESULT CatalogItemReference::FromJson(const JsonValue& input)
{
    std::optional<int32_t> amount{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Amount", amount));
    this->SetAmount(std::move(amount));

    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    std::optional<CatalogPriceOptions> priceOptions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PriceOptions", priceOptions));
    if (priceOptions)
    {
        this->SetPriceOptions(std::move(*priceOptions));
    }

    return S_OK;
}

size_t CatalogItemReference::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogItemReference const*> CatalogItemReference::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogItemReference>(&this->Model());
}

size_t CatalogItemReference::RequiredBufferSize(const PFCatalogCatalogItemReference& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.amount)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.priceOptions)
    {
        requiredSize += CatalogPriceOptions::RequiredBufferSize(*model.priceOptions);
    }
    return requiredSize;
}

HRESULT CatalogItemReference::Copy(const PFCatalogCatalogItemReference& input, PFCatalogCatalogItemReference& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.amount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.amount = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.id);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<CatalogPriceOptions>(input.priceOptions);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.priceOptions = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue KeywordSet::ToJson() const
{
    return KeywordSet::ToJson(this->Model());
}

JsonValue KeywordSet::ToJson(const PFCatalogKeywordSet& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray(output, "Values", input.values, input.valuesCount);
    return output;
}

HRESULT KeywordSet::FromJson(const JsonValue& input)
{
    CStringVector values{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Values", values));
    this->SetValues(std::move(values));

    return S_OK;
}

size_t KeywordSet::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogKeywordSet const*> KeywordSet::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<KeywordSet>(&this->Model());
}

size_t KeywordSet::RequiredBufferSize(const PFCatalogKeywordSet& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.valuesCount);
    for (size_t i = 0; i < model.valuesCount; ++i)
    {
        requiredSize += (std::strlen(model.values[i]) + 1);
    }
    return requiredSize;
}

HRESULT KeywordSet::Copy(const PFCatalogKeywordSet& input, PFCatalogKeywordSet& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.values, input.valuesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.values = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ModerationState::ToJson() const
{
    return ModerationState::ToJson(this->Model());
}

JsonValue ModerationState::ToJson(const PFCatalogModerationState& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberTime(output, "LastModifiedDate", input.lastModifiedDate);
    JsonUtils::ObjectAddMember(output, "Reason", input.reason);
    JsonUtils::ObjectAddMember(output, "Status", input.status);
    return output;
}

HRESULT ModerationState::FromJson(const JsonValue& input)
{
    std::optional<time_t> lastModifiedDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastModifiedDate", lastModifiedDate));
    this->SetLastModifiedDate(std::move(lastModifiedDate));

    String reason{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Reason", reason));
    this->SetReason(std::move(reason));

    std::optional<PFCatalogModerationStatus> status{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Status", status));
    this->SetStatus(std::move(status));

    return S_OK;
}

size_t ModerationState::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogModerationState const*> ModerationState::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ModerationState>(&this->Model());
}

size_t ModerationState::RequiredBufferSize(const PFCatalogModerationState& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.lastModifiedDate)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.reason)
    {
        requiredSize += (std::strlen(model.reason) + 1);
    }
    if (model.status)
    {
        requiredSize += (alignof(PFCatalogModerationStatus) + sizeof(PFCatalogModerationStatus));
    }
    return requiredSize;
}

HRESULT ModerationState::Copy(const PFCatalogModerationState& input, PFCatalogModerationState& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.lastModifiedDate);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lastModifiedDate = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.reason);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.reason = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.status);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.status = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue Rating::ToJson() const
{
    return Rating::ToJson(this->Model());
}

JsonValue Rating::ToJson(const PFCatalogRating& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Average", input.average);
    JsonUtils::ObjectAddMember(output, "Count1Star", input.count1Star);
    JsonUtils::ObjectAddMember(output, "Count2Star", input.count2Star);
    JsonUtils::ObjectAddMember(output, "Count3Star", input.count3Star);
    JsonUtils::ObjectAddMember(output, "Count4Star", input.count4Star);
    JsonUtils::ObjectAddMember(output, "Count5Star", input.count5Star);
    JsonUtils::ObjectAddMember(output, "TotalCount", input.totalCount);
    return output;
}

HRESULT Rating::FromJson(const JsonValue& input)
{
    std::optional<float> average{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Average", average));
    this->SetAverage(std::move(average));

    std::optional<int32_t> count1Star{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Count1Star", count1Star));
    this->SetCount1Star(std::move(count1Star));

    std::optional<int32_t> count2Star{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Count2Star", count2Star));
    this->SetCount2Star(std::move(count2Star));

    std::optional<int32_t> count3Star{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Count3Star", count3Star));
    this->SetCount3Star(std::move(count3Star));

    std::optional<int32_t> count4Star{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Count4Star", count4Star));
    this->SetCount4Star(std::move(count4Star));

    std::optional<int32_t> count5Star{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Count5Star", count5Star));
    this->SetCount5Star(std::move(count5Star));

    std::optional<int32_t> totalCount{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TotalCount", totalCount));
    this->SetTotalCount(std::move(totalCount));

    return S_OK;
}

size_t Rating::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogRating const*> Rating::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Rating>(&this->Model());
}

size_t Rating::RequiredBufferSize(const PFCatalogRating& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.average)
    {
        requiredSize += (alignof(float) + sizeof(float));
    }
    if (model.count1Star)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.count2Star)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.count3Star)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.count4Star)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.count5Star)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.totalCount)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    return requiredSize;
}

HRESULT Rating::Copy(const PFCatalogRating& input, PFCatalogRating& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.average);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.average = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.count1Star);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.count1Star = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.count2Star);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.count2Star = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.count3Star);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.count3Star = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.count4Star);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.count4Star = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.count5Star);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.count5Star = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.totalCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.totalCount = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue FilterOptions::ToJson() const
{
    return FilterOptions::ToJson(this->Model());
}

JsonValue FilterOptions::ToJson(const PFCatalogFilterOptions& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Filter", input.filter);
    JsonUtils::ObjectAddMember(output, "IncludeAllItems", input.includeAllItems);
    return output;
}

HRESULT FilterOptions::FromJson(const JsonValue& input)
{
    String filter{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Filter", filter));
    this->SetFilter(std::move(filter));

    std::optional<bool> includeAllItems{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IncludeAllItems", includeAllItems));
    this->SetIncludeAllItems(std::move(includeAllItems));

    return S_OK;
}

size_t FilterOptions::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogFilterOptions const*> FilterOptions::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<FilterOptions>(&this->Model());
}

size_t FilterOptions::RequiredBufferSize(const PFCatalogFilterOptions& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.filter)
    {
        requiredSize += (std::strlen(model.filter) + 1);
    }
    if (model.includeAllItems)
    {
        requiredSize += (alignof(bool) + sizeof(bool));
    }
    return requiredSize;
}

HRESULT FilterOptions::Copy(const PFCatalogFilterOptions& input, PFCatalogFilterOptions& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.filter);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.filter = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.includeAllItems);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.includeAllItems = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue Permissions::ToJson() const
{
    return Permissions::ToJson(this->Model());
}

JsonValue Permissions::ToJson(const PFCatalogPermissions& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray(output, "SegmentIds", input.segmentIds, input.segmentIdsCount);
    return output;
}

HRESULT Permissions::FromJson(const JsonValue& input)
{
    CStringVector segmentIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SegmentIds", segmentIds));
    this->SetSegmentIds(std::move(segmentIds));

    return S_OK;
}

size_t Permissions::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogPermissions const*> Permissions::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Permissions>(&this->Model());
}

size_t Permissions::RequiredBufferSize(const PFCatalogPermissions& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.segmentIdsCount);
    for (size_t i = 0; i < model.segmentIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.segmentIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT Permissions::Copy(const PFCatalogPermissions& input, PFCatalogPermissions& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.segmentIds, input.segmentIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.segmentIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CatalogPriceAmountOverride::ToJson() const
{
    return CatalogPriceAmountOverride::ToJson(this->Model());
}

JsonValue CatalogPriceAmountOverride::ToJson(const PFCatalogCatalogPriceAmountOverride& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "FixedValue", input.fixedValue);
    JsonUtils::ObjectAddMember(output, "ItemId", input.itemId);
    JsonUtils::ObjectAddMember(output, "Multiplier", input.multiplier);
    return output;
}

HRESULT CatalogPriceAmountOverride::FromJson(const JsonValue& input)
{
    std::optional<int32_t> fixedValue{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FixedValue", fixedValue));
    this->SetFixedValue(std::move(fixedValue));

    String itemId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemId", itemId));
    this->SetItemId(std::move(itemId));

    std::optional<double> multiplier{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Multiplier", multiplier));
    this->SetMultiplier(std::move(multiplier));

    return S_OK;
}

size_t CatalogPriceAmountOverride::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogPriceAmountOverride const*> CatalogPriceAmountOverride::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogPriceAmountOverride>(&this->Model());
}

size_t CatalogPriceAmountOverride::RequiredBufferSize(const PFCatalogCatalogPriceAmountOverride& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.fixedValue)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.itemId)
    {
        requiredSize += (std::strlen(model.itemId) + 1);
    }
    if (model.multiplier)
    {
        requiredSize += (alignof(double) + sizeof(double));
    }
    return requiredSize;
}

HRESULT CatalogPriceAmountOverride::Copy(const PFCatalogCatalogPriceAmountOverride& input, PFCatalogCatalogPriceAmountOverride& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.fixedValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.fixedValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.itemId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.multiplier);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.multiplier = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CatalogPriceOverride::ToJson() const
{
    return CatalogPriceOverride::ToJson(this->Model());
}

JsonValue CatalogPriceOverride::ToJson(const PFCatalogCatalogPriceOverride& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<CatalogPriceAmountOverride>(output, "Amounts", input.amounts, input.amountsCount);
    return output;
}

HRESULT CatalogPriceOverride::FromJson(const JsonValue& input)
{
    ModelVector<CatalogPriceAmountOverride> amounts{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogPriceAmountOverride>(input, "Amounts", amounts));
    this->SetAmounts(std::move(amounts));

    return S_OK;
}

size_t CatalogPriceOverride::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogPriceOverride const*> CatalogPriceOverride::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogPriceOverride>(&this->Model());
}

size_t CatalogPriceOverride::RequiredBufferSize(const PFCatalogCatalogPriceOverride& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCatalogCatalogPriceAmountOverride*) + sizeof(PFCatalogCatalogPriceAmountOverride*) * model.amountsCount);
    for (size_t i = 0; i < model.amountsCount; ++i)
    {
        requiredSize += CatalogPriceAmountOverride::RequiredBufferSize(*model.amounts[i]);
    }
    return requiredSize;
}

HRESULT CatalogPriceOverride::Copy(const PFCatalogCatalogPriceOverride& input, PFCatalogCatalogPriceOverride& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CatalogPriceAmountOverride>(input.amounts, input.amountsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.amounts = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CatalogPriceOptionsOverride::ToJson() const
{
    return CatalogPriceOptionsOverride::ToJson(this->Model());
}

JsonValue CatalogPriceOptionsOverride::ToJson(const PFCatalogCatalogPriceOptionsOverride& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<CatalogPriceOverride>(output, "Prices", input.prices, input.pricesCount);
    return output;
}

HRESULT CatalogPriceOptionsOverride::FromJson(const JsonValue& input)
{
    ModelVector<CatalogPriceOverride> prices{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogPriceOverride>(input, "Prices", prices));
    this->SetPrices(std::move(prices));

    return S_OK;
}

size_t CatalogPriceOptionsOverride::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogPriceOptionsOverride const*> CatalogPriceOptionsOverride::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogPriceOptionsOverride>(&this->Model());
}

size_t CatalogPriceOptionsOverride::RequiredBufferSize(const PFCatalogCatalogPriceOptionsOverride& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCatalogCatalogPriceOverride*) + sizeof(PFCatalogCatalogPriceOverride*) * model.pricesCount);
    for (size_t i = 0; i < model.pricesCount; ++i)
    {
        requiredSize += CatalogPriceOverride::RequiredBufferSize(*model.prices[i]);
    }
    return requiredSize;
}

HRESULT CatalogPriceOptionsOverride::Copy(const PFCatalogCatalogPriceOptionsOverride& input, PFCatalogCatalogPriceOptionsOverride& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CatalogPriceOverride>(input.prices, input.pricesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.prices = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue StoreDetails::ToJson() const
{
    return StoreDetails::ToJson(this->Model());
}

JsonValue StoreDetails::ToJson(const PFCatalogStoreDetails& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<FilterOptions>(output, "FilterOptions", input.filterOptions);
    JsonUtils::ObjectAddMember<Permissions>(output, "Permissions", input.permissions);
    JsonUtils::ObjectAddMember<CatalogPriceOptionsOverride>(output, "PriceOptionsOverride", input.priceOptionsOverride);
    return output;
}

HRESULT StoreDetails::FromJson(const JsonValue& input)
{
    std::optional<FilterOptions> filterOptions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FilterOptions", filterOptions));
    if (filterOptions)
    {
        this->SetFilterOptions(std::move(*filterOptions));
    }

    std::optional<Permissions> permissions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Permissions", permissions));
    if (permissions)
    {
        this->SetPermissions(std::move(*permissions));
    }

    std::optional<CatalogPriceOptionsOverride> priceOptionsOverride{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PriceOptionsOverride", priceOptionsOverride));
    if (priceOptionsOverride)
    {
        this->SetPriceOptionsOverride(std::move(*priceOptionsOverride));
    }

    return S_OK;
}

size_t StoreDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogStoreDetails const*> StoreDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<StoreDetails>(&this->Model());
}

size_t StoreDetails::RequiredBufferSize(const PFCatalogStoreDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.filterOptions)
    {
        requiredSize += FilterOptions::RequiredBufferSize(*model.filterOptions);
    }
    if (model.permissions)
    {
        requiredSize += Permissions::RequiredBufferSize(*model.permissions);
    }
    if (model.priceOptionsOverride)
    {
        requiredSize += CatalogPriceOptionsOverride::RequiredBufferSize(*model.priceOptionsOverride);
    }
    return requiredSize;
}

HRESULT StoreDetails::Copy(const PFCatalogStoreDetails& input, PFCatalogStoreDetails& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<FilterOptions>(input.filterOptions);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.filterOptions = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<Permissions>(input.permissions);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.permissions = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<CatalogPriceOptionsOverride>(input.priceOptionsOverride);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.priceOptionsOverride = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CatalogItem::ToJson() const
{
    return CatalogItem::ToJson(this->Model());
}

JsonValue CatalogItem::ToJson(const PFCatalogCatalogItem& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<CatalogAlternateId>(output, "AlternateIds", input.alternateIds, input.alternateIdsCount);
    JsonUtils::ObjectAddMemberArray<Content>(output, "Contents", input.contents, input.contentsCount);
    JsonUtils::ObjectAddMember(output, "ContentType", input.contentType);
    JsonUtils::ObjectAddMemberTime(output, "CreationDate", input.creationDate);
    JsonUtils::ObjectAddMember<EntityKey>(output, "CreatorEntity", input.creatorEntity);
    JsonUtils::ObjectAddMemberArray<DeepLink>(output, "DeepLinks", input.deepLinks, input.deepLinksCount);
    JsonUtils::ObjectAddMember(output, "DefaultStackId", input.defaultStackId);
    JsonUtils::ObjectAddMemberDictionary(output, "Description", input.description, input.descriptionCount);
    JsonUtils::ObjectAddMember(output, "DisplayProperties", input.displayProperties);
    JsonUtils::ObjectAddMember(output, "DisplayVersion", input.displayVersion);
    JsonUtils::ObjectAddMemberTime(output, "EndDate", input.endDate);
    JsonUtils::ObjectAddMember(output, "ETag", input.eTag);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMemberArray<Image>(output, "Images", input.images, input.imagesCount);
    JsonUtils::ObjectAddMember(output, "IsHidden", input.isHidden);
    JsonUtils::ObjectAddMemberArray<CatalogItemReference>(output, "ItemReferences", input.itemReferences, input.itemReferencesCount);
    JsonUtils::ObjectAddMemberDictionary<KeywordSet>(output, "Keywords", input.keywords, input.keywordsCount);
    JsonUtils::ObjectAddMemberTime(output, "LastModifiedDate", input.lastModifiedDate);
    JsonUtils::ObjectAddMember<ModerationState>(output, "Moderation", input.moderation);
    JsonUtils::ObjectAddMemberArray(output, "Platforms", input.platforms, input.platformsCount);
    JsonUtils::ObjectAddMember<CatalogPriceOptions>(output, "PriceOptions", input.priceOptions);
    JsonUtils::ObjectAddMember<Rating>(output, "Rating", input.rating);
    JsonUtils::ObjectAddMemberTime(output, "StartDate", input.startDate);
    JsonUtils::ObjectAddMember<StoreDetails>(output, "StoreDetails", input.storeDetails);
    JsonUtils::ObjectAddMemberArray(output, "Tags", input.tags, input.tagsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "Title", input.title, input.titleCount);
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    return output;
}

HRESULT CatalogItem::FromJson(const JsonValue& input)
{
    ModelVector<CatalogAlternateId> alternateIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogAlternateId>(input, "AlternateIds", alternateIds));
    this->SetAlternateIds(std::move(alternateIds));

    ModelVector<Content> contents{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<Content>(input, "Contents", contents));
    this->SetContents(std::move(contents));

    String contentType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContentType", contentType));
    this->SetContentType(std::move(contentType));

    std::optional<time_t> creationDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "CreationDate", creationDate));
    this->SetCreationDate(std::move(creationDate));

    std::optional<EntityKey> creatorEntity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CreatorEntity", creatorEntity));
    if (creatorEntity)
    {
        this->SetCreatorEntity(std::move(*creatorEntity));
    }

    ModelVector<DeepLink> deepLinks{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<DeepLink>(input, "DeepLinks", deepLinks));
    this->SetDeepLinks(std::move(deepLinks));

    String defaultStackId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DefaultStackId", defaultStackId));
    this->SetDefaultStackId(std::move(defaultStackId));

    StringDictionaryEntryVector description{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Description", description));
    this->SetDescription(std::move(description));

    JsonObject displayProperties{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisplayProperties", displayProperties));
    this->SetDisplayProperties(std::move(displayProperties));

    String displayVersion{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisplayVersion", displayVersion));
    this->SetDisplayVersion(std::move(displayVersion));

    std::optional<time_t> endDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "EndDate", endDate));
    this->SetEndDate(std::move(endDate));

    String eTag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ETag", eTag));
    this->SetETag(std::move(eTag));

    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    ModelVector<Image> images{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<Image>(input, "Images", images));
    this->SetImages(std::move(images));

    std::optional<bool> isHidden{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsHidden", isHidden));
    this->SetIsHidden(std::move(isHidden));

    ModelVector<CatalogItemReference> itemReferences{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogItemReference>(input, "ItemReferences", itemReferences));
    this->SetItemReferences(std::move(itemReferences));

    ModelDictionaryEntryVector<KeywordSet> keywords{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<KeywordSet>(input, "Keywords", keywords));
    this->SetKeywords(std::move(keywords));

    std::optional<time_t> lastModifiedDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastModifiedDate", lastModifiedDate));
    this->SetLastModifiedDate(std::move(lastModifiedDate));

    std::optional<ModerationState> moderation{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Moderation", moderation));
    if (moderation)
    {
        this->SetModeration(std::move(*moderation));
    }

    CStringVector platforms{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Platforms", platforms));
    this->SetPlatforms(std::move(platforms));

    std::optional<CatalogPriceOptions> priceOptions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PriceOptions", priceOptions));
    if (priceOptions)
    {
        this->SetPriceOptions(std::move(*priceOptions));
    }

    std::optional<Rating> rating{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Rating", rating));
    if (rating)
    {
        this->SetRating(std::move(*rating));
    }

    std::optional<time_t> startDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "StartDate", startDate));
    this->SetStartDate(std::move(startDate));

    std::optional<StoreDetails> storeDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StoreDetails", storeDetails));
    if (storeDetails)
    {
        this->SetStoreDetails(std::move(*storeDetails));
    }

    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    StringDictionaryEntryVector title{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Title", title));
    this->SetTitle(std::move(title));

    String type{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", type));
    this->SetType(std::move(type));

    return S_OK;
}

size_t CatalogItem::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogItem const*> CatalogItem::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogItem>(&this->Model());
}

size_t CatalogItem::RequiredBufferSize(const PFCatalogCatalogItem& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCatalogCatalogAlternateId*) + sizeof(PFCatalogCatalogAlternateId*) * model.alternateIdsCount);
    for (size_t i = 0; i < model.alternateIdsCount; ++i)
    {
        requiredSize += CatalogAlternateId::RequiredBufferSize(*model.alternateIds[i]);
    }
    requiredSize += (alignof(PFCatalogContent*) + sizeof(PFCatalogContent*) * model.contentsCount);
    for (size_t i = 0; i < model.contentsCount; ++i)
    {
        requiredSize += Content::RequiredBufferSize(*model.contents[i]);
    }
    if (model.contentType)
    {
        requiredSize += (std::strlen(model.contentType) + 1);
    }
    if (model.creationDate)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.creatorEntity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.creatorEntity);
    }
    requiredSize += (alignof(PFCatalogDeepLink*) + sizeof(PFCatalogDeepLink*) * model.deepLinksCount);
    for (size_t i = 0; i < model.deepLinksCount; ++i)
    {
        requiredSize += DeepLink::RequiredBufferSize(*model.deepLinks[i]);
    }
    if (model.defaultStackId)
    {
        requiredSize += (std::strlen(model.defaultStackId) + 1);
    }
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.descriptionCount);
    for (size_t i = 0; i < model.descriptionCount; ++i)
    {
        requiredSize += (std::strlen(model.description[i].key) + 1);
        requiredSize += (std::strlen(model.description[i].value) + 1);
    }
    if (model.displayProperties.stringValue)
    {
    requiredSize += (std::strlen(model.displayProperties.stringValue) + 1);
    }
    if (model.displayVersion)
    {
        requiredSize += (std::strlen(model.displayVersion) + 1);
    }
    if (model.endDate)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.eTag)
    {
        requiredSize += (std::strlen(model.eTag) + 1);
    }
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    requiredSize += (alignof(PFCatalogImage*) + sizeof(PFCatalogImage*) * model.imagesCount);
    for (size_t i = 0; i < model.imagesCount; ++i)
    {
        requiredSize += Image::RequiredBufferSize(*model.images[i]);
    }
    if (model.isHidden)
    {
        requiredSize += (alignof(bool) + sizeof(bool));
    }
    requiredSize += (alignof(PFCatalogCatalogItemReference*) + sizeof(PFCatalogCatalogItemReference*) * model.itemReferencesCount);
    for (size_t i = 0; i < model.itemReferencesCount; ++i)
    {
        requiredSize += CatalogItemReference::RequiredBufferSize(*model.itemReferences[i]);
    }
    requiredSize += (alignof(PFCatalogKeywordSetDictionaryEntry) + sizeof(PFCatalogKeywordSetDictionaryEntry) * model.keywordsCount);
    for (size_t i = 0; i < model.keywordsCount; ++i)
    {
        requiredSize += (std::strlen(model.keywords[i].key) + 1);
        requiredSize += KeywordSet::RequiredBufferSize(*model.keywords[i].value);
    }
    if (model.lastModifiedDate)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.moderation)
    {
        requiredSize += ModerationState::RequiredBufferSize(*model.moderation);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.platformsCount);
    for (size_t i = 0; i < model.platformsCount; ++i)
    {
        requiredSize += (std::strlen(model.platforms[i]) + 1);
    }
    if (model.priceOptions)
    {
        requiredSize += CatalogPriceOptions::RequiredBufferSize(*model.priceOptions);
    }
    if (model.rating)
    {
        requiredSize += Rating::RequiredBufferSize(*model.rating);
    }
    if (model.startDate)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.storeDetails)
    {
        requiredSize += StoreDetails::RequiredBufferSize(*model.storeDetails);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.titleCount);
    for (size_t i = 0; i < model.titleCount; ++i)
    {
        requiredSize += (std::strlen(model.title[i].key) + 1);
        requiredSize += (std::strlen(model.title[i].value) + 1);
    }
    if (model.type)
    {
        requiredSize += (std::strlen(model.type) + 1);
    }
    return requiredSize;
}

HRESULT CatalogItem::Copy(const PFCatalogCatalogItem& input, PFCatalogCatalogItem& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CatalogAlternateId>(input.alternateIds, input.alternateIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.alternateIds = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<Content>(input.contents, input.contentsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.contents = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.contentType);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.contentType = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.creationDate);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.creationDate = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.creatorEntity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.creatorEntity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<DeepLink>(input.deepLinks, input.deepLinksCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.deepLinks = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.defaultStackId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.defaultStackId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.description, input.descriptionCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.description = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.displayProperties.stringValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.displayProperties.stringValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.displayVersion);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.displayVersion = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.endDate);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.endDate = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.eTag);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.eTag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.id);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<Image>(input.images, input.imagesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.images = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.isHidden);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.isHidden = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<CatalogItemReference>(input.itemReferences, input.itemReferencesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemReferences = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<KeywordSet>(input.keywords, input.keywordsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.keywords = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.lastModifiedDate);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lastModifiedDate = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<ModerationState>(input.moderation);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.moderation = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.platforms, input.platformsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.platforms = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<CatalogPriceOptions>(input.priceOptions);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.priceOptions = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<Rating>(input.rating);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.rating = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.startDate);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.startDate = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<StoreDetails>(input.storeDetails);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.storeDetails = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.title, input.titleCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.title = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.type);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.type = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CreateDraftItemRequest::ToJson() const
{
    return CreateDraftItemRequest::ToJson(this->Model());
}

JsonValue CreateDraftItemRequest::ToJson(const PFCatalogCreateDraftItemRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<CatalogItem>(output, "Item", input.item);
    JsonUtils::ObjectAddMember(output, "Publish", input.publish);
    return output;
}

HRESULT CreateDraftItemResponse::FromJson(const JsonValue& input)
{
    std::optional<CatalogItem> item{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Item", item));
    if (item)
    {
        this->SetItem(std::move(*item));
    }

    return S_OK;
}

size_t CreateDraftItemResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCreateDraftItemResponse const*> CreateDraftItemResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CreateDraftItemResponse>(&this->Model());
}

size_t CreateDraftItemResponse::RequiredBufferSize(const PFCatalogCreateDraftItemResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.item)
    {
        requiredSize += CatalogItem::RequiredBufferSize(*model.item);
    }
    return requiredSize;
}

HRESULT CreateDraftItemResponse::Copy(const PFCatalogCreateDraftItemResponse& input, PFCatalogCreateDraftItemResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<CatalogItem>(input.item);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.item = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UploadInfo::ToJson() const
{
    return UploadInfo::ToJson(this->Model());
}

JsonValue UploadInfo::ToJson(const PFCatalogUploadInfo& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "FileName", input.fileName);
    return output;
}

JsonValue CreateUploadUrlsRequest::ToJson() const
{
    return CreateUploadUrlsRequest::ToJson(this->Model());
}

JsonValue CreateUploadUrlsRequest::ToJson(const PFCatalogCreateUploadUrlsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray<UploadInfo>(output, "Files", input.files, input.filesCount);
    return output;
}

HRESULT UploadUrlMetadata::FromJson(const JsonValue& input)
{
    String fileName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FileName", fileName));
    this->SetFileName(std::move(fileName));

    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    String url{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Url", url));
    this->SetUrl(std::move(url));

    return S_OK;
}

size_t UploadUrlMetadata::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogUploadUrlMetadata const*> UploadUrlMetadata::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UploadUrlMetadata>(&this->Model());
}

size_t UploadUrlMetadata::RequiredBufferSize(const PFCatalogUploadUrlMetadata& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.fileName)
    {
        requiredSize += (std::strlen(model.fileName) + 1);
    }
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.url)
    {
        requiredSize += (std::strlen(model.url) + 1);
    }
    return requiredSize;
}

HRESULT UploadUrlMetadata::Copy(const PFCatalogUploadUrlMetadata& input, PFCatalogUploadUrlMetadata& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.fileName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.fileName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.id);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.url);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.url = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT CreateUploadUrlsResponse::FromJson(const JsonValue& input)
{
    ModelVector<UploadUrlMetadata> uploadUrls{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<UploadUrlMetadata>(input, "UploadUrls", uploadUrls));
    this->SetUploadUrls(std::move(uploadUrls));

    return S_OK;
}

size_t CreateUploadUrlsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCreateUploadUrlsResponse const*> CreateUploadUrlsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CreateUploadUrlsResponse>(&this->Model());
}

size_t CreateUploadUrlsResponse::RequiredBufferSize(const PFCatalogCreateUploadUrlsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCatalogUploadUrlMetadata*) + sizeof(PFCatalogUploadUrlMetadata*) * model.uploadUrlsCount);
    for (size_t i = 0; i < model.uploadUrlsCount; ++i)
    {
        requiredSize += UploadUrlMetadata::RequiredBufferSize(*model.uploadUrls[i]);
    }
    return requiredSize;
}

HRESULT CreateUploadUrlsResponse::Copy(const PFCatalogCreateUploadUrlsResponse& input, PFCatalogCreateUploadUrlsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<UploadUrlMetadata>(input.uploadUrls, input.uploadUrlsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.uploadUrls = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DeleteEntityItemReviewsRequest::ToJson() const
{
    return DeleteEntityItemReviewsRequest::ToJson(this->Model());
}

JsonValue DeleteEntityItemReviewsRequest::ToJson(const PFCatalogDeleteEntityItemReviewsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

JsonValue DeleteItemRequest::ToJson() const
{
    return DeleteItemRequest::ToJson(this->Model());
}

JsonValue DeleteItemRequest::ToJson(const PFCatalogDeleteItemRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

JsonValue GetCatalogConfigRequest::ToJson() const
{
    return GetCatalogConfigRequest::ToJson(this->Model());
}

JsonValue GetCatalogConfigRequest::ToJson(const PFCatalogGetCatalogConfigRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    return output;
}

JsonValue CatalogSpecificConfig::ToJson() const
{
    return CatalogSpecificConfig::ToJson(this->Model());
}

JsonValue CatalogSpecificConfig::ToJson(const PFCatalogCatalogSpecificConfig& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray(output, "ContentTypes", input.contentTypes, input.contentTypesCount);
    JsonUtils::ObjectAddMemberArray(output, "Tags", input.tags, input.tagsCount);
    return output;
}

HRESULT CatalogSpecificConfig::FromJson(const JsonValue& input)
{
    CStringVector contentTypes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContentTypes", contentTypes));
    this->SetContentTypes(std::move(contentTypes));

    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    return S_OK;
}

size_t CatalogSpecificConfig::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogSpecificConfig const*> CatalogSpecificConfig::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogSpecificConfig>(&this->Model());
}

size_t CatalogSpecificConfig::RequiredBufferSize(const PFCatalogCatalogSpecificConfig& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.contentTypesCount);
    for (size_t i = 0; i < model.contentTypesCount; ++i)
    {
        requiredSize += (std::strlen(model.contentTypes[i]) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    return requiredSize;
}

HRESULT CatalogSpecificConfig::Copy(const PFCatalogCatalogSpecificConfig& input, PFCatalogCatalogSpecificConfig& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.contentTypes, input.contentTypesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.contentTypes = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DeepLinkFormat::ToJson() const
{
    return DeepLinkFormat::ToJson(this->Model());
}

JsonValue DeepLinkFormat::ToJson(const PFCatalogDeepLinkFormat& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Format", input.format);
    JsonUtils::ObjectAddMember(output, "Platform", input.platform);
    return output;
}

HRESULT DeepLinkFormat::FromJson(const JsonValue& input)
{
    String format{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Format", format));
    this->SetFormat(std::move(format));

    String platform{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Platform", platform));
    this->SetPlatform(std::move(platform));

    return S_OK;
}

size_t DeepLinkFormat::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogDeepLinkFormat const*> DeepLinkFormat::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DeepLinkFormat>(&this->Model());
}

size_t DeepLinkFormat::RequiredBufferSize(const PFCatalogDeepLinkFormat& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.format)
    {
        requiredSize += (std::strlen(model.format) + 1);
    }
    if (model.platform)
    {
        requiredSize += (std::strlen(model.platform) + 1);
    }
    return requiredSize;
}

HRESULT DeepLinkFormat::Copy(const PFCatalogDeepLinkFormat& input, PFCatalogDeepLinkFormat& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.format);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.format = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.platform);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.platform = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DisplayPropertyIndexInfo::ToJson() const
{
    return DisplayPropertyIndexInfo::ToJson(this->Model());
}

JsonValue DisplayPropertyIndexInfo::ToJson(const PFCatalogDisplayPropertyIndexInfo& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    return output;
}

HRESULT DisplayPropertyIndexInfo::FromJson(const JsonValue& input)
{
    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    std::optional<PFCatalogDisplayPropertyType> type{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", type));
    this->SetType(std::move(type));

    return S_OK;
}

size_t DisplayPropertyIndexInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogDisplayPropertyIndexInfo const*> DisplayPropertyIndexInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DisplayPropertyIndexInfo>(&this->Model());
}

size_t DisplayPropertyIndexInfo::RequiredBufferSize(const PFCatalogDisplayPropertyIndexInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    if (model.type)
    {
        requiredSize += (alignof(PFCatalogDisplayPropertyType) + sizeof(PFCatalogDisplayPropertyType));
    }
    return requiredSize;
}

HRESULT DisplayPropertyIndexInfo::Copy(const PFCatalogDisplayPropertyIndexInfo& input, PFCatalogDisplayPropertyIndexInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.name);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.type);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.type = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue FileConfig::ToJson() const
{
    return FileConfig::ToJson(this->Model());
}

JsonValue FileConfig::ToJson(const PFCatalogFileConfig& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray(output, "ContentTypes", input.contentTypes, input.contentTypesCount);
    JsonUtils::ObjectAddMemberArray(output, "Tags", input.tags, input.tagsCount);
    return output;
}

HRESULT FileConfig::FromJson(const JsonValue& input)
{
    CStringVector contentTypes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContentTypes", contentTypes));
    this->SetContentTypes(std::move(contentTypes));

    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    return S_OK;
}

size_t FileConfig::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogFileConfig const*> FileConfig::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<FileConfig>(&this->Model());
}

size_t FileConfig::RequiredBufferSize(const PFCatalogFileConfig& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.contentTypesCount);
    for (size_t i = 0; i < model.contentTypesCount; ++i)
    {
        requiredSize += (std::strlen(model.contentTypes[i]) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    return requiredSize;
}

HRESULT FileConfig::Copy(const PFCatalogFileConfig& input, PFCatalogFileConfig& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.contentTypes, input.contentTypesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.contentTypes = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ImageConfig::ToJson() const
{
    return ImageConfig::ToJson(this->Model());
}

JsonValue ImageConfig::ToJson(const PFCatalogImageConfig& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray(output, "Tags", input.tags, input.tagsCount);
    return output;
}

HRESULT ImageConfig::FromJson(const JsonValue& input)
{
    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    return S_OK;
}

size_t ImageConfig::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogImageConfig const*> ImageConfig::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ImageConfig>(&this->Model());
}

size_t ImageConfig::RequiredBufferSize(const PFCatalogImageConfig& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    return requiredSize;
}

HRESULT ImageConfig::Copy(const PFCatalogImageConfig& input, PFCatalogImageConfig& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CategoryRatingConfig::ToJson() const
{
    return CategoryRatingConfig::ToJson(this->Model());
}

JsonValue CategoryRatingConfig::ToJson(const PFCatalogCategoryRatingConfig& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

HRESULT CategoryRatingConfig::FromJson(const JsonValue& input)
{
    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    return S_OK;
}

size_t CategoryRatingConfig::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCategoryRatingConfig const*> CategoryRatingConfig::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CategoryRatingConfig>(&this->Model());
}

size_t CategoryRatingConfig::RequiredBufferSize(const PFCatalogCategoryRatingConfig& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT CategoryRatingConfig::Copy(const PFCatalogCategoryRatingConfig& input, PFCatalogCategoryRatingConfig& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.name);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ReviewConfig::ToJson() const
{
    return ReviewConfig::ToJson(this->Model());
}

JsonValue ReviewConfig::ToJson(const PFCatalogReviewConfig& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<CategoryRatingConfig>(output, "CategoryRatings", input.categoryRatings, input.categoryRatingsCount);
    return output;
}

HRESULT ReviewConfig::FromJson(const JsonValue& input)
{
    ModelVector<CategoryRatingConfig> categoryRatings{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CategoryRatingConfig>(input, "CategoryRatings", categoryRatings));
    this->SetCategoryRatings(std::move(categoryRatings));

    return S_OK;
}

size_t ReviewConfig::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogReviewConfig const*> ReviewConfig::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ReviewConfig>(&this->Model());
}

size_t ReviewConfig::RequiredBufferSize(const PFCatalogReviewConfig& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCatalogCategoryRatingConfig*) + sizeof(PFCatalogCategoryRatingConfig*) * model.categoryRatingsCount);
    for (size_t i = 0; i < model.categoryRatingsCount; ++i)
    {
        requiredSize += CategoryRatingConfig::RequiredBufferSize(*model.categoryRatings[i]);
    }
    return requiredSize;
}

HRESULT ReviewConfig::Copy(const PFCatalogReviewConfig& input, PFCatalogReviewConfig& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CategoryRatingConfig>(input.categoryRatings, input.categoryRatingsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.categoryRatings = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UserGeneratedContentSpecificConfig::ToJson() const
{
    return UserGeneratedContentSpecificConfig::ToJson(this->Model());
}

JsonValue UserGeneratedContentSpecificConfig::ToJson(const PFCatalogUserGeneratedContentSpecificConfig& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray(output, "ContentTypes", input.contentTypes, input.contentTypesCount);
    JsonUtils::ObjectAddMemberArray(output, "Tags", input.tags, input.tagsCount);
    return output;
}

HRESULT UserGeneratedContentSpecificConfig::FromJson(const JsonValue& input)
{
    CStringVector contentTypes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContentTypes", contentTypes));
    this->SetContentTypes(std::move(contentTypes));

    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    return S_OK;
}

size_t UserGeneratedContentSpecificConfig::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogUserGeneratedContentSpecificConfig const*> UserGeneratedContentSpecificConfig::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UserGeneratedContentSpecificConfig>(&this->Model());
}

size_t UserGeneratedContentSpecificConfig::RequiredBufferSize(const PFCatalogUserGeneratedContentSpecificConfig& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.contentTypesCount);
    for (size_t i = 0; i < model.contentTypesCount; ++i)
    {
        requiredSize += (std::strlen(model.contentTypes[i]) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    return requiredSize;
}

HRESULT UserGeneratedContentSpecificConfig::Copy(const PFCatalogUserGeneratedContentSpecificConfig& input, PFCatalogUserGeneratedContentSpecificConfig& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.contentTypes, input.contentTypesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.contentTypes = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CatalogConfig::ToJson() const
{
    return CatalogConfig::ToJson(this->Model());
}

JsonValue CatalogConfig::ToJson(const PFCatalogCatalogConfig& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<EntityKey>(output, "AdminEntities", input.adminEntities, input.adminEntitiesCount);
    JsonUtils::ObjectAddMember<CatalogSpecificConfig>(output, "Catalog", input.catalog);
    JsonUtils::ObjectAddMemberArray<DeepLinkFormat>(output, "DeepLinkFormats", input.deepLinkFormats, input.deepLinkFormatsCount);
    JsonUtils::ObjectAddMemberArray<DisplayPropertyIndexInfo>(output, "DisplayPropertyIndexInfos", input.displayPropertyIndexInfos, input.displayPropertyIndexInfosCount);
    JsonUtils::ObjectAddMember<FileConfig>(output, "File", input.file);
    JsonUtils::ObjectAddMember<ImageConfig>(output, "Image", input.image);
    JsonUtils::ObjectAddMember(output, "IsCatalogEnabled", input.isCatalogEnabled);
    JsonUtils::ObjectAddMemberArray(output, "Platforms", input.platforms, input.platformsCount);
    JsonUtils::ObjectAddMember<ReviewConfig>(output, "Review", input.review);
    JsonUtils::ObjectAddMemberArray<EntityKey>(output, "ReviewerEntities", input.reviewerEntities, input.reviewerEntitiesCount);
    JsonUtils::ObjectAddMember<UserGeneratedContentSpecificConfig>(output, "UserGeneratedContent", input.userGeneratedContent);
    return output;
}

HRESULT CatalogConfig::FromJson(const JsonValue& input)
{
    ModelVector<EntityKey> adminEntities{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityKey>(input, "AdminEntities", adminEntities));
    this->SetAdminEntities(std::move(adminEntities));

    std::optional<CatalogSpecificConfig> catalog{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Catalog", catalog));
    if (catalog)
    {
        this->SetCatalog(std::move(*catalog));
    }

    ModelVector<DeepLinkFormat> deepLinkFormats{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<DeepLinkFormat>(input, "DeepLinkFormats", deepLinkFormats));
    this->SetDeepLinkFormats(std::move(deepLinkFormats));

    ModelVector<DisplayPropertyIndexInfo> displayPropertyIndexInfos{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<DisplayPropertyIndexInfo>(input, "DisplayPropertyIndexInfos", displayPropertyIndexInfos));
    this->SetDisplayPropertyIndexInfos(std::move(displayPropertyIndexInfos));

    std::optional<FileConfig> file{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "File", file));
    if (file)
    {
        this->SetFile(std::move(*file));
    }

    std::optional<ImageConfig> image{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Image", image));
    if (image)
    {
        this->SetImage(std::move(*image));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsCatalogEnabled", this->m_model.isCatalogEnabled));

    CStringVector platforms{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Platforms", platforms));
    this->SetPlatforms(std::move(platforms));

    std::optional<ReviewConfig> review{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Review", review));
    if (review)
    {
        this->SetReview(std::move(*review));
    }

    ModelVector<EntityKey> reviewerEntities{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityKey>(input, "ReviewerEntities", reviewerEntities));
    this->SetReviewerEntities(std::move(reviewerEntities));

    std::optional<UserGeneratedContentSpecificConfig> userGeneratedContent{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UserGeneratedContent", userGeneratedContent));
    if (userGeneratedContent)
    {
        this->SetUserGeneratedContent(std::move(*userGeneratedContent));
    }

    return S_OK;
}

size_t CatalogConfig::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogCatalogConfig const*> CatalogConfig::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CatalogConfig>(&this->Model());
}

size_t CatalogConfig::RequiredBufferSize(const PFCatalogCatalogConfig& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFEntityKey*) + sizeof(PFEntityKey*) * model.adminEntitiesCount);
    for (size_t i = 0; i < model.adminEntitiesCount; ++i)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.adminEntities[i]);
    }
    if (model.catalog)
    {
        requiredSize += CatalogSpecificConfig::RequiredBufferSize(*model.catalog);
    }
    requiredSize += (alignof(PFCatalogDeepLinkFormat*) + sizeof(PFCatalogDeepLinkFormat*) * model.deepLinkFormatsCount);
    for (size_t i = 0; i < model.deepLinkFormatsCount; ++i)
    {
        requiredSize += DeepLinkFormat::RequiredBufferSize(*model.deepLinkFormats[i]);
    }
    requiredSize += (alignof(PFCatalogDisplayPropertyIndexInfo*) + sizeof(PFCatalogDisplayPropertyIndexInfo*) * model.displayPropertyIndexInfosCount);
    for (size_t i = 0; i < model.displayPropertyIndexInfosCount; ++i)
    {
        requiredSize += DisplayPropertyIndexInfo::RequiredBufferSize(*model.displayPropertyIndexInfos[i]);
    }
    if (model.file)
    {
        requiredSize += FileConfig::RequiredBufferSize(*model.file);
    }
    if (model.image)
    {
        requiredSize += ImageConfig::RequiredBufferSize(*model.image);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.platformsCount);
    for (size_t i = 0; i < model.platformsCount; ++i)
    {
        requiredSize += (std::strlen(model.platforms[i]) + 1);
    }
    if (model.review)
    {
        requiredSize += ReviewConfig::RequiredBufferSize(*model.review);
    }
    requiredSize += (alignof(PFEntityKey*) + sizeof(PFEntityKey*) * model.reviewerEntitiesCount);
    for (size_t i = 0; i < model.reviewerEntitiesCount; ++i)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.reviewerEntities[i]);
    }
    if (model.userGeneratedContent)
    {
        requiredSize += UserGeneratedContentSpecificConfig::RequiredBufferSize(*model.userGeneratedContent);
    }
    return requiredSize;
}

HRESULT CatalogConfig::Copy(const PFCatalogCatalogConfig& input, PFCatalogCatalogConfig& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<EntityKey>(input.adminEntities, input.adminEntitiesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.adminEntities = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<CatalogSpecificConfig>(input.catalog);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.catalog = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<DeepLinkFormat>(input.deepLinkFormats, input.deepLinkFormatsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.deepLinkFormats = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<DisplayPropertyIndexInfo>(input.displayPropertyIndexInfos, input.displayPropertyIndexInfosCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.displayPropertyIndexInfos = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<FileConfig>(input.file);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.file = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<ImageConfig>(input.image);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.image = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.platforms, input.platformsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.platforms = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<ReviewConfig>(input.review);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.review = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<EntityKey>(input.reviewerEntities, input.reviewerEntitiesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.reviewerEntities = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserGeneratedContentSpecificConfig>(input.userGeneratedContent);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.userGeneratedContent = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetCatalogConfigResponse::FromJson(const JsonValue& input)
{
    std::optional<CatalogConfig> config{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Config", config));
    if (config)
    {
        this->SetConfig(std::move(*config));
    }

    return S_OK;
}

size_t GetCatalogConfigResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetCatalogConfigResponse const*> GetCatalogConfigResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetCatalogConfigResponse>(&this->Model());
}

size_t GetCatalogConfigResponse::RequiredBufferSize(const PFCatalogGetCatalogConfigResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.config)
    {
        requiredSize += CatalogConfig::RequiredBufferSize(*model.config);
    }
    return requiredSize;
}

HRESULT GetCatalogConfigResponse::Copy(const PFCatalogGetCatalogConfigResponse& input, PFCatalogGetCatalogConfigResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<CatalogConfig>(input.config);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.config = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetDraftItemRequest::ToJson() const
{
    return GetDraftItemRequest::ToJson(this->Model());
}

JsonValue GetDraftItemRequest::ToJson(const PFCatalogGetDraftItemRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

HRESULT GetDraftItemResponse::FromJson(const JsonValue& input)
{
    std::optional<CatalogItem> item{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Item", item));
    if (item)
    {
        this->SetItem(std::move(*item));
    }

    return S_OK;
}

size_t GetDraftItemResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetDraftItemResponse const*> GetDraftItemResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetDraftItemResponse>(&this->Model());
}

size_t GetDraftItemResponse::RequiredBufferSize(const PFCatalogGetDraftItemResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.item)
    {
        requiredSize += CatalogItem::RequiredBufferSize(*model.item);
    }
    return requiredSize;
}

HRESULT GetDraftItemResponse::Copy(const PFCatalogGetDraftItemResponse& input, PFCatalogGetDraftItemResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<CatalogItem>(input.item);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.item = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetDraftItemsRequest::ToJson() const
{
    return GetDraftItemsRequest::ToJson(this->Model());
}

JsonValue GetDraftItemsRequest::ToJson(const PFCatalogGetDraftItemsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<CatalogAlternateId>(output, "AlternateIds", input.alternateIds, input.alternateIdsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray(output, "Ids", input.ids, input.idsCount);
    return output;
}

HRESULT GetDraftItemsResponse::FromJson(const JsonValue& input)
{
    String continuationToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinuationToken", continuationToken));
    this->SetContinuationToken(std::move(continuationToken));

    ModelVector<CatalogItem> items{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogItem>(input, "Items", items));
    this->SetItems(std::move(items));

    return S_OK;
}

size_t GetDraftItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetDraftItemsResponse const*> GetDraftItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetDraftItemsResponse>(&this->Model());
}

size_t GetDraftItemsResponse::RequiredBufferSize(const PFCatalogGetDraftItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.continuationToken)
    {
        requiredSize += (std::strlen(model.continuationToken) + 1);
    }
    requiredSize += (alignof(PFCatalogCatalogItem*) + sizeof(PFCatalogCatalogItem*) * model.itemsCount);
    for (size_t i = 0; i < model.itemsCount; ++i)
    {
        requiredSize += CatalogItem::RequiredBufferSize(*model.items[i]);
    }
    return requiredSize;
}

HRESULT GetDraftItemsResponse::Copy(const PFCatalogGetDraftItemsResponse& input, PFCatalogGetDraftItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.continuationToken);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continuationToken = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<CatalogItem>(input.items, input.itemsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.items = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetEntityDraftItemsRequest::ToJson() const
{
    return GetEntityDraftItemsRequest::ToJson(this->Model());
}

JsonValue GetEntityDraftItemsRequest::ToJson(const PFCatalogGetEntityDraftItemsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "ContinuationToken", input.continuationToken);
    JsonUtils::ObjectAddMember(output, "Count", input.count);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Filter", input.filter);
    return output;
}

HRESULT GetEntityDraftItemsResponse::FromJson(const JsonValue& input)
{
    String continuationToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinuationToken", continuationToken));
    this->SetContinuationToken(std::move(continuationToken));

    ModelVector<CatalogItem> items{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogItem>(input, "Items", items));
    this->SetItems(std::move(items));

    return S_OK;
}

size_t GetEntityDraftItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetEntityDraftItemsResponse const*> GetEntityDraftItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetEntityDraftItemsResponse>(&this->Model());
}

size_t GetEntityDraftItemsResponse::RequiredBufferSize(const PFCatalogGetEntityDraftItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.continuationToken)
    {
        requiredSize += (std::strlen(model.continuationToken) + 1);
    }
    requiredSize += (alignof(PFCatalogCatalogItem*) + sizeof(PFCatalogCatalogItem*) * model.itemsCount);
    for (size_t i = 0; i < model.itemsCount; ++i)
    {
        requiredSize += CatalogItem::RequiredBufferSize(*model.items[i]);
    }
    return requiredSize;
}

HRESULT GetEntityDraftItemsResponse::Copy(const PFCatalogGetEntityDraftItemsResponse& input, PFCatalogGetEntityDraftItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.continuationToken);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continuationToken = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<CatalogItem>(input.items, input.itemsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.items = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetEntityItemReviewRequest::ToJson() const
{
    return GetEntityItemReviewRequest::ToJson(this->Model());
}

JsonValue GetEntityItemReviewRequest::ToJson(const PFCatalogGetEntityItemReviewRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

JsonValue Review::ToJson() const
{
    return Review::ToJson(this->Model());
}

JsonValue Review::ToJson(const PFCatalogReview& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CategoryRatings", input.categoryRatings, input.categoryRatingsCount);
    JsonUtils::ObjectAddMember(output, "HelpfulNegative", input.helpfulNegative);
    JsonUtils::ObjectAddMember(output, "HelpfulPositive", input.helpfulPositive);
    JsonUtils::ObjectAddMember(output, "IsInstalled", input.isInstalled);
    JsonUtils::ObjectAddMember(output, "ItemId", input.itemId);
    JsonUtils::ObjectAddMember(output, "ItemVersion", input.itemVersion);
    JsonUtils::ObjectAddMember(output, "Locale", input.locale);
    JsonUtils::ObjectAddMember(output, "Rating", input.rating);
    JsonUtils::ObjectAddMember<EntityKey>(output, "ReviewerEntity", input.reviewerEntity);
    JsonUtils::ObjectAddMember(output, "ReviewerId", input.reviewerId);
    JsonUtils::ObjectAddMember(output, "ReviewId", input.reviewId);
    JsonUtils::ObjectAddMember(output, "ReviewText", input.reviewText);
    JsonUtils::ObjectAddMemberTime(output, "Submitted", input.submitted);
    JsonUtils::ObjectAddMember(output, "Title", input.title);
    return output;
}

HRESULT Review::FromJson(const JsonValue& input)
{
    DictionaryEntryVector<PFInt32DictionaryEntry> categoryRatings{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CategoryRatings", categoryRatings));
    this->SetCategoryRatings(std::move(categoryRatings));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "HelpfulNegative", this->m_model.helpfulNegative));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "HelpfulPositive", this->m_model.helpfulPositive));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsInstalled", this->m_model.isInstalled));

    String itemId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemId", itemId));
    this->SetItemId(std::move(itemId));

    String itemVersion{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemVersion", itemVersion));
    this->SetItemVersion(std::move(itemVersion));

    String locale{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Locale", locale));
    this->SetLocale(std::move(locale));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Rating", this->m_model.rating));

    std::optional<EntityKey> reviewerEntity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ReviewerEntity", reviewerEntity));
    if (reviewerEntity)
    {
        this->SetReviewerEntity(std::move(*reviewerEntity));
    }

    String reviewerId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ReviewerId", reviewerId));
    this->SetReviewerId(std::move(reviewerId));

    String reviewId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ReviewId", reviewId));
    this->SetReviewId(std::move(reviewId));

    String reviewText{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ReviewText", reviewText));
    this->SetReviewText(std::move(reviewText));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Submitted", this->m_model.submitted));

    String title{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Title", title));
    this->SetTitle(std::move(title));

    return S_OK;
}

size_t Review::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogReview const*> Review::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Review>(&this->Model());
}

size_t Review::RequiredBufferSize(const PFCatalogReview& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFInt32DictionaryEntry) + sizeof(PFInt32DictionaryEntry) * model.categoryRatingsCount);
    for (size_t i = 0; i < model.categoryRatingsCount; ++i)
    {
        requiredSize += (std::strlen(model.categoryRatings[i].key) + 1);
    }
    if (model.itemId)
    {
        requiredSize += (std::strlen(model.itemId) + 1);
    }
    if (model.itemVersion)
    {
        requiredSize += (std::strlen(model.itemVersion) + 1);
    }
    if (model.locale)
    {
        requiredSize += (std::strlen(model.locale) + 1);
    }
    if (model.reviewerEntity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.reviewerEntity);
    }
    if (model.reviewerId)
    {
        requiredSize += (std::strlen(model.reviewerId) + 1);
    }
    if (model.reviewId)
    {
        requiredSize += (std::strlen(model.reviewId) + 1);
    }
    if (model.reviewText)
    {
        requiredSize += (std::strlen(model.reviewText) + 1);
    }
    if (model.title)
    {
        requiredSize += (std::strlen(model.title) + 1);
    }
    return requiredSize;
}

HRESULT Review::Copy(const PFCatalogReview& input, PFCatalogReview& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary(input.categoryRatings, input.categoryRatingsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.categoryRatings = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.itemId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.itemVersion);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemVersion = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.locale);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.locale = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.reviewerEntity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.reviewerEntity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.reviewerId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.reviewerId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.reviewId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.reviewId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.reviewText);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.reviewText = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.title);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.title = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetEntityItemReviewResponse::FromJson(const JsonValue& input)
{
    std::optional<Review> review{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Review", review));
    if (review)
    {
        this->SetReview(std::move(*review));
    }

    return S_OK;
}

size_t GetEntityItemReviewResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetEntityItemReviewResponse const*> GetEntityItemReviewResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetEntityItemReviewResponse>(&this->Model());
}

size_t GetEntityItemReviewResponse::RequiredBufferSize(const PFCatalogGetEntityItemReviewResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.review)
    {
        requiredSize += Review::RequiredBufferSize(*model.review);
    }
    return requiredSize;
}

HRESULT GetEntityItemReviewResponse::Copy(const PFCatalogGetEntityItemReviewResponse& input, PFCatalogGetEntityItemReviewResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<Review>(input.review);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.review = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetItemRequest::ToJson() const
{
    return GetItemRequest::ToJson(this->Model());
}

JsonValue GetItemRequest::ToJson(const PFCatalogGetItemRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

HRESULT GetItemResponse::FromJson(const JsonValue& input)
{
    std::optional<CatalogItem> item{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Item", item));
    if (item)
    {
        this->SetItem(std::move(*item));
    }

    return S_OK;
}

size_t GetItemResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetItemResponse const*> GetItemResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetItemResponse>(&this->Model());
}

size_t GetItemResponse::RequiredBufferSize(const PFCatalogGetItemResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.item)
    {
        requiredSize += CatalogItem::RequiredBufferSize(*model.item);
    }
    return requiredSize;
}

HRESULT GetItemResponse::Copy(const PFCatalogGetItemResponse& input, PFCatalogGetItemResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<CatalogItem>(input.item);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.item = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetItemContainersRequest::ToJson() const
{
    return GetItemContainersRequest::ToJson(this->Model());
}

JsonValue GetItemContainersRequest::ToJson(const PFCatalogGetItemContainersRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMember(output, "ContinuationToken", input.continuationToken);
    JsonUtils::ObjectAddMember(output, "Count", input.count);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

HRESULT GetItemContainersResponse::FromJson(const JsonValue& input)
{
    ModelVector<CatalogItem> containers{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogItem>(input, "Containers", containers));
    this->SetContainers(std::move(containers));

    String continuationToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinuationToken", continuationToken));
    this->SetContinuationToken(std::move(continuationToken));

    return S_OK;
}

size_t GetItemContainersResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetItemContainersResponse const*> GetItemContainersResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetItemContainersResponse>(&this->Model());
}

size_t GetItemContainersResponse::RequiredBufferSize(const PFCatalogGetItemContainersResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCatalogCatalogItem*) + sizeof(PFCatalogCatalogItem*) * model.containersCount);
    for (size_t i = 0; i < model.containersCount; ++i)
    {
        requiredSize += CatalogItem::RequiredBufferSize(*model.containers[i]);
    }
    if (model.continuationToken)
    {
        requiredSize += (std::strlen(model.continuationToken) + 1);
    }
    return requiredSize;
}

HRESULT GetItemContainersResponse::Copy(const PFCatalogGetItemContainersResponse& input, PFCatalogGetItemContainersResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CatalogItem>(input.containers, input.containersCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.containers = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.continuationToken);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continuationToken = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetItemModerationStateRequest::ToJson() const
{
    return GetItemModerationStateRequest::ToJson(this->Model());
}

JsonValue GetItemModerationStateRequest::ToJson(const PFCatalogGetItemModerationStateRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

HRESULT GetItemModerationStateResponse::FromJson(const JsonValue& input)
{
    std::optional<ModerationState> state{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "State", state));
    if (state)
    {
        this->SetState(std::move(*state));
    }

    return S_OK;
}

size_t GetItemModerationStateResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetItemModerationStateResponse const*> GetItemModerationStateResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetItemModerationStateResponse>(&this->Model());
}

size_t GetItemModerationStateResponse::RequiredBufferSize(const PFCatalogGetItemModerationStateResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.state)
    {
        requiredSize += ModerationState::RequiredBufferSize(*model.state);
    }
    return requiredSize;
}

HRESULT GetItemModerationStateResponse::Copy(const PFCatalogGetItemModerationStateResponse& input, PFCatalogGetItemModerationStateResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<ModerationState>(input.state);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.state = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetItemPublishStatusRequest::ToJson() const
{
    return GetItemPublishStatusRequest::ToJson(this->Model());
}

JsonValue GetItemPublishStatusRequest::ToJson(const PFCatalogGetItemPublishStatusRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

HRESULT GetItemPublishStatusResponse::FromJson(const JsonValue& input)
{
    std::optional<PFCatalogPublishResult> result{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Result", result));
    this->SetResult(std::move(result));

    String statusMessage{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StatusMessage", statusMessage));
    this->SetStatusMessage(std::move(statusMessage));

    return S_OK;
}

size_t GetItemPublishStatusResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetItemPublishStatusResponse const*> GetItemPublishStatusResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetItemPublishStatusResponse>(&this->Model());
}

size_t GetItemPublishStatusResponse::RequiredBufferSize(const PFCatalogGetItemPublishStatusResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.result)
    {
        requiredSize += (alignof(PFCatalogPublishResult) + sizeof(PFCatalogPublishResult));
    }
    if (model.statusMessage)
    {
        requiredSize += (std::strlen(model.statusMessage) + 1);
    }
    return requiredSize;
}

HRESULT GetItemPublishStatusResponse::Copy(const PFCatalogGetItemPublishStatusResponse& input, PFCatalogGetItemPublishStatusResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.result);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.result = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.statusMessage);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.statusMessage = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetItemReviewsRequest::ToJson() const
{
    return GetItemReviewsRequest::ToJson(this->Model());
}

JsonValue GetItemReviewsRequest::ToJson(const PFCatalogGetItemReviewsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMember(output, "ContinuationToken", input.continuationToken);
    JsonUtils::ObjectAddMember(output, "Count", input.count);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember(output, "OrderBy", input.orderBy);
    return output;
}

HRESULT GetItemReviewsResponse::FromJson(const JsonValue& input)
{
    String continuationToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinuationToken", continuationToken));
    this->SetContinuationToken(std::move(continuationToken));

    ModelVector<Review> reviews{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<Review>(input, "Reviews", reviews));
    this->SetReviews(std::move(reviews));

    return S_OK;
}

size_t GetItemReviewsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetItemReviewsResponse const*> GetItemReviewsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetItemReviewsResponse>(&this->Model());
}

size_t GetItemReviewsResponse::RequiredBufferSize(const PFCatalogGetItemReviewsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.continuationToken)
    {
        requiredSize += (std::strlen(model.continuationToken) + 1);
    }
    requiredSize += (alignof(PFCatalogReview*) + sizeof(PFCatalogReview*) * model.reviewsCount);
    for (size_t i = 0; i < model.reviewsCount; ++i)
    {
        requiredSize += Review::RequiredBufferSize(*model.reviews[i]);
    }
    return requiredSize;
}

HRESULT GetItemReviewsResponse::Copy(const PFCatalogGetItemReviewsResponse& input, PFCatalogGetItemReviewsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.continuationToken);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continuationToken = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<Review>(input.reviews, input.reviewsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.reviews = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetItemReviewSummaryRequest::ToJson() const
{
    return GetItemReviewSummaryRequest::ToJson(this->Model());
}

JsonValue GetItemReviewSummaryRequest::ToJson(const PFCatalogGetItemReviewSummaryRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

HRESULT GetItemReviewSummaryResponse::FromJson(const JsonValue& input)
{
    std::optional<Review> leastFavorableReview{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "LeastFavorableReview", leastFavorableReview));
    if (leastFavorableReview)
    {
        this->SetLeastFavorableReview(std::move(*leastFavorableReview));
    }

    std::optional<Review> mostFavorableReview{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MostFavorableReview", mostFavorableReview));
    if (mostFavorableReview)
    {
        this->SetMostFavorableReview(std::move(*mostFavorableReview));
    }

    std::optional<Rating> rating{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Rating", rating));
    if (rating)
    {
        this->SetRating(std::move(*rating));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ReviewsCount", this->m_model.reviewsCount));

    return S_OK;
}

size_t GetItemReviewSummaryResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetItemReviewSummaryResponse const*> GetItemReviewSummaryResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetItemReviewSummaryResponse>(&this->Model());
}

size_t GetItemReviewSummaryResponse::RequiredBufferSize(const PFCatalogGetItemReviewSummaryResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.leastFavorableReview)
    {
        requiredSize += Review::RequiredBufferSize(*model.leastFavorableReview);
    }
    if (model.mostFavorableReview)
    {
        requiredSize += Review::RequiredBufferSize(*model.mostFavorableReview);
    }
    if (model.rating)
    {
        requiredSize += Rating::RequiredBufferSize(*model.rating);
    }
    return requiredSize;
}

HRESULT GetItemReviewSummaryResponse::Copy(const PFCatalogGetItemReviewSummaryResponse& input, PFCatalogGetItemReviewSummaryResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<Review>(input.leastFavorableReview);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.leastFavorableReview = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<Review>(input.mostFavorableReview);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.mostFavorableReview = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<Rating>(input.rating);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.rating = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetItemsRequest::ToJson() const
{
    return GetItemsRequest::ToJson(this->Model());
}

JsonValue GetItemsRequest::ToJson(const PFCatalogGetItemsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<CatalogAlternateId>(output, "AlternateIds", input.alternateIds, input.alternateIdsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray(output, "Ids", input.ids, input.idsCount);
    return output;
}

HRESULT GetItemsResponse::FromJson(const JsonValue& input)
{
    ModelVector<CatalogItem> items{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogItem>(input, "Items", items));
    this->SetItems(std::move(items));

    return S_OK;
}

size_t GetItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogGetItemsResponse const*> GetItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetItemsResponse>(&this->Model());
}

size_t GetItemsResponse::RequiredBufferSize(const PFCatalogGetItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCatalogCatalogItem*) + sizeof(PFCatalogCatalogItem*) * model.itemsCount);
    for (size_t i = 0; i < model.itemsCount; ++i)
    {
        requiredSize += CatalogItem::RequiredBufferSize(*model.items[i]);
    }
    return requiredSize;
}

HRESULT GetItemsResponse::Copy(const PFCatalogGetItemsResponse& input, PFCatalogGetItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CatalogItem>(input.items, input.itemsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.items = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue PublishDraftItemRequest::ToJson() const
{
    return PublishDraftItemRequest::ToJson(this->Model());
}

JsonValue PublishDraftItemRequest::ToJson(const PFCatalogPublishDraftItemRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ETag", input.eTag);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

JsonValue ReportItemRequest::ToJson() const
{
    return ReportItemRequest::ToJson(this->Model());
}

JsonValue ReportItemRequest::ToJson(const PFCatalogReportItemRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMember(output, "ConcernCategory", input.concernCategory);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember(output, "Reason", input.reason);
    return output;
}

JsonValue ReportItemReviewRequest::ToJson() const
{
    return ReportItemReviewRequest::ToJson(this->Model());
}

JsonValue ReportItemReviewRequest::ToJson(const PFCatalogReportItemReviewRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMember(output, "ConcernCategory", input.concernCategory);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ItemId", input.itemId);
    JsonUtils::ObjectAddMember(output, "Reason", input.reason);
    JsonUtils::ObjectAddMember(output, "ReviewId", input.reviewId);
    return output;
}

JsonValue ReviewItemRequest::ToJson() const
{
    return ReviewItemRequest::ToJson(this->Model());
}

JsonValue ReviewItemRequest::ToJson(const PFCatalogReviewItemRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember<Review>(output, "Review", input.review);
    return output;
}

JsonValue StoreReference::ToJson() const
{
    return StoreReference::ToJson(this->Model());
}

JsonValue StoreReference::ToJson(const PFCatalogStoreReference& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    return output;
}

JsonValue SearchItemsRequest::ToJson() const
{
    return SearchItemsRequest::ToJson(this->Model());
}

JsonValue SearchItemsRequest::ToJson(const PFCatalogSearchItemsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "ContinuationToken", input.continuationToken);
    JsonUtils::ObjectAddMember(output, "Count", input.count);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Filter", input.filter);
    JsonUtils::ObjectAddMember(output, "Language", input.language);
    JsonUtils::ObjectAddMember(output, "OrderBy", input.orderBy);
    JsonUtils::ObjectAddMember(output, "Search", input.search);
    JsonUtils::ObjectAddMember(output, "Select", input.select);
    JsonUtils::ObjectAddMember<StoreReference>(output, "Store", input.store);
    return output;
}

HRESULT SearchItemsResponse::FromJson(const JsonValue& input)
{
    String continuationToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinuationToken", continuationToken));
    this->SetContinuationToken(std::move(continuationToken));

    ModelVector<CatalogItem> items{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CatalogItem>(input, "Items", items));
    this->SetItems(std::move(items));

    return S_OK;
}

size_t SearchItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogSearchItemsResponse const*> SearchItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SearchItemsResponse>(&this->Model());
}

size_t SearchItemsResponse::RequiredBufferSize(const PFCatalogSearchItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.continuationToken)
    {
        requiredSize += (std::strlen(model.continuationToken) + 1);
    }
    requiredSize += (alignof(PFCatalogCatalogItem*) + sizeof(PFCatalogCatalogItem*) * model.itemsCount);
    for (size_t i = 0; i < model.itemsCount; ++i)
    {
        requiredSize += CatalogItem::RequiredBufferSize(*model.items[i]);
    }
    return requiredSize;
}

HRESULT SearchItemsResponse::Copy(const PFCatalogSearchItemsResponse& input, PFCatalogSearchItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.continuationToken);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continuationToken = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<CatalogItem>(input.items, input.itemsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.items = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue SetItemModerationStateRequest::ToJson() const
{
    return SetItemModerationStateRequest::ToJson(this->Model());
}

JsonValue SetItemModerationStateRequest::ToJson(const PFCatalogSetItemModerationStateRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember(output, "Reason", input.reason);
    JsonUtils::ObjectAddMember(output, "Status", input.status);
    return output;
}

JsonValue SubmitItemReviewVoteRequest::ToJson() const
{
    return SubmitItemReviewVoteRequest::ToJson(this->Model());
}

JsonValue SubmitItemReviewVoteRequest::ToJson(const PFCatalogSubmitItemReviewVoteRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ItemId", input.itemId);
    JsonUtils::ObjectAddMember(output, "ReviewId", input.reviewId);
    JsonUtils::ObjectAddMember(output, "Vote", input.vote);
    return output;
}

JsonValue ReviewTakedown::ToJson() const
{
    return ReviewTakedown::ToJson(this->Model());
}

JsonValue ReviewTakedown::ToJson(const PFCatalogReviewTakedown& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogAlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMember(output, "ItemId", input.itemId);
    JsonUtils::ObjectAddMember(output, "ReviewId", input.reviewId);
    return output;
}

JsonValue TakedownItemReviewsRequest::ToJson() const
{
    return TakedownItemReviewsRequest::ToJson(this->Model());
}

JsonValue TakedownItemReviewsRequest::ToJson(const PFCatalogTakedownItemReviewsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray<ReviewTakedown>(output, "Reviews", input.reviews, input.reviewsCount);
    return output;
}

JsonValue UpdateCatalogConfigRequest::ToJson() const
{
    return UpdateCatalogConfigRequest::ToJson(this->Model());
}

JsonValue UpdateCatalogConfigRequest::ToJson(const PFCatalogUpdateCatalogConfigRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<CatalogConfig>(output, "Config", input.config);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    return output;
}

JsonValue UpdateDraftItemRequest::ToJson() const
{
    return UpdateDraftItemRequest::ToJson(this->Model());
}

JsonValue UpdateDraftItemRequest::ToJson(const PFCatalogUpdateDraftItemRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<CatalogItem>(output, "Item", input.item);
    JsonUtils::ObjectAddMember(output, "Publish", input.publish);
    return output;
}

HRESULT UpdateDraftItemResponse::FromJson(const JsonValue& input)
{
    std::optional<CatalogItem> item{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Item", item));
    if (item)
    {
        this->SetItem(std::move(*item));
    }

    return S_OK;
}

size_t UpdateDraftItemResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCatalogUpdateDraftItemResponse const*> UpdateDraftItemResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UpdateDraftItemResponse>(&this->Model());
}

size_t UpdateDraftItemResponse::RequiredBufferSize(const PFCatalogUpdateDraftItemResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.item)
    {
        requiredSize += CatalogItem::RequiredBufferSize(*model.item);
    }
    return requiredSize;
}

HRESULT UpdateDraftItemResponse::Copy(const PFCatalogUpdateDraftItemResponse& input, PFCatalogUpdateDraftItemResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<CatalogItem>(input.item);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.item = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Catalog

// Json serialization helpers

} // namespace PlayFab

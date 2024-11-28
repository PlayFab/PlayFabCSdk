#include "stdafx.h"
#include "TitleDataManagementTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace TitleDataManagement
{

JsonValue GetPublisherDataRequest::ToJson() const
{
    return GetPublisherDataRequest::ToJson(this->Model());
}

JsonValue GetPublisherDataRequest::ToJson(const PFTitleDataManagementGetPublisherDataRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray(output, "Keys", input.keys, input.keysCount);
    return output;
}

HRESULT GetPublisherDataResult::FromJson(const JsonValue& input)
{
    StringDictionaryEntryVector data{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Data", data));
    this->SetData(std::move(data));

    return S_OK;
}

size_t GetPublisherDataResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFTitleDataManagementGetPublisherDataResult const*> GetPublisherDataResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetPublisherDataResult>(&this->Model());
}

size_t GetPublisherDataResult::RequiredBufferSize(const PFTitleDataManagementGetPublisherDataResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.dataCount);
    for (size_t i = 0; i < model.dataCount; ++i)
    {
        requiredSize += (std::strlen(model.data[i].key) + 1);
        requiredSize += (std::strlen(model.data[i].value) + 1);
    }
    return requiredSize;
}

HRESULT GetPublisherDataResult::Copy(const PFTitleDataManagementGetPublisherDataResult& input, PFTitleDataManagementGetPublisherDataResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary(input.data, input.dataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.data = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetTimeResult::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Time", this->m_model.time));

    return S_OK;
}

size_t GetTimeResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFTitleDataManagementGetTimeResult const*> GetTimeResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetTimeResult>(&this->Model());
}

size_t GetTimeResult::RequiredBufferSize(const PFTitleDataManagementGetTimeResult& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT GetTimeResult::Copy(const PFTitleDataManagementGetTimeResult& input, PFTitleDataManagementGetTimeResult& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue GetTitleDataRequest::ToJson() const
{
    return GetTitleDataRequest::ToJson(this->Model());
}

JsonValue GetTitleDataRequest::ToJson(const PFTitleDataManagementGetTitleDataRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray(output, "Keys", input.keys, input.keysCount);
    JsonUtils::ObjectAddMember(output, "OverrideLabel", input.overrideLabel);
    return output;
}

HRESULT GetTitleDataResult::FromJson(const JsonValue& input)
{
    StringDictionaryEntryVector data{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Data", data));
    this->SetData(std::move(data));

    return S_OK;
}

size_t GetTitleDataResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFTitleDataManagementGetTitleDataResult const*> GetTitleDataResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetTitleDataResult>(&this->Model());
}

size_t GetTitleDataResult::RequiredBufferSize(const PFTitleDataManagementGetTitleDataResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.dataCount);
    for (size_t i = 0; i < model.dataCount; ++i)
    {
        requiredSize += (std::strlen(model.data[i].key) + 1);
        requiredSize += (std::strlen(model.data[i].value) + 1);
    }
    return requiredSize;
}

HRESULT GetTitleDataResult::Copy(const PFTitleDataManagementGetTitleDataResult& input, PFTitleDataManagementGetTitleDataResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary(input.data, input.dataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.data = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetTitleNewsRequest::ToJson() const
{
    return GetTitleNewsRequest::ToJson(this->Model());
}

JsonValue GetTitleNewsRequest::ToJson(const PFTitleDataManagementGetTitleNewsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Count", input.count);
    return output;
}

HRESULT TitleNewsItem::FromJson(const JsonValue& input)
{
    String body{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Body", body));
    this->SetBody(std::move(body));

    String newsId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NewsId", newsId));
    this->SetNewsId(std::move(newsId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Timestamp", this->m_model.timestamp));

    String title{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Title", title));
    this->SetTitle(std::move(title));

    return S_OK;
}

size_t TitleNewsItem::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFTitleDataManagementTitleNewsItem const*> TitleNewsItem::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TitleNewsItem>(&this->Model());
}

size_t TitleNewsItem::RequiredBufferSize(const PFTitleDataManagementTitleNewsItem& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.body)
    {
        requiredSize += (std::strlen(model.body) + 1);
    }
    if (model.newsId)
    {
        requiredSize += (std::strlen(model.newsId) + 1);
    }
    if (model.title)
    {
        requiredSize += (std::strlen(model.title) + 1);
    }
    return requiredSize;
}

HRESULT TitleNewsItem::Copy(const PFTitleDataManagementTitleNewsItem& input, PFTitleDataManagementTitleNewsItem& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.body);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.body = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.newsId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.newsId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.title);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.title = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetTitleNewsResult::FromJson(const JsonValue& input)
{
    ModelVector<TitleNewsItem> news{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<TitleNewsItem>(input, "News", news));
    this->SetNews(std::move(news));

    return S_OK;
}

size_t GetTitleNewsResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFTitleDataManagementGetTitleNewsResult const*> GetTitleNewsResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetTitleNewsResult>(&this->Model());
}

size_t GetTitleNewsResult::RequiredBufferSize(const PFTitleDataManagementGetTitleNewsResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFTitleDataManagementTitleNewsItem*) + sizeof(PFTitleDataManagementTitleNewsItem*) * model.newsCount);
    for (size_t i = 0; i < model.newsCount; ++i)
    {
        requiredSize += TitleNewsItem::RequiredBufferSize(*model.news[i]);
    }
    return requiredSize;
}

HRESULT GetTitleNewsResult::Copy(const PFTitleDataManagementGetTitleNewsResult& input, PFTitleDataManagementGetTitleNewsResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<TitleNewsItem>(input.news, input.newsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.news = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue SetPublisherDataRequest::ToJson() const
{
    return SetPublisherDataRequest::ToJson(this->Model());
}

JsonValue SetPublisherDataRequest::ToJson(const PFTitleDataManagementSetPublisherDataRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Key", input.key);
    JsonUtils::ObjectAddMember(output, "Value", input.value);
    return output;
}

JsonValue SetTitleDataRequest::ToJson() const
{
    return SetTitleDataRequest::ToJson(this->Model());
}

JsonValue SetTitleDataRequest::ToJson(const PFTitleDataManagementSetTitleDataRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Key", input.key);
    JsonUtils::ObjectAddMember(output, "Value", input.value);
    return output;
}

} // namespace TitleDataManagement

// Json serialization helpers

} // namespace PlayFab

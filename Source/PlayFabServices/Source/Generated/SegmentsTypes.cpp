#include "stdafx.h"
#include "SegmentsTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Segments
{

HRESULT GetSegmentResult::FromJson(const JsonValue& input)
{
    String aBTestParent{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ABTestParent", aBTestParent));
    this->SetABTestParent(std::move(aBTestParent));

    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    return S_OK;
}

size_t GetSegmentResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsGetSegmentResult const*> GetSegmentResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetSegmentResult>(&this->Model());
}

size_t GetSegmentResult::RequiredBufferSize(const PFSegmentsGetSegmentResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.aBTestParent)
    {
        requiredSize += (std::strlen(model.aBTestParent) + 1);
    }
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT GetSegmentResult::Copy(const PFSegmentsGetSegmentResult& input, PFSegmentsGetSegmentResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.aBTestParent);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.aBTestParent = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.id);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetPlayerSegmentsResult::FromJson(const JsonValue& input)
{
    ModelVector<GetSegmentResult> segments{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GetSegmentResult>(input, "Segments", segments));
    this->SetSegments(std::move(segments));

    return S_OK;
}

size_t GetPlayerSegmentsResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsGetPlayerSegmentsResult const*> GetPlayerSegmentsResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetPlayerSegmentsResult>(&this->Model());
}

size_t GetPlayerSegmentsResult::RequiredBufferSize(const PFSegmentsGetPlayerSegmentsResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFSegmentsGetSegmentResult*) + sizeof(PFSegmentsGetSegmentResult*) * model.segmentsCount);
    for (size_t i = 0; i < model.segmentsCount; ++i)
    {
        requiredSize += GetSegmentResult::RequiredBufferSize(*model.segments[i]);
    }
    return requiredSize;
}

HRESULT GetPlayerSegmentsResult::Copy(const PFSegmentsGetPlayerSegmentsResult& input, PFSegmentsGetPlayerSegmentsResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<GetSegmentResult>(input.segments, input.segmentsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.segments = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetPlayerTagsRequest::ToJson() const
{
    return GetPlayerTagsRequest::ToJson(this->Model());
}

JsonValue GetPlayerTagsRequest::ToJson(const PFSegmentsGetPlayerTagsRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Namespace", input.playfabNamespace);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

HRESULT GetPlayerTagsResult::FromJson(const JsonValue& input)
{
    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    return S_OK;
}

size_t GetPlayerTagsResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsGetPlayerTagsResult const*> GetPlayerTagsResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetPlayerTagsResult>(&this->Model());
}

size_t GetPlayerTagsResult::RequiredBufferSize(const PFSegmentsGetPlayerTagsResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    return requiredSize;
}

HRESULT GetPlayerTagsResult::Copy(const PFSegmentsGetPlayerTagsResult& input, PFSegmentsGetPlayerTagsResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue AddPlayerTagRequest::ToJson() const
{
    return AddPlayerTagRequest::ToJson(this->Model());
}

JsonValue AddPlayerTagRequest::ToJson(const PFSegmentsAddPlayerTagRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMember(output, "TagName", input.tagName);
    return output;
}

HRESULT GetAllSegmentsResult::FromJson(const JsonValue& input)
{
    ModelVector<GetSegmentResult> segments{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GetSegmentResult>(input, "Segments", segments));
    this->SetSegments(std::move(segments));

    return S_OK;
}

size_t GetAllSegmentsResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFSegmentsGetAllSegmentsResult const*> GetAllSegmentsResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetAllSegmentsResult>(&this->Model());
}

size_t GetAllSegmentsResult::RequiredBufferSize(const PFSegmentsGetAllSegmentsResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFSegmentsGetSegmentResult*) + sizeof(PFSegmentsGetSegmentResult*) * model.segmentsCount);
    for (size_t i = 0; i < model.segmentsCount; ++i)
    {
        requiredSize += GetSegmentResult::RequiredBufferSize(*model.segments[i]);
    }
    return requiredSize;
}

HRESULT GetAllSegmentsResult::Copy(const PFSegmentsGetAllSegmentsResult& input, PFSegmentsGetAllSegmentsResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<GetSegmentResult>(input.segments, input.segmentsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.segments = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetPlayersSegmentsRequest::ToJson() const
{
    return GetPlayersSegmentsRequest::ToJson(this->Model());
}

JsonValue GetPlayersSegmentsRequest::ToJson(const PFSegmentsGetPlayersSegmentsRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

JsonValue RemovePlayerTagRequest::ToJson() const
{
    return RemovePlayerTagRequest::ToJson(this->Model());
}

JsonValue RemovePlayerTagRequest::ToJson(const PFSegmentsRemovePlayerTagRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMember(output, "TagName", input.tagName);
    return output;
}

} // namespace Segments

// Json serialization helpers

} // namespace PlayFab

#include "stdafx.h"
#include "PlayStreamTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace PlayStream
{

JsonValue ExportPlayersInSegmentRequest::ToJson() const
{
    return ExportPlayersInSegmentRequest::ToJson(this->Model());
}

JsonValue ExportPlayersInSegmentRequest::ToJson(const PFPlayStreamExportPlayersInSegmentRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "SegmentId", input.segmentId);
    return output;
}

HRESULT ExportPlayersInSegmentResult::FromJson(const JsonValue& input)
{
    String exportId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ExportId", exportId));
    this->SetExportId(std::move(exportId));

    String segmentId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SegmentId", segmentId));
    this->SetSegmentId(std::move(segmentId));

    return S_OK;
}

size_t ExportPlayersInSegmentResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayStreamExportPlayersInSegmentResult const*> ExportPlayersInSegmentResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ExportPlayersInSegmentResult>(&this->Model());
}

size_t ExportPlayersInSegmentResult::RequiredBufferSize(const PFPlayStreamExportPlayersInSegmentResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.exportId)
    {
        requiredSize += (std::strlen(model.exportId) + 1);
    }
    if (model.segmentId)
    {
        requiredSize += (std::strlen(model.segmentId) + 1);
    }
    return requiredSize;
}

HRESULT ExportPlayersInSegmentResult::Copy(const PFPlayStreamExportPlayersInSegmentResult& input, PFPlayStreamExportPlayersInSegmentResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.exportId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.exportId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.segmentId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.segmentId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetPlayersInSegmentExportRequest::ToJson() const
{
    return GetPlayersInSegmentExportRequest::ToJson(this->Model());
}

JsonValue GetPlayersInSegmentExportRequest::ToJson(const PFPlayStreamGetPlayersInSegmentExportRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "ExportId", input.exportId);
    return output;
}

HRESULT GetPlayersInSegmentExportResponse::FromJson(const JsonValue& input)
{
    String indexUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IndexUrl", indexUrl));
    this->SetIndexUrl(std::move(indexUrl));

    String state{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "State", state));
    this->SetState(std::move(state));

    return S_OK;
}

size_t GetPlayersInSegmentExportResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayStreamGetPlayersInSegmentExportResponse const*> GetPlayersInSegmentExportResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetPlayersInSegmentExportResponse>(&this->Model());
}

size_t GetPlayersInSegmentExportResponse::RequiredBufferSize(const PFPlayStreamGetPlayersInSegmentExportResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.indexUrl)
    {
        requiredSize += (std::strlen(model.indexUrl) + 1);
    }
    if (model.state)
    {
        requiredSize += (std::strlen(model.state) + 1);
    }
    return requiredSize;
}

HRESULT GetPlayersInSegmentExportResponse::Copy(const PFPlayStreamGetPlayersInSegmentExportResponse& input, PFPlayStreamGetPlayersInSegmentExportResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.indexUrl);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.indexUrl = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.state);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.state = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetSegmentPlayerCountRequest::ToJson() const
{
    return GetSegmentPlayerCountRequest::ToJson(this->Model());
}

JsonValue GetSegmentPlayerCountRequest::ToJson(const PFPlayStreamGetSegmentPlayerCountRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "SegmentId", input.segmentId);
    return output;
}

HRESULT GetSegmentPlayerCountResult::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfilesInSegment", this->m_model.profilesInSegment));

    return S_OK;
}

size_t GetSegmentPlayerCountResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayStreamGetSegmentPlayerCountResult const*> GetSegmentPlayerCountResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetSegmentPlayerCountResult>(&this->Model());
}

size_t GetSegmentPlayerCountResult::RequiredBufferSize(const PFPlayStreamGetSegmentPlayerCountResult& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT GetSegmentPlayerCountResult::Copy(const PFPlayStreamGetSegmentPlayerCountResult& input, PFPlayStreamGetSegmentPlayerCountResult& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

} // namespace PlayStream

// Json serialization helpers

} // namespace PlayFab

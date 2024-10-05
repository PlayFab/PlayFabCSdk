#include "stdafx.h"
#include "Types.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{

JsonValue VersionConfiguration::ToJson() const
{
    return VersionConfiguration::ToJson(this->Model());
}

JsonValue VersionConfiguration::ToJson(const PFVersionConfiguration& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "MaxQueryableVersions", input.maxQueryableVersions);
    JsonUtils::ObjectAddMember(output, "ResetInterval", input.resetInterval);
    return output;
}

HRESULT VersionConfiguration::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MaxQueryableVersions", this->m_model.maxQueryableVersions));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ResetInterval", this->m_model.resetInterval));

    return S_OK;
}

size_t VersionConfiguration::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFVersionConfiguration const*> VersionConfiguration::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<VersionConfiguration>(&this->Model());
}

size_t VersionConfiguration::RequiredBufferSize(const PFVersionConfiguration& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT VersionConfiguration::Copy(const PFVersionConfiguration& input, PFVersionConfiguration& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}


// Json serialization helpers

} // namespace PlayFab

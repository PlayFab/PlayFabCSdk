#include "stdafx.h"

namespace PlayFab
{

JsonValue EntityKey::ToJson() const
{
    return EntityKey::ToJson(this->Model());
}

JsonValue EntityKey::ToJson(const PFEntityKey& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    return output;
}

HRESULT EntityKey::FromJson(const JsonValue& input)
{
    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    String type{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", type));
    this->SetType(std::move(type));

    return S_OK;
}

size_t EntityKey::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEntityKey const*> EntityKey::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityKey>(&this->Model());
}

size_t EntityKey::RequiredBufferSize(const PFEntityKey& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.type)
    {
        requiredSize += (std::strlen(model.type) + 1);
    }
    return requiredSize;
}

HRESULT EntityKey::Copy(const PFEntityKey& input, PFEntityKey& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.id);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.type);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.type = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace PlayFab

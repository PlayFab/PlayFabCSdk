#include "stdafx.h"
#include "ExperimentationTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Experimentation
{

JsonValue GetTreatmentAssignmentRequest::ToJson() const
{
    return GetTreatmentAssignmentRequest::ToJson(this->Model());
}

JsonValue GetTreatmentAssignmentRequest::ToJson(const PFExperimentationGetTreatmentAssignmentRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT GetTreatmentAssignmentResult::FromJson(const JsonValue& input)
{
    std::optional<TreatmentAssignment> treatmentAssignment{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TreatmentAssignment", treatmentAssignment));
    if (treatmentAssignment)
    {
        this->SetTreatmentAssignment(std::move(*treatmentAssignment));
    }

    return S_OK;
}

size_t GetTreatmentAssignmentResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFExperimentationGetTreatmentAssignmentResult const*> GetTreatmentAssignmentResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetTreatmentAssignmentResult>(&this->Model());
}

size_t GetTreatmentAssignmentResult::RequiredBufferSize(const PFExperimentationGetTreatmentAssignmentResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.treatmentAssignment)
    {
        requiredSize += TreatmentAssignment::RequiredBufferSize(*model.treatmentAssignment);
    }
    return requiredSize;
}

HRESULT GetTreatmentAssignmentResult::Copy(const PFExperimentationGetTreatmentAssignmentResult& input, PFExperimentationGetTreatmentAssignmentResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<TreatmentAssignment>(input.treatmentAssignment); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.treatmentAssignment = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Experimentation

// Json serialization helpers

} // namespace PlayFab

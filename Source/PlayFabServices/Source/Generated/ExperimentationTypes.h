#pragma once

#include <playfab/services/cpp/ExperimentationTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Experimentation
{

// Experimentation Classes
class GetTreatmentAssignmentRequest : public Wrappers::PFExperimentationGetTreatmentAssignmentRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFExperimentationGetTreatmentAssignmentRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFExperimentationGetTreatmentAssignmentRequest& input);
};

class GetTreatmentAssignmentResult : public Wrappers::PFExperimentationGetTreatmentAssignmentResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFExperimentationGetTreatmentAssignmentResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFExperimentationGetTreatmentAssignmentResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFExperimentationGetTreatmentAssignmentResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFExperimentationGetTreatmentAssignmentResult& model);
    static HRESULT Copy(const PFExperimentationGetTreatmentAssignmentResult& input, PFExperimentationGetTreatmentAssignmentResult& output, ModelBuffer& buffer);
};

} // namespace Experimentation
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

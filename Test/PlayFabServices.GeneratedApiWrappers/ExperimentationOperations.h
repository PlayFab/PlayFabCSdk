#pragma once

#include <playfab/services/cpp/ExperimentationTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class GetTreatmentAssignmentOperation : public XAsyncOperation<Wrappers::PFExperimentationGetTreatmentAssignmentResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFExperimentationGetTreatmentAssignmentRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFExperimentationGetTreatmentAssignmentResultWrapper<Allocator>;

    GetTreatmentAssignmentOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFExperimentationGetTreatmentAssignmentResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

}
}

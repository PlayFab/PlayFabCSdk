#include "TestAppPch.h"
#include "ExperimentationOperations.h"
#include <playfab/services/PFExperimentation.h>

namespace PlayFab
{
namespace Test
{


GetTreatmentAssignmentOperation::GetTreatmentAssignmentOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetTreatmentAssignmentOperation::ResultType> GetTreatmentAssignmentOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetTreatmentAssignmentOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetTreatmentAssignmentOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFExperimentationGetTreatmentAssignmentAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetTreatmentAssignmentOperation::ResultType> GetTreatmentAssignmentOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFExperimentationGetTreatmentAssignmentGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFExperimentationGetTreatmentAssignmentResult* result;
    RETURN_IF_FAILED(PFExperimentationGetTreatmentAssignmentGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

}
}

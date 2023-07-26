#include "stdafx.h"
#include "Experimentation.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace Experimentation
{


AsyncOp<GetTreatmentAssignmentResult> ExperimentationAPI::GetTreatmentAssignment(
    Entity const& entity,
    const GetTreatmentAssignmentRequest& request,
    RunContext rc
)
{
    const char* path{ "/Experimentation/GetTreatmentAssignment" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::ExperimentationGetTreatmentAssignment,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTreatmentAssignmentResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTreatmentAssignmentResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTreatmentAssignmentResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace Experimentation
} // namespace PlayFab

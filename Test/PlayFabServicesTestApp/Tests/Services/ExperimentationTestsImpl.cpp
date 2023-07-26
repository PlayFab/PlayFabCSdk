#include "TestAppPch.h"
#include "ExperimentationTests.h"
#include "ExperimentationOperations.h"

namespace PlayFab
{
namespace Test
{

AsyncOp<void> ExperimentationTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> ExperimentationTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void ExperimentationTests::TestGetTreatmentAssignment(TestContext& tc)
{
    GetTreatmentAssignmentOperation::RequestType request;
    request.SetEntity(DefaultTitlePlayer().EntityKey());

    GetTreatmentAssignmentOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetTreatmentAssignmentOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        auto& model = result.Payload().Model();
        tc.AssertTrue(model.treatmentAssignment, "treatmentAssignment");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}


}
}

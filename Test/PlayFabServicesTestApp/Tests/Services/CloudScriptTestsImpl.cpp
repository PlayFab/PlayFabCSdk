#include "TestAppPch.h"
#include "CloudScriptTests.h"
#include "CloudScriptOperations.h"

namespace PlayFab
{
namespace Test
{

AsyncOp<void> CloudScriptTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> CloudScriptTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void CloudScriptTests::TestExecuteFunction(TestContext& tc)
{
    // Requires manual setup in Azure Portal and PlayFab Game Manager
    ExecuteFunctionOperation::RequestType request;
    request.SetFunctionName("CloudScriptTestFunction");
    
    ExecuteFunctionOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ExecuteFunctionOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        tc.AssertEqual("CloudScriptTestFunction", result.Payload().Model().functionName, "functionName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

}
}

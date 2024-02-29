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

void CloudScriptTests::TestClientExecuteCloudScript(TestContext& tc)
{
    // TestReturnPlayerId is a custom Cloud Script function stored in PlayFab Game Manager
    ClientExecuteCloudScriptOperation::RequestType request;
    request.SetFunctionName("TestReturnPlayerId");

    ClientExecuteCloudScriptOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ClientExecuteCloudScriptOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual("TestReturnPlayerId", result.Payload().Model().functionName, "functionName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });

}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void CloudScriptTests::TestServerExecuteCloudScript(TestContext& tc)
{
    // TestGetServerFriendsList is a custom Cloud Script function stored in PlayFab Game Manager
    ServerExecuteCloudScriptOperation::RequestType request;
    request.SetFunctionName("TestGetServerFriendsList");
    request.SetPlayFabId(DefaultTitlePlayerId()); 

    ServerExecuteCloudScriptOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerExecuteCloudScriptOperation::ResultType> result) -> Result<void>
    {
            RETURN_IF_FAILED_PLAYFAB(result);
            tc.AssertEqual("TestGetServerFriendsList", result.Payload().Model().functionName, "functionName"); 

            return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });

}
#endif

void CloudScriptTests::TestExecuteEntityCloudScript(TestContext& tc)
{
    ExecuteEntityCloudScriptOperation::RequestType setRequest;

    // AddObject is a custom Cloud Script function given in PlayFab Game Manager
    setRequest.SetFunctionName("TestFunction");
    setRequest.SetEntity(DefaultTitlePlayer().EntityKey());

    ExecuteEntityCloudScriptOperation::Run(DefaultTitlePlayer(), setRequest, RunContext()).Then([&](Result<ExecuteEntityCloudScriptOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual("TestFunction", result.Payload().Model().functionName, "functionName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });

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

#include "stdafx.h"
#include "CloudScript.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace CloudScript
{


AsyncOp<ExecuteFunctionResult> CloudScriptAPI::ExecuteFunction(
    Entity const& entity,
    const ExecuteFunctionRequest& request,
    RunContext rc
)
{
    const char* path{ "/CloudScript/ExecuteFunction" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::CloudScriptExecuteFunction,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ExecuteFunctionResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ExecuteFunctionResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ExecuteFunctionResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace CloudScript
} // namespace PlayFab

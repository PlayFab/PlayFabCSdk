#include "stdafx.h"
#include "CloudScript.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace CloudScript
{


AsyncOp<ExecuteCloudScriptResult> CloudScriptAPI::ClientExecuteCloudScript(
    Entity const& entity,
    const ExecuteCloudScriptRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/ExecuteCloudScript" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::CloudScriptClientExecuteCloudScript,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ExecuteCloudScriptResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ExecuteCloudScriptResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ExecuteCloudScriptResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ExecuteCloudScriptResult> CloudScriptAPI::ServerExecuteCloudScript(
    Entity const& entity,
    const ExecuteCloudScriptServerRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/ExecuteCloudScript" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CloudScriptServerExecuteCloudScript,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ExecuteCloudScriptResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ExecuteCloudScriptResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ExecuteCloudScriptResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ExecuteCloudScriptResult> CloudScriptAPI::ExecuteEntityCloudScript(
    Entity const& entity,
    const ExecuteEntityCloudScriptRequest& request,
    RunContext rc
)
{
    const char* path{ "/CloudScript/ExecuteEntityCloudScript" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::CloudScriptExecuteEntityCloudScript,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ExecuteCloudScriptResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ExecuteCloudScriptResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ExecuteCloudScriptResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

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

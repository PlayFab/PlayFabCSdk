#include "stdafx.h"
#include "MultiplayerServer.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace MultiplayerServer
{


AsyncOp<ListBuildAliasesResponse> MultiplayerServerAPI::ListBuildAliases(
    Entity const& entity,
    const ListBuildAliasesRequest& request,
    RunContext rc
)
{
    const char* path{ "/MultiplayerServer/ListBuildAliases" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::MultiplayerServerListBuildAliases,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ListBuildAliasesResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ListBuildAliasesResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ListBuildAliasesResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ListBuildSummariesResponse> MultiplayerServerAPI::ListBuildSummariesV2(
    Entity const& entity,
    const ListBuildSummariesRequest& request,
    RunContext rc
)
{
    const char* path{ "/MultiplayerServer/ListBuildSummariesV2" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::MultiplayerServerListBuildSummariesV2,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ListBuildSummariesResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ListBuildSummariesResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ListBuildSummariesResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ListQosServersForTitleResponse> MultiplayerServerAPI::ListQosServersForTitle(
    Entity const& entity,
    const ListQosServersForTitleRequest& request,
    RunContext rc
)
{
    const char* path{ "/MultiplayerServer/ListQosServersForTitle" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::MultiplayerServerListQosServersForTitle,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ListQosServersForTitleResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ListQosServersForTitleResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ListQosServersForTitleResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<RequestMultiplayerServerResponse> MultiplayerServerAPI::RequestMultiplayerServer(
    Entity const& entity,
    const RequestMultiplayerServerRequest& request,
    RunContext rc
)
{
    const char* path{ "/MultiplayerServer/RequestMultiplayerServer" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::MultiplayerServerRequestMultiplayerServer,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RequestMultiplayerServerResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RequestMultiplayerServerResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RequestMultiplayerServerResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace MultiplayerServer
} // namespace PlayFab

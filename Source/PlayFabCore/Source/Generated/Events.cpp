#include "stdafx.h"
#include "Events.h"
#include "PFCoreGlobalState.h"

namespace PlayFab
{
namespace Events
{


AsyncOp<DeleteDataConnectionResponse> EventsAPI::DeleteDataConnection(
    SharedPtr<Entity> entity,
    const DeleteDataConnectionRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/DeleteDataConnection" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsDeleteDataConnection;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<DeleteDataConnectionResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            DeleteDataConnectionResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<DeleteDataConnectionResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetDataConnectionResponse> EventsAPI::GetDataConnection(
    SharedPtr<Entity> entity,
    const GetDataConnectionRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/GetDataConnection" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsGetDataConnection;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetDataConnectionResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetDataConnectionResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetDataConnectionResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ListDataConnectionsResponse> EventsAPI::ListDataConnections(
    SharedPtr<Entity> entity,
    const ListDataConnectionsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/ListDataConnections" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsListDataConnections;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ListDataConnectionsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ListDataConnectionsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ListDataConnectionsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<SetDataConnectionResponse> EventsAPI::SetDataConnection(
    SharedPtr<Entity> entity,
    const SetDataConnectionRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/SetDataConnection" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsSetDataConnection;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<SetDataConnectionResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            SetDataConnectionResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<SetDataConnectionResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<SetDataConnectionActiveResponse> EventsAPI::SetDataConnectionActive(
    SharedPtr<Entity> entity,
    const SetDataConnectionActiveRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/SetDataConnectionActive" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsSetDataConnectionActive;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<SetDataConnectionActiveResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            SetDataConnectionActiveResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<SetDataConnectionActiveResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<WriteEventsResponse> EventsAPI::WriteEvents(
    SharedPtr<Entity> entity,
    const WriteEventsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/WriteEvents" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsWriteEvents;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<WriteEventsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            WriteEventsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<WriteEventsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<WriteEventsResponse> EventsAPI::WriteTelemetryEvents(
    SharedPtr<Entity> entity,
    const WriteEventsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/WriteTelemetryEvents" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsWriteTelemetryEvents;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<WriteEventsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            WriteEventsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<WriteEventsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace Events
} // namespace PlayFab
